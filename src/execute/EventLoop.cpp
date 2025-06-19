#include "EventLoop.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

EventLoop::EventLoop(EpollManager &epollManager, std::map<Socket *, int> serverSockets, std::map<int, std::vector<ServerConfig *> >	servers)
    : _epollManager(epollManager), _serverSockets(serverSockets), _servers(servers)
{
    /* Register sockets into epoll */
	for (std::map<Socket *, int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); ++it)
	{
		int socketFd = it->first->getSocket();

		try
		{
			_epollManager.addFd(socketFd, EPOLLIN);
		}
		catch (const std::exception &e)
		{
			std::cerr << "Failed to add socketFd " << socketFd << " to epoll: " << e.what() << std::endl;
		}
	}
}

EventLoop::~EventLoop(void)
{

}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void EventLoop::runEventLoop(void)
{
	const int MAX_EVENTS = 1500;
	struct epoll_event events[MAX_EVENTS];

	while (true)
	{
		int n_ready = this->_epollManager.waitForEvents(MAX_EVENTS, -1, events);
		if (n_ready == -1)
		{
			perror("epoll_wait");
			continue ;
		}
		for (int i = 0; i < n_ready; ++i)
		{
			int fd = events[i].data.fd;

			if (isServerSocket(fd))
			{
				handleNewConnection(fd);
			}
			else
			{
				handleClientData(fd);
			}
		}
	}
}

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/

bool EventLoop::isServerSocket(int fd)
{
	for (std::map<Socket *, int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); ++it)
	{
		if (it->first->getSocket() == fd)
		{
			return (true);
		}
	}
	return (false);
}

void EventLoop::handleNewConnection(int serverFd)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(serverFd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd == -1)
	{
		perror("accept");
		return ;
	}

	// Set non-blocking
	fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK);

	try
	{
		this->_epollManager.addFd(client_fd, EPOLLIN | EPOLLET);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Failed to add client fd to epoll: " << e.what() << std::endl;
		close(client_fd);
		return ;
	}
	std::cout << "New connection accepted (fd = " << client_fd << ")\n";
}

void EventLoop::handleClientData(int clientFd)
{
	char buffer[4096];
	ssize_t count = recv(clientFd, buffer, sizeof(buffer), 0);  // Replaced read with recv
	if (count <= 0)
	{
		if (count < 0)
		{
			perror("recv");
			close(clientFd);
		}
		try
		{
			this->_epollManager.removeFd(clientFd);
		}
		catch (const std::exception &e)
		{
			std::cerr << "Failed to remove client fd from epoll: " << e.what() << std::endl;
		}
	}
	else
	{
		_buffers[clientFd].append(buffer, count);

		size_t header_end = _buffers[clientFd].find("\r\n\r\n");
		if (header_end != std::string::npos)
		{
			handleHttpRequest(clientFd, header_end);				
		}
		/*std::cout << "Received (" << fd << "): " << std::string(buffer, count);

		// Replace write with send
		const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nOK\n";
		ssize_t sent = send(fd, response, 41, 0); // 0 = default flags
		if (sent == -1)
		{
			perror("send");
			close(fd);
			epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
		}*/
	}
}

void EventLoop::handleHttpRequest(int clientFd, size_t header_end)
{
	try
	{
		HttpRequestManager reqMan;

		reqMan.parseHttpHeader(_buffers[clientFd]);
		HttpRequest *request = reqMan.buildHttpRequest();

		long long content_length = request->getContentLenght();
		size_t received_body_size = _buffers[clientFd].size() - (header_end + 4);

		if (received_body_size < content_length)
		{
			delete request;
			return ;
		}
		std::string full_request = _buffers[clientFd].substr(0, header_end + 4 + content_length);
		reqMan.parseHttpRequest(full_request);

		reqMan.requestPrinter();

		request->HttpRequestPrinter();
		delete request; //request manage temporal
	}
	catch(const std::exception& e)
	{
		std::cerr << RED "REQUEST PARSER ERROR: " << e.what() << WHITE "\n";;
	}	
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/
