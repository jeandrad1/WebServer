#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP

# include "EpollManager.hpp"
# include "SocketsManager.hpp"

class EventLoop {

	private:

		std::map<Socket *, int>		_serverSockets;
		EpollManager				&_epollManager;
		std::map<int, std::string>	_buffers;

		bool isServerSocket(int fd);
		void handleNewConnection(int serverFd);
		void handleClientData(int clientFd);
		void handleHttpRequest(int clientFd, size_t header_end);

	public:

		EventLoop(EpollManager &epollManager, std::map<Socket *, int> serverSockets);
		~EventLoop(void);

		void runEventLoop(void);
};

# endif