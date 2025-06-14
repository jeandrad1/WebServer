# include "Socket.hpp"
# include <fcntl.h>

Socket::Socket() {};

Socket::Socket(int socket_fd)
{
    _socket = socket_fd;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		std::cout<<"setsockopt"<<std::endl;
		close(_socket);
		_socket = -1; // Set to -1 to indicate an error
	}
	else
	{
		fcntl(_socket, F_SETFL, O_NONBLOCK); // Set the socket to non-blocking mode
	}
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