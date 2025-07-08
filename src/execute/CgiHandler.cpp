#include "CgiHandler.hpp"
#include <stdio.h>

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

CgiHandler::CgiHandler(HttpRequest *req, std::string clientIp, std::vector<ServerConfig *> servers)
	: _req(req), _clientIp(clientIp)
{
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
		throw std::runtime_error("No matching location found for host: " + this->_requestPath);
}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

bool	CgiHandler::isCgiRequest(void)
{
	if (this->_location->cgiDirective == false)
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

bool	CgiHandler::executeCgi(void)
{
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
		char *argv[] = {const_cast<char*>(this->_interpreterPath.c_str()), const_cast<char*>(this->_scriptPath.c_str()), NULL};
		execve(argv[0], argv, this->_envv);
		std::cerr << "Falla\n";
	}
	else
	{
		close(input_pipe[0]);
		close(output_pipe[1]);
		if (this->_req->getMethod() == "POST")
		{
			ServerUtils::setNotBlockingFd(input_pipe[1]);
			EpollManager::getInstance().addFd(input_pipe[1], EPOLLIN);
		}
		ServerUtils::setNotBlockingFd(output_pipe[0]);
		EpollManager::getInstance().addFd(output_pipe[0], EPOLLOUT);
	}
	return (0);
}

void	CgiHandler::buildEnv(void)
{
	this->_scriptPath = ServerUtils::resolveScriptPath(const_cast<HttpRequest *>(this->_req), const_cast<LocationConfig *>(this->_location));
	this->_interpreterPath = ServerUtils::resolveInterpreterPath(this->_location, this->_extension);
	this->executeCgi();
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/