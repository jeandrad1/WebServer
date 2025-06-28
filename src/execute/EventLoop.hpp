#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP

# include "EpollManager.hpp"
# include "SocketsManager.hpp"
# include "CgiDetector.hpp"
# include "CgiHandler.hpp"
# include "ServerUtils.hpp"

class EventLoop {

	private:

		std::map<Socket *, int>		_serverSockets;
		EpollManager				&_epollManager;
		std::map<int, std::string>	_buffers;
		std::map<int, std::vector<ServerConfig *> > _servers;
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