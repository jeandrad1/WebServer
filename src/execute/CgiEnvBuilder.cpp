#include "CgiEnvBuilder.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

CgiEnvBuilder::CgiEnvBuilder(HttpRequest *req, ServerConfig *server, LocationConfig *location, std::string clientIP)
	: _req(req), _server(server), _location(location), _clientIP(clientIP)
{

}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

std::vector<std::string>	CgiEnvBuilder::build(void)
{
	if (!_req || !_server || !_location)
		throw std::runtime_error("Invalid arguments to build Env Variables");

	addEnv("GATEWAY_INTERFACE", "CGI/1.1");
	addEnv("REQUEST_METHOD", _req->getMethod());
	addEnv("SCRIPT_NAME", _req->getPath());
	addEnv("QUERY_STRING", _req->getQueryString());
	addEnv("SERVER_NAME", _req->getServerName());
	addEnv("SERVER_PORT", _req->getServerPort());
	addEnv("SERVER_PROTOCOL", _req->getVersion());
	addEnv("REMOTE_ADDR", this->_clientIP);
	addEnv("HTTP_HOST", _req->getHost());
	addEnv("HTTP_USER_AGENT", _req->getUserAgent());
	addEnv("HTTP_ACCEPT", _req->getAccept());

	if (_req->getMethod() == "POST")
	{
		addEnv("CONTENT_LENGTH", _req->getContentLengthString());
		addEnv("CONTENT_TYPE", _req->getContentType());
	}
	return (this->_envv);
}

void	CgiEnvBuilder::addEnv(const std::string &key, const std::string &value)
{
	std::string newEnvVariable;

	newEnvVariable = key + "=" + value;
	this->_envv.push_back(newEnvVariable);
}

void	CgiEnvBuilder::printEnv(void)
{
	std::vector<std::string>::iterator ite = this->_envv.end();
	for (std::vector<std::string>::iterator it = this->_envv.begin(); it != ite; it++)
	{
		std::cout << (*it) << "\n";
	}
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

std::vector<std::string>	CgiEnvBuilder::getEnvv(void)
{
	return (this->_envv);
}

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/