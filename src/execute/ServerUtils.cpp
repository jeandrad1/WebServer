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

std::string	ServerUtils::resolveScriptPath(const HttpRequest *req, const LocationConfig *location)
{
	std::string rootPath = location->getRoot();
	std::string script = req->getPath();
	if (rootPath.compare("/") == 0)
		return (script);
	std::string scriptPath = rootPath + script;
	return (scriptPath);
}

std::string ServerUtils::resolveInterpreterPath(LocationConfig *location, std::string extension)
{
	std::string interpreterPath;

	std::vector<t_cgi *>::iterator ite = location->cgi.end();
	for (std::vector<t_cgi *>::iterator it = location->cgi.begin(); it != ite; it++)
	{
		if (extension == (*it)->extension)
		{
			interpreterPath = (*it)->path;
			if (extension == ".cgi")
				interpreterPath = "." + interpreterPath;
			break ;
		}
	}
	return (interpreterPath);
}

void	ServerUtils::setNotBlockingFd(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags <= -1)
	{
		return ;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		std::cerr << "Error setting Not Blocking pipe\n";
	}
}