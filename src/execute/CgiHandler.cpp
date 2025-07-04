#include "CgiHandler.hpp"

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
		dup2(input_pipe[0], STDIN_FILENO);
		dup2(output_pipe[1], STDOUT_FILENO);
		close(input_pipe[1]);
		close(output_pipe[0]);

		char *argv[] = {const_cast<char*>(this->_interpreterPath), const_cast<char*>(this->_scriptPath), NULL};
		execve(argv[0], argv, this->_envv);
	}
	else
	{
		close(input_pipe[0]);
		close(output_pipe[1]);
/* 		if (this->_req->getMethod() == "POST")
		{
			char *this->_req->getBody()
			write(input_pipe[1], this->_req->getBody());
		} */
	}
}

void	CgiHandler::buildEnv(void)
{
	CgiEnvBuilder *envBuilder = new CgiEnvBuilder(this->_req, this->_server, this->_location, this->_clientIp);
	this->_envv = envBuilder->build();
	resolveScriptPath();
	envBuilder->printEnv();
	delete envBuilder;
}

void	CgiHandler::resolveScriptPath(void)
{
	std::string rootPath = this->_location->getRoot();
	std::string script = this->_req->getPath();
	std::string scriptPath = rootPath + script;
	this->_scriptPath = scriptPath.c_str();
}

void	CgiHandler::resolveInterpreterPath(void)
{
	std::vector<t_cgi *>::iterator ite = this->_location->cgi.end();
	for (std::vector<t_cgi *>::iterator it = this->_location->cgi.begin(); it != ite; it++)
	{
		if (this->_extension == (*it)->extension)
		{
			this->_interpreterPath = (*it)->path.c_str();
			break ;
		}
	}
}
/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/