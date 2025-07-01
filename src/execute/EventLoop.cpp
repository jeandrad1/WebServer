#include "EventLoop.hpp"
#include "HttpRequestRouter.hpp"
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
	    int serverKey = -1;
    for (std::map<Socket *, int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); ++it)
    {
        if (it->first->getSocket() == serverFd)
        {
            serverKey = it->second;
            break;
        }
    }	

	// new to handle the connections correctly
    if (serverKey != -1)
    {
        _clientToServer[client_fd] = serverKey; 
        std::cout << "✓ Mapped client " << client_fd << " to server key " << serverKey << std::endl;
    }
    else
    {
        std::cout << "✗ ERROR: No server key found for serverFd " << serverFd << std::endl;
    }

	_clientToServer[client_fd] = serverKey; 

	_clientToServerSocket[client_fd] = serverFd;

	std::string ip = ServerUtils::getClientIP(client_addr);
	this->_ClientIPs[client_fd] = ip;

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
		close(clientFd);
		_buffers.erase(clientFd);
        _clientToServer.erase(clientFd);
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
			try
			{
				HttpRequest *request = handleHttpRequest(clientFd, header_end);
				if (request == NULL)
					return ;
				std::string ip = this->_ClientIPs[clientFd];
				CgiHandler *cgi = new CgiHandler(request, ip, getServersByFd(_clientToServerSocket[clientFd]));
				if (cgi->isCgiRequest())
				{
					std::cout << "CGI matches\n";
					cgi->buildEnv();
				}
				if (request != NULL)
				{
					std::cout << RED << "Received HTTP request from fd " << clientFd << RESET << ":\n";
					request->HttpRequestPrinter();

					int serverFd = _clientToServer[clientFd];

					//_servers[serverFd][0]->printValues();  
					if (_servers.find(serverFd) != _servers.end() && !_servers[serverFd].empty())
					{
						HttpRequestRouter router;
						HttpResponse response = router.handleRequest(*request, *(_servers[serverFd][0]));

						// Transform the response into a string
						std::ostringstream oss;
						oss << "HTTP/1.1 " << response.getStatusCode() << " " << response.getStatusMessage() << "\r\n";
						const std::map<std::string, std::string>& headers = response.getHeaders();
						for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
						{
							oss << it->first << ": " << it->second << "\r\n";
						}
						oss << "\r\n";
						oss << response.getBody();
						std::string responseStr = oss.str();

							// Send the response back to the client
						std::cout << "The response is:\n" << responseStr << std::endl;
						ssize_t sent = send(clientFd, responseStr.c_str(), responseStr.size(), 0);
						if (sent == -1)
						{
							perror("send");
							close(clientFd);
							_buffers.erase(clientFd);
							_clientToServer.erase(clientFd);
							try {
								_epollManager.removeFd(clientFd);
							} catch (const std::exception& e) {
								std::cerr << "Failed to remove fd: " << e.what() << std::endl;
							}
						}
						else
						{
							_buffers[clientFd].clear();
							std::cout << "✓ Response sent, connection kept alive" << std::endl;
						}			
						delete request;
					}
				}
			}
			catch (const std::exception &e)
			{
				std::cerr << "Failed cgi: " << e.what() << "\n";
			}
		}
	}
}

HttpRequest *EventLoop::handleHttpRequest(int clientFd, size_t header_end)
{
	try
	{
		HttpRequestManager reqMan;

		reqMan.parseHttpRequest(_buffers[clientFd]);
		HttpRequest *request = reqMan.buildHttpRequest();

		long long content_length = request->getContentLength();
		long long received_body_size = _buffers[clientFd].size() - (header_end + 4);

		if (received_body_size < content_length)
		{
			delete request;
			return (NULL);
		}
		std::string full_request = _buffers[clientFd].substr(0, header_end + 4 + content_length);
		reqMan.parseHttpRequest(full_request);
		request = reqMan.buildHttpRequest();
		reqMan.requestPrinter();

		return request;
	}
	catch(const std::exception& e)
	{
		std::cerr << RED "REQUEST PARSER ERROR: " << e.what() << WHITE "\n";;
		return NULL;
	}
	return NULL;
}


std::vector<ServerConfig *> EventLoop::getServersByFd(int fd)
{
	std::map<Socket *, int>::iterator ite = this->_serverSockets.end();
	int port = -1;
	for (std::map<Socket *, int>::iterator it = this->_serverSockets.begin(); it != ite; it++)
	{
		if (it->first->getSocket() == fd)
		{
			port = it->second;
			break ;
		}
	}
	std::vector<ServerConfig *> server = this->_servers[port];
	return (server);
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/
