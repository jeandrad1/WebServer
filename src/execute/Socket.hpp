#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <unistd.h>

class Socket
{
	private:
		int socket;
		Socket();
	public:
		Socket(int int_socket);
		~Socket();
		
		int getSocket() const;
		bool operator<(const Socket& other) const {return getSocket() < other.getSocket();}
		bool operator==(const Socket& other) const {return getSocket() == other.getSocket();}
};

#endif