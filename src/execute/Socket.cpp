# include "Socket.hpp"

Socket::Socket() {};

Socket::Socket(int socket_fd)
{
	_socket = socket_fd;
}

Socket::~Socket()
{
    if (_socket > 0)
	    close(_socket);
}

int Socket::getSocket() const
{
	return (_socket);
}