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
	CgiHeaderParser::loadMimeTypes();
	CgiHeaderParser::loadErrorCodes();
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
			std::cout << "Actual fd: " << fd << "\nN_ready: " << n_ready << "\n";
			if (isServerSocket(fd))
			{
				handleNewConnection(fd);
			}
			else if (isCgiOutputPipe(fd))
			{
				handleCgiOutput(fd);
			}
			else if (isCgiInputPipe(fd))
			{
				handleCgiInput(fd);
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

bool EventLoop::isCgiOutputPipe(int fd)
{
	std::cerr << "IscgiOutputPipe 2\n";
	for (std::map<int, CgiHandler *>::iterator it = this->_CgiOutputFds.begin(); it != this->_CgiOutputFds.end(); ++it)
	{
		if (it->first == fd)
		{
			std::cerr << "IscgiOutputPipe 1\n";
			return (true);
		}
	}
	return (false);
}

bool EventLoop::isCgiInputPipe(int fd)
{
	std::cerr << "IscgiInputPipe 2\n";
	for (std::map<int, CgiHandler *>::iterator it = this->_CgiInputFds.begin(); it != this->_CgiInputFds.end(); ++it)
	{
		if (it->first == fd)
		{
			std::cerr << "IscgiInputPipe 1\n";
			return (true);
		}
	}
	return (false);
}

void EventLoop::handleCgiOutput(int fd)
{
	CgiHandler *cgi = this->_CgiOutputFds[fd];

	char	buffer[4096];
	ssize_t	bytesRead = read(fd, buffer, sizeof(buffer));
	if (bytesRead > 0)
	{
		cgi->appendToCgiBuffer(buffer, bytesRead);
	}
	else if (bytesRead <= 0)
	{
		this->_epollManager.removeFd(fd);
		close(fd);
		cgi->setOutputAsClosed();
	}
	if (cgi->getInputFdClosed() == true && cgi->getOutputFdClosed() == true)
	{
		std::string buffer = cgi->parseCgiBuffer();
		send(cgi->getClientFd(), buffer.c_str(), buffer.size(), 0);
		this->_CgiOutputFds.erase(fd);
		close(fd);
	}
}

void EventLoop::handleCgiInput(int fd)
{
    CgiHandler *cgi = this->_CgiInputFds[fd];

    std::string body = cgi->getRequestBody();
    size_t bytesAlreadyWritten = cgi->getBytesWritten();
    size_t bodySize = body.size();

    size_t bytesToWrite = (bytesAlreadyWritten < bodySize) ? (bodySize - bytesAlreadyWritten) : 0;

	// DEBUG
    std::cerr << "[CGI DEBUG] BODY SIZE: " << bodySize 
              << " | bytesAlreadyWritten: " << bytesAlreadyWritten
              << " | bytesToWrite: " << bytesToWrite << std::endl;

    if (bytesToWrite > 0)
    {
        ssize_t bytesWritten = write(fd, body.data() + bytesAlreadyWritten, bytesToWrite);

		//DEBUG
        std::cerr << "[CGI DEBUG] write(fd=" << fd << ", offset=" << bytesAlreadyWritten 
                  << ", bytesToWrite=" << bytesToWrite << ") -> bytesWritten: " << bytesWritten << std::endl;
        if (bytesWritten > 0)
		{
            cgi->updateBytesWritten(bytesWritten);

			//DEBUG
			std::cerr << "[CGI DEBUG] Updated bytesWritten: " << cgi->getBytesWritten() << std::endl;
		}
        else
        {
			//DEBUG
			std::cerr << "[CGI DEBUG] write() returned <= 0, closing fd=" << fd << std::endl;

            this->_epollManager.removeFd(fd);
            close(fd);
            cgi->setInputAsClosed();
        }
    }
    else
    {
		// DEBUG
		std::cerr << "Cerrando pipe de entrada CGI fd=" << fd << std::endl;
		
        this->_epollManager.removeFd(fd);
        close(fd);
        cgi->setInputAsClosed();
		
		// DEBUG
		std::cerr << "Pipe de entrada CGI cerrado fd=" << fd << std::endl;

    }
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
	ServerUtils::setNotBlockingFd(client_fd);

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

HttpRequest* EventLoop::parseRequestFromBuffer(int clientFd, size_t header_end)
{
	std::string bufferStr(_requestBuffers[clientFd].begin(), _requestBuffers[clientFd].end());
	HttpRequestManager reqMan;
	reqMan.parseHttpRequest(bufferStr);
	HttpRequest *request = reqMan.buildHttpRequest();

	long long content_length = request->getContentLength();
	long long received_body_size = bufferStr.size() - (header_end + 4);
	long long chunked_end = bufferStr.find("\r\n0\r\n\r\n");

	if (((received_body_size < content_length) && request->getTransferEncoding() != "chunked") ||
		(request->getTransferEncoding() == "chunked" && static_cast<size_t>(chunked_end) == std::string::npos))
	{
		delete request;
		return NULL;
	}
	std::string full_request;
	if (request->getTransferEncoding() != "chunked")
		full_request = bufferStr.substr(0, header_end + 4 + content_length);
	else
		full_request = bufferStr.substr(0, chunked_end + 5);
	reqMan.parseHttpRequest(full_request);
	request = reqMan.buildHttpRequest();
	return request;
}

bool EventLoop::handleCgiIfNeeded(HttpRequest* request, int clientFd)
{
	std::string ip = this->_ClientIPs[clientFd];
	try
	{
		CgiHandler *cgi = new CgiHandler(request, ip, getServersByFd(_clientToServerSocket[clientFd]));
		if (cgi->isCgiRequest())
		{
			std::cout << "CGI matches\n";
			cgi->executeCgi(this->_CgiInputFds, this->_CgiOutputFds, clientFd);
			return true;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Failed cgi: " << e.what() << "\n";
	}
	return false;
}

void EventLoop::prepareResponseBuffer(const HttpResponse& response)
{
	_responseBuffer.clear();
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
	_responseBuffer.assign(responseStr.begin(), responseStr.end());
}

void EventLoop::sendResponse(int clientFd)
{
	ssize_t sent = send(clientFd, _responseBuffer.data(), _responseBuffer.size(), 0);
	if (sent == -1)
	{
		perror("send");
		close(clientFd);
		_requestBuffers.erase(clientFd);
		_clientToServer.erase(clientFd);
		try
		{
			_epollManager.removeFd(clientFd);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Failed to remove fd: " << e.what() << std::endl;
		}
	}
	else
	{
		_requestBuffers[clientFd].clear();
		std::cout << "✓ Response sent, connection kept alive" << std::endl;
	}
}


void EventLoop::handleClientData(int clientFd)
{
	char buffer[4096];
	ssize_t count = recv(clientFd, buffer, sizeof(buffer), 0);
	if (count <= 0)
	{
		if (count < 0)
		{
			perror("recv");
			close(clientFd);
		}
		close(clientFd);
		_requestBuffers.erase(clientFd);
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
		std::vector<char>& reqBuffer = _requestBuffers[clientFd];
		reqBuffer.insert(reqBuffer.end(), buffer, buffer + count);

		std::string reqStr(reqBuffer.begin(), reqBuffer.end());
		size_t header_end = reqStr.find("\r\n\r\n");
		if (header_end != std::string::npos)
		{
			HttpRequest *request = parseRequestFromBuffer(clientFd, header_end);
			if (request == NULL)
				return;
			if (handleCgiIfNeeded(request, clientFd))
				return;
			std::cout << RED << "Received HTTP request from fd " << clientFd << RESET << ":\n";
			request->HttpRequestPrinter();

			int serverFd = _clientToServer[clientFd];
			if (_servers.find(serverFd) != _servers.end() && !_servers[serverFd].empty())
			{
				HttpRequestRouter router;
				HttpResponse response = router.handleRequest(*request, *(_servers[serverFd][0]));
				prepareResponseBuffer(response);
				sendResponse(clientFd);
				delete request;
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
		long long chunked_end = _buffers[clientFd].find("\r\n0\r\n\r\n");

		if (((received_body_size < content_length) && request->getTransferEncoding() != "chunked") || (request->getTransferEncoding() == "chunked" && static_cast<size_t>(chunked_end) == std::string::npos))
		{
			delete request;
			return (NULL);
		}
		std::string full_request;
		if (request->getTransferEncoding() != "chunked")
			full_request = _buffers[clientFd].substr(0, header_end + 4 + content_length);
		else
			full_request = _buffers[clientFd].substr(0, chunked_end + 5);
		reqMan.parseHttpRequest(full_request);
		request = reqMan.buildHttpRequest();
		//reqMan.requestPrinter();

		return request;
	}
	catch(const std::exception& e)
	{
		ResponseFactory factory;
        HttpResponse res = factory.createBasicErrorResponse(501);
		
		std::ostringstream oss;
		oss << "HTTP/1.1 " << res.getStatusCode() << " " << res.getStatusMessage() << "\r\n";
		const std::map<std::string, std::string>& headers = res.getHeaders();
		for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		{
			oss << it->first << ": " << it->second << "\r\n";
		}
		oss << "\r\n";
		oss << res.getBody();
		std::string responseStr = oss.str();

		// lacks security
		send(clientFd, responseStr.c_str(), responseStr.size(), 0);

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
