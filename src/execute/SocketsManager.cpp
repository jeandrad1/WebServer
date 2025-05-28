#include "SocketsManager.hpp"

SocketsManager::SocketsManager(){}

void SocketsManager::createSockets(const std::map<int, std::vector<ServerConfig * > > &servers)
{
	for (std::map<int, std::vector<ServerConfig * > >::const_iterator it = servers.begin(); it != servers.end(); ++it)
	{
		int port = it->first;

		int socket_fd = this->createListeningSocket(port);

		Socket *server_fd = new Socket(socket_fd);

		mapServer[server_fd] = (*it).first;
	}
}

int SocketsManager::createListeningSocket(int port)
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
		std::cout << "Error: Failed to create socket." << std::endl;
        exit(EXIT_FAILURE);
    }
	
	int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cout << "Error: Failed to set socket options." << std::endl;
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

	struct sockaddr_in server_addr;
	std::memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		std::cout << "Error: Failed to bind socket to port " << port << std::endl;
		close(socket_fd);
	}

	if (listen(socket_fd, SOMAXCONN) == -1)
	{
		std::cout << "Error: Failed to listen on port " << port << std::endl;
		close(socket_fd);
	}

	if (fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK) == -1)
	{
		std::cout << "Error: Failed to set non-blocking mode on socket fd: " << socket_fd << std::endl;
		close(socket_fd);
	}
	return (socket_fd);
}

void SocketsManager::registerServersToEpoll(int epoll_fd)
{
	for (std::map<Socket *, int>::iterator it = mapServer.begin(); it != mapServer.end(); ++it)
	{
		struct epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.fd = it->first->getSocket();

		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, it->first->getSocket(), &ev) == -1)
		{
			std::cout << "Error: epoll_ctl , listen socket failed" <<std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void SocketsManager::handleConnections(int epoll_fd)
{
	const int MAX_EVENTS = 1500;
	struct epoll_event events[MAX_EVENTS];

	while (true)
	{
		int n_ready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (n_ready == -1)
		{
			perror("epoll_wait");
			continue;
		}

		for (int i = 0; i < n_ready; ++i)
		{
			int fd = events[i].data.fd;

			// Check if it's a listening socket
			bool isServerSocket = false;
			for (std::map<Socket *, int>::iterator it = mapServer.begin(); it != mapServer.end(); ++it)
			{
				if (it->first->getSocket() == fd)
				{
					isServerSocket = true;
					break;
				}
			}

			if (isServerSocket)
			{
				struct sockaddr_in client_addr;
				socklen_t client_len = sizeof(client_addr);
				int client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
				if (client_fd == -1)
				{
					perror("accept");
					continue;
				}

				// Set non-blocking
				fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK);

				struct epoll_event client_ev;
				client_ev.events = EPOLLIN | EPOLLET; // Edge-triggered
				client_ev.data.fd = client_fd;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev) == -1)
				{
					perror("epoll_ctl: client_fd");
					close(client_fd);
					continue;
				}
				std::cout << "New connection accepted (fd = " << client_fd << ")\n";
			}
			else
			{
				char buffer[4096];
				ssize_t count = recv(fd, buffer, sizeof(buffer), 0);  // Replaced read with recv
				if (count <= 0)
				{
					if (count < 0) perror("recv");
					close(fd);
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
				}
				else
				{
					std::cout << "Received (" << fd << "): " << std::string(buffer, count);
					
					// Replace write with send
					const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nOK\n";
					ssize_t sent = send(fd, response, 41, 0); // 0 = default flags
					if (sent == -1)
					{
						perror("send");
						close(fd);
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
					}
				}
			}
		}
	}
}

void SocketsManager::runEpollLoop()
{
	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}

	registerServersToEpoll(epoll_fd);
	handleConnections(epoll_fd);
}

void SocketsManager::printMapServer()
{
	std::map< Socket *, int >::iterator it = mapServer.begin();
	for(; it != mapServer.end(); it++)
	{
		Socket *ret = (*it).first;
		std::cout << WHITE"Socket(" << YELLOW << ret->getSocket() << WHITE")";
		std::cout << CYAN" -> " << GREEN << (*it).second << "\n"; 
	}
}