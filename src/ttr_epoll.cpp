#include <sys/epoll.h>

#include "ttr_epoll.h"

cttr_epoll::cttr_epool(int size, int timeout), 
    m_size(size),
    m_timeout(timeout)
{
    if(size == 0) {
        m_fd = epoll_create1(0);
    } else {
        m_fd = epoll_create(size);
    }

    if(m_fd == -1) {
        perror("epoll create error.");
        abort();
    }
}

cttr_epoll::~cttr_epoll()
{
    close(m_fd); m_fd = -1;
}

int cttr_epoll::epoll_event_add()
{
    return 0;
}

int cttr_epoll::epoll_event_delete()
{
    return 0;
}

int cttr_epoll::epoll_event_wait()
{
    return 0;
}

