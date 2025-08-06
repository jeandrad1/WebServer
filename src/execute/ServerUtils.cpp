#include "ServerUtils.hpp"
#include <unistd.h>

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

	std::string filename = path.substr(path.find_last_of("/") + 1);

	std::vector<LocationConfig *>::iterator ite = location.end();
	for (std::vector<LocationConfig *>::iterator it = location.begin(); it != ite; it++)
	{
		std::string locPath = (*it)->getLocationPath();
		if (path.find(locPath) == 0 || locPath == "/" + filename)
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
	{
		std::cerr << "Error: Not location block found for cgi\n";
		return (NULL);
	}
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
	std::string script;
	std::string rootPath = location->getRoot();
	std::string path = req->getPath();

	if (rootPath.compare("/") == 0)
	{
		script = path;
		return (script.substr(1, script.size() - 1));
	}
	else
		script = path.substr(path.find_last_of("/") + 1);
	std::string scriptPath = rootPath.substr(1) + "/" + script;
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

std::vector<char> ServerUtils::prepareResponseBuffer(const HttpResponse& response)
{
	std::vector<char> responseBuffer;

	std::ostringstream oss;
	oss << "HTTP/1.1 " << response.getStatusCode() << " " << response.getStatusMessage() << "\r\n";
	const std::map<std::string, std::string>& headers = response.getHeaders();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
		oss << it->first << ": " << it->second << "\r\n";
	}
	oss << "\r\n";
	std::string headerStr = oss.str();
	responseBuffer.insert(responseBuffer.end(), headerStr.begin(), headerStr.end());

	if (response.bodyIsBinary())
	{
		const std::vector<unsigned char>& body = response.getBodyBinary();
		responseBuffer.insert(responseBuffer.end(), body.begin(), body.end());
	}
	else
	{
		std::string bodyStr = response.getBody();
		responseBuffer.insert(responseBuffer.end(), bodyStr.begin(), bodyStr.end());
	}
	return (responseBuffer);
}

void ServerUtils::sendErrorResponse(int clientFd, std::vector<char> responseBuffer)
{
	size_t bufferSize = responseBuffer.size();

	ssize_t sent = send(clientFd, responseBuffer.data(), bufferSize, 0);

	if (sent == -1)
	{
		close(clientFd);
		return;
	}
}