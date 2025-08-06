#ifndef EPOLLMANAGER_HPP
# define EPOLLMANAGER_HPP

# include <vector>
# include <sys/epoll.h>
# include <unistd.h>
# include <iostream>
# include <stdio.h>

class EpollManager {

	private:
	
		int _epollFd;

	public:

		EpollManager(void);
		~EpollManager(void);

		static	EpollManager &getInstance(void);

		void	addFd(int serverFd, uint32_t events);
		void	modifyFd(int serverFd, uint32_t events);
		void	removeFd(int serverFd);

		int		waitForEvents(int maxEvents, int timeoutMs, struct epoll_event *events);

		int		getEpollFd(void);
};

#endif