#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP

# include "EpollManager.hpp"
# include "SocketsManager.hpp"
# include "CgiHandler.hpp"
# include "ServerUtils.hpp"
# include "HttpResponse.hpp"

class EventLoop {

	private:

		EpollManager								&_epollManager;
		std::map<Socket *, int>						_serverSockets;
		std::map<int, std::vector<ServerConfig *> >	_servers;
		std::map<int, std::string>					_buffers;
		std::map<int, int>							_clientToServer;
		std::map<int, int>							_clientToServerSocket;
		std::map<int, std::string>					_ClientIPs;
		std::map<int, CgiHandler *>					_CgiInputFds;
		std::map<int, CgiHandler *> 				_CgiOutputFds;
		std::map<int, std::vector<char> >			_requestBuffers;
		std::vector<char> 							_responseBuffer;
		std::map<int, size_t> 						_clientSendOffset;

		bool isServerSocket(int fd);
		bool isCgiOutputPipe(int fd);
		bool isCgiInputPipe(int fd);
		void handleNewConnection(int serverFd);
		void handleClientData(int clientFd);
		void handleCgiOutput(int fd);
		void handleCgiInput(int fd);
		HttpRequest *handleHttpRequest(int clientFd, size_t header_end);
		std::vector<ServerConfig *> getServersByFd(int fd);
		
		// Aux for handleClientData
		HttpRequest* parseRequestFromBuffer(int clientFd, size_t header_end);
		bool handleCgiIfNeeded(HttpRequest* request, int clientFd);
		void prepareResponseBuffer(const HttpResponse& response);
		void sendResponse(int clientFd);


	public:

		EventLoop(EpollManager &epollManager, std::map<Socket *, int> serverSockets, std::map<int, std::vector<ServerConfig *> > servers);
		~EventLoop(void);


		void runEventLoop(void);

		void printCgiOutputFds() const;
		
};

# endif