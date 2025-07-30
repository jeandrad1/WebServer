#include "EventLoop.hpp"
#include "HttpRequestRouter.hpp"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>

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
			uint32_t revents = events[i].events;

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
				if (revents & EPOLLOUT)
					sendResponse(fd);
				if (revents & EPOLLIN)
					handleClientData(fd);
				if ((revents & EPOLLHUP) || (revents & EPOLLERR))
				{
					close(fd);
					_requestBuffers.erase(fd);
					_clientToServer.erase(fd);
					_clientSendOffset.erase(fd);
					_epollManager.removeFd(fd);
				}
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
	for (std::map<int, CgiHandler *>::iterator it = this->_CgiOutputFds.begin(); it != this->_CgiOutputFds.end(); ++it)
	{
		if (it->first == fd)
		{
			return (true);
		}
	}
	return (false);
}

bool EventLoop::isCgiInputPipe(int fd)
{
	for (std::map<int, CgiHandler *>::iterator it = this->_CgiInputFds.begin(); it != this->_CgiInputFds.end(); ++it)
	{
		if (it->first == fd)
		{
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
        //std::cout << YELLOW << "CGI OUTPUT READ (" << bytesRead << " bytes): " << std::string(buffer, bytesRead) << RESET << std::endl;
        
        cgi->appendToCgiBuffer(buffer, bytesRead);
        
        //std::cout << CYAN << "CGI BUFFER COMPLETO: " << cgi->getBuffer() << RESET << std::endl;
    }
    else if (bytesRead <= 0)
    {
        //std::cout << MAGENTA << "CGI OUTPUT CERRADO (fd=" << fd << ")" << RESET << std::endl;
        this->_epollManager.removeFd(fd);
        cgi->setOutputAsClosed();
    }
    
    if (cgi->getInputFdClosed() == true && cgi->getOutputFdClosed() == true)
    {
        std::string parsedBuffer = cgi->parseCgiBuffer();
        _responseBuffer.assign(parsedBuffer.begin(), parsedBuffer.end());
        _clientSendOffset[cgi->getClientFd()] = 0;
        _epollManager.modifyFd(cgi->getClientFd(), EPOLLIN | EPOLLOUT);
        this->_CgiOutputFds.erase(fd);
        delete cgi;
        std::cout << GREEN << "CGI HANDLER ELIMINADO" << RESET << std::endl;
    }
}

void EventLoop::handleCgiInput(int fd)
{
    CgiHandler *cgi = this->_CgiInputFds[fd];
	if (!cgi)
		return ;

    std::string body = cgi->getRequestBody();

	//std::cout<<"requestBody para cgi: "<< body << std::endl;
	//size_t header_end = body.find("\r\n\r\n");

    size_t bytesAlreadyWritten = cgi->getBytesWritten();
    size_t bodySize = body.size();
	
    if (bytesAlreadyWritten >= bodySize)
    {
        this->_epollManager.removeFd(fd);
        close(fd);
        cgi->setInputAsClosed();
        this->_CgiInputFds.erase(fd);
        return;
    }

    size_t bytesToWrite = bodySize - bytesAlreadyWritten;
    ssize_t bytesWritten = write(fd, body.data() + bytesAlreadyWritten, bytesToWrite);

    if (bytesWritten > 0)
    {
        cgi->updateBytesWritten(bytesWritten);
        if (cgi->getBytesWritten() >= bodySize)
        {
            this->_epollManager.removeFd(fd);
            close(fd);
            cgi->setInputAsClosed();
            this->_CgiInputFds.erase(fd);
        }
    }
    else
    {
        this->_epollManager.removeFd(fd);
        close(fd);
        cgi->setInputAsClosed();
        this->_CgiInputFds.erase(fd);
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
	reqMan.parseHttpRequest(bufferStr, this->getServersByFd(clientFd));
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
	reqMan.parseHttpRequest(full_request, this->getServersByFd(clientFd));
	delete request;
	request = reqMan.buildHttpRequest();
	
	const size_t LARGE_BODY_THRESHOLD = 1024 * 1024; // 1MB
    if (request->getContentLength() > 0 && static_cast<size_t>(request->getContentLength()) > LARGE_BODY_THRESHOLD)
    {
        std::stringstream temp_filename;
        temp_filename << "/tmp/webserv_body_" << clientFd << "_" << std::time(0);

        std::ofstream temp_file(temp_filename.str().c_str(), std::ios::binary);
        if (temp_file.is_open())
        {
            const std::vector<unsigned char>& body_vec = request->getBody();
            temp_file.write(reinterpret_cast<const char*>(body_vec.data()), body_vec.size());
            temp_file.close();
            chmod(temp_filename.str().c_str(), 0644);

            request->setBodyFilePath(temp_filename.str());
            request->clearBody();
        }
        else
        {
            std::cerr << "Error: Could not open temporary file for body." << std::endl;
        }
    }

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
			cgi->executeCgi(this->_CgiInputFds, this->_CgiOutputFds, clientFd);
			_requestBuffers[clientFd].clear();
			_clientSendOffset.erase(clientFd);
			_epollManager.modifyFd(clientFd, EPOLLIN);
			//delete request;
			return true;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Failed cgi: " << e.what() << "\n";
		_requestBuffers[clientFd].clear();
		_clientSendOffset.erase(clientFd);
		_epollManager.modifyFd(clientFd, EPOLLIN);
		//delete request;
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
	std::string headerStr = oss.str();
	_responseBuffer.insert(_responseBuffer.end(), headerStr.begin(), headerStr.end());

	if (response.bodyIsBinary())
	{
		const std::vector<unsigned char>& body = response.getBodyBinary();
		_responseBuffer.insert(_responseBuffer.end(), body.begin(), body.end());
	}
	else
	{
		std::string bodyStr = response.getBody();
		_responseBuffer.insert(_responseBuffer.end(), bodyStr.begin(), bodyStr.end());
	}
}

void EventLoop::sendResponse(int clientFd)
{
	size_t &totalSent = _clientSendOffset[clientFd];
	size_t bufferSize = _responseBuffer.size();

	while (totalSent < bufferSize)
	{	
		size_t chunkSize = std::min(static_cast<size_t>(4096), bufferSize - totalSent);
		//std::cout<<RED<<"REsponse: "<< _responseBuffer.data()<<RESET<<std::endl;
		ssize_t sent = send(clientFd, _responseBuffer.data() + totalSent, chunkSize, 0);

		if (sent == -1)
		{
			perror("send");
			close(clientFd);
			_requestBuffers.erase(clientFd);
			_clientToServer.erase(clientFd);
			_clientSendOffset.erase(clientFd);
			try
			{
				_epollManager.removeFd(clientFd);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Failed to remove fd: " << e.what() << std::endl;
			}
			return;
		}
		totalSent += sent;
		if (static_cast<size_t>(sent) < chunkSize)
        {
			_epollManager.modifyFd(clientFd, EPOLLOUT);
			return;
		}
	}

	_requestBuffers[clientFd].clear();
	_clientSendOffset.erase(clientFd);
	_epollManager.modifyFd(clientFd, EPOLLIN);
	//std::cout <<BLUE<< "Response sent, connection kept alive" <<RESET<< std::endl;
}


void EventLoop::handleClientData(int clientFd)
{
	char buffer[4096];
	ssize_t count = recv(clientFd, buffer, sizeof(buffer), 0);

	if (count <= 0)
	{
		if (count < 0)
			perror("recv");
		close(clientFd);
		_requestBuffers.erase(clientFd);
		_clientToServer.erase(clientFd);
		_clientSendOffset.erase(clientFd);
		try
		{
			this->_epollManager.removeFd(clientFd);
		}
		catch (const std::exception &e)
		{
			std::cerr << "Failed to remove client fd from epoll: " << e.what() << std::endl;
		}
		return;
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
			//std::cout << RED << "Received HTTP request from fd " << clientFd << RESET << ":\n";
			if (request == NULL)
				return;
			//request->HttpRequestPrinter();
			if (handleCgiIfNeeded(request, clientFd))
				return;

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

		reqMan.parseHttpRequest(_buffers[clientFd], this->getServersByFd(clientFd));
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
		reqMan.parseHttpRequest(full_request, this->getServersByFd(clientFd));
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
