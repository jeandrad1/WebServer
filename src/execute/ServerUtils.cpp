#include "ServerUtils.hpp"

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

ServerConfig *ServerUtils::getServerByHostName(std::string host, std::vector<ServerConfig *> servers)
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

LocationConfig *ServerUtils::getLocationByRequestPath(std::string path, ServerConfig *server)
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

std::string ServerUtils::getClientIP(struct sockaddr_in &client_addr)
{
	unsigned char* bytes = (unsigned char*)&client_addr.sin_addr.s_addr;
	std::ostringstream oss;
	oss << (int)bytes[0] << "." << (int)bytes[1] << "." << (int)bytes[2] << "." << (int)bytes[3];
	return oss.str();
}
