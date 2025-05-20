#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>

class Socket
{
	private:
		int _socket;
		Socket();
	public:
		Socket(int int_socket);
		~Socket();
		
		int getSocket() const;
		bool operator<(const Socket& other) const {return getSocket() < other.getSocket();}
		bool operator==(const Socket& other) const {return getSocket() == other.getSocket();}
};

#endif