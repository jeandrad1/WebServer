#include "EventLoop.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

EventLoop::EventLoop(EpollManager &epollManager, std::map<Socket *, int> serverSockets)
    : _epollManager(epollManager), _serverSockets(serverSockets)
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
            exit(1);
			continue;
		}
		for (int i = 0; i < n_ready; ++i)
		{
			int fd = events[i].data.fd;

			// Check if it's a listening socket
			bool isServerSocket = false;
			for (std::map<Socket *, int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); ++it)
			{
				if (it->first->getSocket() == fd)
				{
					isServerSocket = true;
					break;
				}
			}

			if (isServerSocket)
			{
				struct sockaddr_in client_addr;
				socklen_t client_len = sizeof(client_addr);
				int client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
				if (client_fd == -1)
				{
					perror("accept");
					continue;
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
					continue;
				}
				std::cout << "New connection accepted (fd = " << client_fd << ")\n";
			}
			else
			{
				char buffer[4096];
				ssize_t count = recv(fd, buffer, sizeof(buffer), 0);  // Replaced read with recv
				if (count <= 0)
				{
					if (count < 0) perror("recv");
					close(fd);
					try
					{
						this->_epollManager.removeFd(fd);
					}
					catch (const std::exception &e)
					{
						std::cerr << "Failed to add client fd to epoll: " << e.what() << std::endl;
					}
				}
				else
				{
					_buffers[fd].append(buffer, count);

					size_t header_end = _buffers[fd].find("\r\n\r\n");
					if (header_end != std::string::npos)
					{
						try
						{
							HttpRequestManager reqMan;
							reqMan.parseRequest(_buffers[fd]);

							HttpRequest *request = reqMan.builderHeadersRequest();
							long long content_length = request->getContentLenght();

							size_t received_body_size = _buffers[fd].size() - (header_end + 4);

							if (received_body_size < content_length)
							{
								delete request;
								continue;
							}
							else
							{
								std::string full_request = _buffers[fd].substr(0, header_end + 4 + content_length);
								reqMan.parseRequest(full_request);

								reqMan.requestPrinter();

								request->HttpRequestPrinter();
								delete request; //request manage temporal
							}
						}
						catch(const std::exception& e)
						{
							 std::cerr << RED "REQUEST PARSER ERROR: " << e.what() << WHITE "\n";;
						}
						
					}
					/*std::cout << "Received (" << fd << "): " << std::string(buffer, count);
					
					// Replace write with send
					const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nOK\n";
					ssize_t sent = send(fd, response, 41, 0); // 0 = default flags
					if (sent == -1)
					{
						perror("send");
						close(fd);
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);*/
				}
			}
		}
	}
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/