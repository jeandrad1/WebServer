# include "Socket.hpp"

Socket::Socket() {};

Socket::Socket(int int_socket)
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;

    if (_socket == -1)
    {
        std::cout << "Error: Failed to create socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cout << "Error: Failed to set socket options." << std::endl;
        close(_socket);
        exit(EXIT_FAILURE);
    }
}

Socket::~Socket()
{
	close(_socket);
}

int Socket::getSocket() const
{
	return (_socket);
}