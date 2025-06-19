#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP

# include "EpollManager.hpp"
# include "SocketsManager.hpp"

class EventLoop {

	private:

		std::map<Socket *, int>		_serverSockets;
		EpollManager				&_epollManager;
		std::map<int, std::string>	_buffers;
        std::map<int, std::vector<ServerConfig *> >	_servers;

		bool isServerSocket(int fd);
		void handleNewConnection(int serverFd);
		void handleClientData(int clientFd);
		void handleHttpRequest(int clientFd, size_t header_end);

	public:

		EventLoop(EpollManager &epollManager, std::map<Socket *, int> serverSockets, std::map<int, std::vector<ServerConfig *> > servers);
		~EventLoop(void);

		void runEventLoop(void);
};

# endif