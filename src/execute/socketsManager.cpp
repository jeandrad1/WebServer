#include "socketsManager.hpp"

socketsManager::socketsManager(){}

void socketsManager::setupServer(const std::map<int, std::vector<ServerConfig * > > &servers)
{
	for (std::map<int, std::vector<ServerConfig * > >::const_iterator it = servers.begin(); it != servers.end(); ++it)
	{
		int port = it->first;

		Socket *server_fd = new Socket(port);

		struct sockaddr_in server_addr;
		std::memset(&server_addr, 0, sizeof(server_addr));

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(server_fd->getSocket(), (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		{
			std::cout << "Error: Failed to bind socket to port " << port << std::endl;
			close(server_fd->getSocket());
			continue;
		}
		mapServer[server_fd] = (*it).first;
	}
}

void socketsManager::printMapServer()
{
	std::map< Socket *, int >::iterator it = mapServer.begin();
	for(; it != mapServer.end(); it++)
	{
		Socket *ret = (*it).first;
		std::cout << WHITE"Socket(" << YELLOW << ret->getSocket() << WHITE")";
		std::cout << CYAN" -> " << GREEN << (*it).second << "\n"; 
	}
}