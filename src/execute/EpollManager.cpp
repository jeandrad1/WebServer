#include "EpollManager.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

EpollManager::EpollManager(void)
{
    this->_epollFd = epoll_create1(0);
	if (this->_epollFd == -1)
	{
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}
}

EpollManager::~EpollManager(void)
{
    close(this->_epollFd);
}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void EpollManager::addFd(int serverFd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = serverFd;

	if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, serverFd, &ev) == -1)
	{
		std::cout << "Error: epoll_ctl at addFd.\n";
		exit(EXIT_FAILURE);
	}
}

void EpollManager::modidyFd(int serverFd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = serverFd;

	if (epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, serverFd, &ev) == -1)
	{
		std::cout << "Error: epoll_ctl at modifyFd.\n";
		exit(EXIT_FAILURE);
	}
}

void EpollManager::removeFd(int serverFd)
{
	if (epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, serverFd, NULL) == -1)
	{
		std::cout << "Error: epoll_ctl at removeFd.\n";
		exit(EXIT_FAILURE);
	}
}

int EpollManager::waitForEvents(int maxEvents, int timeoutMs, struct epoll_event *events)
{
	int n_ready = epoll_wait(this->_epollFd, events, maxEvents, timeoutMs);
	if (n_ready == -1)
	{
		perror("epoll_wait");
		return (-1);
	}
	return (n_ready);
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/

int EpollManager::getEpollFd(void)
{
    return (this->_epollFd);
}

/***********************************************************************/
/*                          Private Functions                          */
/***********************************************************************/