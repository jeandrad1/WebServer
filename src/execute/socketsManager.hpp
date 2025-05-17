#ifndef SOCKETSMANAGER_HPP
# define SOCKETSMANAGER_HPP

#include <string>
#include <netinet/in.h>
#include <map>
#include <vector>
#include "../builder/ServerConfig.hpp"
#include "Socket.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

class socketsManager
{
    private:
        std::map<Socket *, int > mapServer;
    public:
        socketsManager();
        int createSocket();
        void setupServer(const std::map<int, std::vector<ServerConfig * > > &servers);
        void printMapServer();
};

#endif