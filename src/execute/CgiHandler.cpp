#include "CgiHandler.hpp"
#include <stdio.h>

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

CgiHandler::CgiHandler(HttpRequest *req, std::string clientIp, std::vector<ServerConfig *> servers)
	: _req(req), _clientIp(clientIp)
{
	this->_outputFdClosed = true;
	this->_inputFdClosed = true;
	if (!req)
		throw std::invalid_argument("HttpRequest pointer is null");
	if (servers.empty())
		throw std::invalid_argument("No servers found");

	this->_requestPath = req->getPath();

	this->_server = ServerUtils::getServerByHostName(req->getServerName(), servers);
	if (this->_server == NULL)
	{
		this->_server = servers[0];
		if (!this->_server)
			throw std::runtime_error("No matching server found for host: " + req->getServerName());
	}

	this->_location = ServerUtils::getLocationByRequestPath(this->_requestPath, this->_server);
	if (!this->_location)
	{
		throw std::runtime_error("No matching location found for host: " + this->_requestPath);
	}
}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

bool	CgiHandler::isCgiRequest(void)
{
	if (!this->_location && this->_location->cgiDirective == false)
	{
		std::cerr << "Error with location in CGI\n";
		return (false);
	}

	std::string::size_type dotPos = this->_requestPath.rfind('.');
	if (dotPos == std::string::npos)
		return (false);

	this->_extension = this->_requestPath.substr(dotPos);
	for (size_t i = 0; i < this->_location->cgi.size(); i++)
	{
		if (this->_extension == this->_location->cgi[i]->extension)
			return (true);
	}
	return (false);
}

std::string	CgiHandler::parseCgiBuffer(void)
{
	std::istringstream iss(this->_buffer);
	std::string			line;
	std::string			parsedBuffer;
	int					HttpHeaderFlag = -1;
	int					endHeadersFlag = -1;

	while (getline(iss, line))
	{
		if (line.empty())
			continue ;
		size_t colon = line.find(":");
		if (colon != std::string::npos && endHeadersFlag == -1)
		{
			std::string key = trimSpaces(line.substr(0, colon));
			std::string lowerKey = to_lowercase(key);

			if (lowerKey == "status")
			{
				parsedBuffer.append(CgiHeaderParser::parseStatusHeader(line));
				HttpHeaderFlag = 1;
			}
			else if (lowerKey == "content-type")
			{
				parsedBuffer.append(CgiHeaderParser::parseContentTypeHeader(line));
			}
			else if (lowerKey == "location")
			{
				parsedBuffer.append(CgiHeaderParser::parseLocationHeader(line));
			}
			else
			{
				if (CgiHeaderParser::parseFormatHeader(line))
				{
					if (to_lowercase(line) == "http/1.1 200 ok\n")
						HttpHeaderFlag = 1;
					parsedBuffer.append(trim(line) + "\r\n");
				}
			}
		}
		else
		{
			parsedBuffer.append(line);
			if (endHeadersFlag == -1)
			{
				endHeadersFlag = 1;
				parsedBuffer.append("Connection: close\r\n");
				parsedBuffer.append("\r\n");
			}
		}
	}
	if (HttpHeaderFlag == -1)
		parsedBuffer = "HTTP/1.1 200 OK\n" + parsedBuffer;
	return (parsedBuffer);
}

bool	CgiHandler::executeCgi(std::map<int, CgiHandler *> &cgiInputFd, std::map<int, CgiHandler *> &cgiOutputFd, int clientFd)
{
	this->_scriptPath = ServerUtils::resolveScriptPath(const_cast<HttpRequest *>(this->_req), const_cast<LocationConfig *>(this->_location));
	this->_interpreterPath = ServerUtils::resolveInterpreterPath(this->_location, this->_extension);
	this->_clientFd = clientFd;

	int	input_pipe[2];
	int output_pipe[2];

	pipe(input_pipe);
	pipe(output_pipe);

	pid_t pid = fork();
	if (pid == 0)
	{
		CgiEnvBuilder *envBuilder = new CgiEnvBuilder(this->_req, this->_server, this->_location, this->_clientIp);
		this->_envv = envBuilder->build();
		dup2(input_pipe[0], STDIN_FILENO);
		dup2(output_pipe[1], STDOUT_FILENO);

		close(input_pipe[1]);
		close(output_pipe[0]);
		close(STDIN_FILENO);
		char *argv[] = {const_cast<char*>(this->_interpreterPath.c_str()), const_cast<char*>(this->_scriptPath.c_str()), NULL};
		execve(argv[0], argv, this->_envv);
		std::cerr << "Falla\n";
		exit(1);
	}
	else
	{
		close(input_pipe[0]);
		close(output_pipe[1]);
		if (this->_req->getMethod() == "POST")
		{
			ServerUtils::setNotBlockingFd(input_pipe[1]);
			EpollManager::getInstance().addFd(input_pipe[1], EPOLLOUT);
			this->_inputFd = input_pipe[1];
			this->_inputFdClosed = false;
			cgiInputFd[input_pipe[1]] = this;
		}
		ServerUtils::setNotBlockingFd(output_pipe[0]);
		EpollManager::getInstance().addFd(output_pipe[0], EPOLLIN);
		this->_outputFd = output_pipe[0];
		this->_outputFdClosed = false;
		cgiOutputFd[output_pipe[0]] = this;
		return (true);
	}
	return (false);
}

void	CgiHandler::appendToCgiBuffer(char *buffer, ssize_t bytes_read)
{
	this->_buffer.append(buffer, bytes_read);
}

void	CgiHandler::updateBytesWritten(size_t newBytesWritten)
{
	size_t	actualBytes = this->_bytesWritten;

	this->_bytesWritten = actualBytes + newBytesWritten;
}

void	CgiHandler::setInputAsClosed(void)
{
	if (this->_inputFdClosed == false)
		this->_inputFdClosed = true;
}

void	CgiHandler::setOutputAsClosed(void)
{
	if (this->_outputFdClosed == false)
		this->_outputFdClosed = true;
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

int	CgiHandler::getClientFd(void)
{
	return (this->_clientFd);
}

int	CgiHandler::getInputFd(void)
{
	return (this->_inputFd);
}

int CgiHandler::getOutputFd(void)
{
	return (this->_outputFd);
}

size_t CgiHandler::getBytesWritten(void)
{
	return (this->_bytesWritten);
}

std::string CgiHandler::getBuffer(void)
{
	return (this->_buffer);
}

std::string CgiHandler::getRequestBody(void)
{
	return (to_string(this->_req->getBody()));
}

bool	CgiHandler::getInputFdClosed(void)
{
	return (this->_inputFdClosed);
}

bool	CgiHandler::getOutputFdClosed(void)
{
	return (this->_outputFdClosed);
}

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/