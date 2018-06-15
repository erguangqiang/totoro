#ifndef TTR_EPOLL_EVENT_H
#define TTR_EPOLL_EVENT_H

class cttr_epoll_event {
public:
    cttr_epoll_event(int fd);
    ~cttr_epoll_event();

private:
    int m_fd;
    int m_timeout;
};

#endif//TTR_EPOLL_EVENT_H