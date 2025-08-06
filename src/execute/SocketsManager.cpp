#include "SocketsManager.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

SocketsManager::SocketsManager(){}

SocketsManager::~SocketsManager()
{
	for (std::map<Socket *, int>::iterator it = serverSockets.begin(); it != serverSockets.end(); ++it)
	{
		delete (*it).first;
	}
}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void SocketsManager::createSockets(const std::map<int, std::vector<ServerConfig * > > &servers)
{
	for (std::map<int, std::vector<ServerConfig * > >::const_iterator it = servers.begin(); it != servers.end(); ++it)
	{
		int port = it->first;

		int socket_fd = this->createListeningSocket(port);

		Socket *server_fd = new Socket(socket_fd);

		serverSockets[server_fd] = (*it).first;
	}
}

int SocketsManager::createListeningSocket(int port)
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
		std::cout << "Error: Failed to create socket." << std::endl;
        exit(EXIT_FAILURE);
    }
	
	int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cout << "Error: Failed to set socket options." << std::endl;
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

	struct sockaddr_in server_addr;
	std::memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		std::cout << "Error: Failed to bind socket to port " << port << std::endl;
		close(socket_fd);
	}

	if (listen(socket_fd, SOMAXCONN) == -1)
	{
		std::cout << "Error: Failed to listen on port " << port << std::endl;
		close(socket_fd);
	}

	if (fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK) == -1)
	{
		std::cout << "Error: Failed to set non-blocking mode on socket fd: " << socket_fd << std::endl;
		close(socket_fd);
	}
	return (socket_fd);
}

void SocketsManager::printServerSockets()
{
	std::map< Socket *, int >::iterator it = serverSockets.begin();
	for(; it != serverSockets.end(); it++)
	{
		Socket *ret = (*it).first;
		std::cout << WHITE"Socket(" << YELLOW << ret->getSocket() << WHITE")";
		std::cout << CYAN" -> " << GREEN << (*it).second << "\n" RESET; 
	}
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

std::map<Socket *, int> SocketsManager::getServerSockets()
{
	return (this->serverSockets);
}