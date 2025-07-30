#include "CgiHandler.hpp"
#include <stdio.h>
#include <sys/stat.h>
#include "ResponseFactory.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

CgiHandler::CgiHandler(HttpRequest *req, std::string clientIp, std::vector<ServerConfig *> servers)
	: _req(req), _clientIp(clientIp), _bytesWritten(0)
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
	if (!this->_location || this->_location->cgiDirective == false)
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

std::string CgiHandler::parseCgiBuffer()
{
    std::istringstream iss(this->_buffer);
    std::string line;
    std::string body;
    std::map<std::string, std::string> headerMap;
    bool headers_ended = false;

    // 1. Parsear los headers y el body desde el buffer del CGI
    while (std::getline(iss, line))
    {
        // ✅ CORRECCIÓN C++98: Reemplazar line.back() y line.pop_back()
        if (!line.empty() && line[line.size() - 1] == '\r')
        {
            line.erase(line.size() - 1);
        }

        if (!headers_ended)
        {
            if (line.empty())
            {
                headers_ended = true;
                continue;
            }
            size_t pos = line.find(':');
            if (pos != std::string::npos)
            {
                std::string key = trimSpaces(line.substr(0, pos));
                std::string value = trimSpaces(line.substr(pos + 1));
                headerMap[key] = value;
            }
        }
        else
        {
            body += line + "\n";
        }
    }

    // 2. Determinar la línea de estado HTTP
    std::string statusLine;
    if (headerMap.count("Status"))
    {
        statusLine = "HTTP/1.1 " + headerMap["Status"] + "\r\n";
        headerMap.erase("Status");
    }
    else if (headerMap.count("Location"))
    {
        statusLine = "HTTP/1.1 302 Found\r\n";
    }
    else
    {
        statusLine = "HTTP/1.1 200 OK\r\n";
    }

    // 3. Asegurar que Content-Length esté presente
    if (!headerMap.count("Content-Length"))
    {
        std::ostringstream oss;
        oss << body.size();
        headerMap["Content-Length"] = oss.str();
    }

    // 4. Construir la respuesta final, asegurando que Set-Cookie se incluya
    std::ostringstream headerStream;
    headerStream << statusLine;

    if (headerMap.count("Set-Cookie"))
    {
        headerStream << "Set-Cookie: " << headerMap["Set-Cookie"] << "\r\n";
        headerMap.erase("Set-Cookie");
    }

    for (std::map<std::string, std::string>::iterator it = headerMap.begin(); it != headerMap.end(); ++it)
    {
        headerStream << it->first << ": " << it->second << "\r\n";
    }
    headerStream << "\r\n";

    return headerStream.str() + body;
}

bool	CgiHandler::checkScriptPermissions(void)
{
	struct stat s;
	std::string script = ServerUtils::resolveScriptPath(_req, _location);

	if (script.find(".cgi") == std::string::npos)
	{
		if (stat(script.c_str(), &s) != 0)
		{
			ResponseFactory factory;
			if (errno == ENOENT)
			{
				factory.generateErrorResponse(404, *_server, _location);
				return (false);
			}
			else
			{
				factory.generateErrorResponse(404, *_server, _location);
				return (false);
			}
		}
		int result = access(script.c_str(), R_OK);
		if (result == -1)
		{
			ResponseFactory factory;
			perror("access");
			factory.generateErrorResponse(403, *_server, _location);
			return (false);
		}
	}
	return (true);
}

bool	CgiHandler::checkInterpreterPermissions(void)
{
	struct stat s;
	if (stat(this->_interpreterPath.c_str(), &s) != 0)
	{
		ResponseFactory factory;
		if (errno == ENOENT)
		{
			factory.generateErrorResponse(404, *_server, _location);
			return (false);
		}
		else
		{
			factory.generateErrorResponse(404, *_server, _location);
			return (false);
		}
	}
	int result = access(this->_interpreterPath.c_str(), X_OK);
	if (result == -1)
	{
		ResponseFactory factory;
		perror("access");
		factory.generateErrorResponse(403, *_server, _location);
		return (false);
	}
	return (true);
}

bool	CgiHandler::executeCgi(std::map<int, CgiHandler *> &cgiInputFd, std::map<int, CgiHandler *> &cgiOutputFd, int clientFd)
{
	this->_scriptPath = ServerUtils::resolveScriptPath(const_cast<HttpRequest *>(this->_req), const_cast<LocationConfig *>(this->_location));
	this->_interpreterPath = ServerUtils::resolveInterpreterPath(this->_location, this->_extension);

	if (!checkScriptPermissions() || !checkInterpreterPermissions())
		return (false);

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

		/*// DEBUG:
        std::cout << "---- CGI ENV ----\n";
        for (int i = 0; this->_envv[i]; ++i)
            std::cout << this->_envv[i] << std::endl;
        std::cout << "Interpreter: " << this->_interpreterPath << std::endl;
        std::cout << "Script: " << this->_scriptPath << std::endl;
        std::cout << "-----------------\n";
		std::string body = this->getRequestBody();
		std::cout << "---- CGI BODY ----\n" << body << "\n------------------\n";
		//DELETE THIS*/

		dup2(input_pipe[0], STDIN_FILENO);
		dup2(output_pipe[1], STDOUT_FILENO);

		close(input_pipe[1]);
		close(output_pipe[0]);

		if (this->_interpreterPath.empty() || this->_interpreterPath == this->_scriptPath)
		{
			char *argv[] = {
				const_cast<char*>(this->_scriptPath.c_str()),
				NULL
			};
			alarm(5);
			execve(argv[0], argv, this->_envv);
		}
		else
		{
			char *argv[] = {
				const_cast<char*>(this->_interpreterPath.c_str()),
				const_cast<char*>(this->_scriptPath.c_str()),
				NULL
			};
			alarm(5);
			execve(argv[0], argv, this->_envv);
		}
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
		else
    	{
			close(input_pipe[1]);
			this->_inputFdClosed = true;
    	}

		ServerUtils::setNotBlockingFd(output_pipe[0]);
		EpollManager::getInstance().addFd(output_pipe[0], EPOLLIN);
		this->_outputFd = output_pipe[0];
		this->_outputFdClosed = false;
		cgiOutputFd[output_pipe[0]] = this;
		return (true);
	}
	ResponseFactory factory;
	factory.generateErrorResponse(500, *_server, _location);
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

size_t& CgiHandler::getBytesSent(void)
{
	return (this->_bytesSent);
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

std::string	CgiHandler::getGivenEnvvByKey(std::string key)
{
	if (!this->_envv || !this->_envv[0])
		return (NULL);
	for (int i = 0; this->_envv[i]; i++)
    {
		std::string envvar = to_string(this->_envv[i]);
		size_t pos = envvar.find(key);
		if (pos != std::string::npos)
		{
			pos = envvar.find("=");
			std::string value = envvar.substr(pos + 1, envvar.size() - 1);
			return (value);
		}
	}
	return (NULL);
}

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/