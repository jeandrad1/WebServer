
#ifndef EXECUTE_HPP
#define EXECUTE_HPP

#include <string>
#include <netinet/in.h>
#include <map>
#include "../builder/ServerConfig.hpp"
#include <vector>

# define BACKLOG 10

int createSocket();
void setupServer(const std::map<int, std::vector<ServerConfig * > > &servers);

#endif