#ifndef TTR_EPOLL_H
#define TTR_EPOLL_H

class cttr_epoll {
public:
    cttr_epool(int size, int timeout);
    ~cttr_epoll();

public:
    int epoll_event_add();
    int epoll_event_delete();
    int epoll_event_wait();
    
private:
    int m_size;
    int m_fd;
    int m_timeout;//time out value micro second
};

#endif//TTR_EPOLL_H