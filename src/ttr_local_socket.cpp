#include <sys/socket.h>
#include <sys/types.h>

#include "ttr_local_socket.h"

cttr_local_socket *cttr_local_socket::m_instance = NULL;

cttr_local_socket::cttr_local_socket(int type), m_type(type)
{
}

cttr_local_socket::~cttr_local_socket()
{
    delete m_instance;
    m_instance = NULL;
}

cttr_local_socket *cttr_local_socket::instance(int type)
{
    if(!m_instance) {
        m_instance = new cttr_local_socket(type);
    }

    return m_instance;
}

int cttr_local_socket::startup(const char *ls)
{
    int ret = 0;

    if(!ls) {
        printf("argument error. ls = NULL\n");
        return -1;
    }

    unlink(ls);
    bzero(&m_server, sizeof(struct sockadd_un));

    m_server.sun_family = AF_UNIX;
    strncpy(m_server.sun_path, ls, sizeof(m_server.sun_path));

    memcpy(m_local, ls, strlen(ls));
    m_socket = socket(AF_UNIX, SOCKET_STREAM, 0);
    if(m_socket == -1) {
        perror("socket error.");
        return -1;
    }

    ret = setsockopt(m_socket, (struct sockaddr *)&m_server, sizeof(m_server));
    if(ret < 0) {
        perror("setsockopt error.");
        return -1;
    }

    ret = bind(m_socket, (struct sockadd *)&m_server, sizeof(m_server));
    if(ret == -1) {
       perror("bind error.");
       return -1;
    }

    ret = listen(m_socket, 5);
    if(ret == -1) {
        perror("listen error.");
        return -1;
    }

    ret = fcntl(m_socket, F_GETFL, 0);
    if(ret == -1) {
        perror("fcntl error.");
        return -1;
    }

    ret |= O_NONBLOCK;
    fcntl(m_socket, F_SETFL, ret);
    return 0;
}

int cttr_local_socket::accept()
{
    int ret = 0;
    socklen_t len = 0;

    len = sizeof(m_client);
    return 0;
}

int cttr_local_socket::connect()
{
    return 0;
}

