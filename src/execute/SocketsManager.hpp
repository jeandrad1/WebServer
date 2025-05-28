#ifndef SOCKETSMANAGER_HPP
# define SOCKETSMANAGER_HPP

#include <string>
#include <netinet/in.h>
#include <map>
#include <vector>
#include "../builder/ServerConfig.hpp"
#include "Socket.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include <sys/epoll.h>
#include <fcntl.h>
#include <cstdio>
#include <errno.h>

#include "HttpRequestManager.hpp"

class SocketsManager
{
	private:

		std::map<Socket *, int > mapServer;

	public:

		SocketsManager();
		void	createSockets(const std::map<int, std::vector<ServerConfig * > > &servers);
		int		createListeningSocket(int port);
		void	registerServersToEpoll(int epoll_fd);
		void	handleConnections(int epoll_fd);
		void	runEpollLoop();
		void	printMapServer();
};

#endif