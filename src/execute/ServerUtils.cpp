#include "ServerUtils.hpp"

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

ServerConfig *ServerUtils::getServerByHostName(std::string host, std::vector<ServerConfig *> servers)
{
	std::vector<ServerConfig *>::iterator ite = servers.end();
	for (std::vector<ServerConfig *>::iterator it = servers.begin(); it != ite; it++)
	{
		std::vector<std::string> serverName = (*it)->getServerNames();
		std::vector<std::string>::iterator itend = serverName.end();
		for (std::vector<std::string>::iterator itbeg = serverName.begin(); itbeg != itend; itbeg++)
		{
			if (host == (*itbeg))
			{
				return (*it);
			}
		}
	}
	return (NULL);
}

LocationConfig *ServerUtils::getLocationByRequestPath(std::string path, ServerConfig *server)
{
	if (server->getLocations().empty())
		return (NULL);
	std::string bestMatch;
	std::vector<LocationConfig *> location = server->getLocations();
	std::vector<LocationConfig *>::iterator bestIt = location.end();
	std::vector<LocationConfig *>::iterator ite = location.end();
	for (std::vector<LocationConfig *>::iterator it = location.begin(); it != ite; it++)
	{
		std::string locPath;
		if (path.find((*it)->getLocationPath()) == 0)
		{
			locPath = (*it)->getLocationPath();
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

std::string ServerUtils::getClientIP(struct sockaddr_in &client_addr)
{
	unsigned char* bytes = (unsigned char*)&client_addr.sin_addr.s_addr;
	std::ostringstream oss;
	oss << (int)bytes[0] << "." << (int)bytes[1] << "." << (int)bytes[2] << "." << (int)bytes[3];
	return oss.str();
}
