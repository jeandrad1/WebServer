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

class socketsManager
{
    private:
        std::map<Socket *, int > mapServer;
    public:
        socketsManager();
        void setupServer(const std::map<int, std::vector<ServerConfig * > > &servers);
        void startListening();
        void registerServersToEpoll(int epoll_fd);
        void handleConnections(int epoll_fd);
        void runEpollLoop();
        void printMapServer();

};

#endif