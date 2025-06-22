#include "CgiDetector.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

bool	CgiDetector::isCgiRequest(HttpRequest *req, std::vector<ServerConfig *> servers)
{
	ServerConfig *server;
	LocationConfig *location;
	if (servers.empty())
	{
		std::cerr << "Error with servers empty\n";
		return (false);
	}
	server = getServerByHostName(req->getServerName(), servers);
	if (server == NULL)
	{
		server = servers[0];
		if (server == NULL)
		{
			std::cerr << "Error with hostName in CGI\n";
			return (false);
		}
	}
	location = getLocationByRequestPath(req->getPath(), server);
	if (location == NULL || location->cgiDirective == false)
	{
		std::cerr << "Error with location in CGI\n";
		return (false);
	}

	std::string::size_type dotPos = req->getPath().rfind('.');
	if (dotPos == std::string::npos)
		return (false);

	std::string extension = req->getPath().substr(dotPos);
	for (size_t i = 0; i < location->cgi.size(); i++)
	{
		if (extension == location->cgi[i]->extension)
			return (true);
	}
	return (false);
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/

ServerConfig *CgiDetector::getServerByHostName(std::string host, std::vector<ServerConfig *> servers)
{
	std::vector<ServerConfig *>::iterator ite = servers.end();
	for (std::vector<ServerConfig *>::iterator it = servers.begin(); it != ite; it++)
	{
		std::vector<std::string>::iterator itend = (*it)->serverNames.end();
		for (std::vector<std::string>::iterator itbeg = (*it)->serverNames.begin(); itbeg != itend; itbeg++)
		{
			if (host == (*itbeg))
			{
				return (*it);
			}
		}
	}
	return (NULL);
}

LocationConfig *CgiDetector::getLocationByRequestPath(std::string path, ServerConfig *server)
{
	if (server->locations.empty())
		return (NULL);
	std::string bestMatch;
	std::vector<LocationConfig *>::iterator bestIt = server->locations.end();
	std::vector<LocationConfig *>::iterator ite = server->locations.end();
	for (std::vector<LocationConfig *>::iterator it = server->locations.begin(); it != ite; it++)
	{
		std::string locPath;
		if (path.find((*it)->locationPath) == 0)
		{
			locPath = (*it)->locationPath;
			if (locPath.length() > bestMatch.length())
			{
				bestMatch = locPath;
				bestIt = it;
			}
		}
	}
	if (bestIt == ite)
		std::cerr << "Error: Not location block found for cgi\n";
	return ((*bestIt));
}