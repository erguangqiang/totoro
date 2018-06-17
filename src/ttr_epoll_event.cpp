#include <sys/epoll.h>

#include "ttr_epoll_event.h"

cttr_epoll_event::cttr_epoll_event(int fd) : m_fd(fd)
{
}

cttr_epoll_event::~cttr_epoll_event()
{
}

