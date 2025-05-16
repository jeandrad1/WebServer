#include "execute.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

int executer::createSocket()
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;

    if (socket_fd == -1)
    {
        std::cout << "Error: Failed to create socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cout << "Error: Failed to set socket options." << std::endl;
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

void executer::setupServer(const std::map<int, std::vector<ServerConfig * > > &servers)
{
	for (std::map<int, std::vector<ServerConfig * > >::const_iterator it = servers.begin(); it != servers.end(); ++it)
	{
		int port = it->first;
		const std::vector<ServerConfig *> &serverConfigs = it->second;

		int server_fd = createSocket();

		// Socket binding can be another function
		struct sockaddr_in server_addr;
		std::memset(&server_addr, 0, sizeof(server_addr));

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		{
			std::cout << "Error: Failed to bind socket to port " << port << std::endl;
			close(server_fd);
			continue;
		}

		// Store the socket FD for later use needed

		for (std::vector<ServerConfig*>::const_iterator its = serverConfigs.begin(); its != serverConfigs.end(); ++its)
        {
            mapServer[*its].push_back(server_fd);
        }
	}
}

void executer::printMapServer()
{
	std::map<ServerConfig *, std::vector<int> >::iterator it = mapServer.begin();
	for (; it != mapServer.end(); it++)
	{
		std::cout << YELLOW << "SERVER MAXBODYSIZE_NUMBER: " << CYAN <<(*it).first->clientMaxBodySize << WHITE"\n" ;
		std::vector<int>::iterator itt = (*it).second.begin();
		for (; itt != (*it).second.end(); itt++)
		{
			std::cout << "   -> " << (*itt) << "\n";
		}
	}
}