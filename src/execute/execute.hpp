
#ifndef EXECUTE_HPP
# define EXECUTE_HPP

#include <string>
#include <netinet/in.h>
#include <map>
#include <vector>
#include "../builder/ServerConfig.hpp"

# define BACKLOG 10

class Socket
{
	private:
		int socket;
		Socket();
	public:
		Socket(int int_socket);
		~Socket();
		
		void getSocket();
};

class executer
{
    private:
        std::map<ServerConfig *, std::vector<int> > mapServer;
    public:
        int createSocket();
        void setupServer(const std::map<int, std::vector<ServerConfig * > > &servers);
        void printMapServer();
};
#endif