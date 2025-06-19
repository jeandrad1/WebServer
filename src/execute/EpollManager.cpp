#include "EpollManager.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

EpollManager::EpollManager(void)
{
    this->_epollFd = epoll_create(5000);
	if (this->_epollFd == -1)
	{
		throw std::runtime_error("Failed to create epoll instance");
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
		throw std::runtime_error("epoll_ctl(ADD) failed");
	}
}

void EpollManager::modidyFd(int serverFd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = serverFd;

	if (epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, serverFd, &ev) == -1)
	{
		throw std::runtime_error("epoll_ctl(MOD) failed");
	}
}

void EpollManager::removeFd(int serverFd)
{
	if (epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, serverFd, NULL) == -1)
	{
		throw std::runtime_error("epoll_ctl failed at removeFd");
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