#ifndef SOCKETSMANAGER_HPP
# define SOCKETSMANAGER_HPP

#include <string>
#include <netinet/in.h>
#include <map>
#include <vector>
#include "../builder/ServerConfig.hpp"
#include "Socket.hpp"

class socketsManager
{
    private:
        std::map<Socket *, int > mapServer;
    public:
        socketsManager();
        void setupServer(const std::map<int, std::vector<ServerConfig * > > &servers);
        void printMapServer();
};

#endif