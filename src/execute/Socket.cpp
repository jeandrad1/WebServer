# include "Socket.hpp"

Socket::Socket() {};

Socket::Socket(int int_socket)
{
	socket = int_socket;
}

Socket::~Socket()
{
	close(socket);
}

int Socket::getSocket() const
{
	return (socket);
}