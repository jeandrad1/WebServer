#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP

# include "EpollManager.hpp"
# include "SocketsManager.hpp"
# include "CgiHandler.hpp"
# include "ServerUtils.hpp"

class EventLoop {

	private:

		EpollManager				&_epollManager;
		std::map<Socket *, int>		_serverSockets;
		std::map<int, std::vector<ServerConfig *> > _servers;
		std::map<int, std::string>	_buffers;
		std::map<int, int>			_clientToServer;
		std::map<int, int> _clientToServerSocket;
		std::map<int, std::string> _ClientIPs;

		bool isServerSocket(int fd);
		void handleNewConnection(int serverFd);
		void handleClientData(int clientFd);
		HttpRequest *handleHttpRequest(int clientFd, size_t header_end);

		std::vector<ServerConfig *> getServersByFd(int fd);

	public:

		EventLoop(EpollManager &epollManager, std::map<Socket *, int> serverSockets, std::map<int, std::vector<ServerConfig *> > servers);
		~EventLoop(void);

		void runEventLoop(void);
		
};

# endif