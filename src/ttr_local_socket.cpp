#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "ttr_local_socket.h"

cttr_local_socket::cttr_local_socket(int type) : m_type(type)
{
    memset(m_local, 0x00, 255);
    m_socket = -1;
    memset(&m_addr, 0x00, sizeof(struct sockaddr_un));
}

cttr_local_socket::cttr_local_socket(int sock, struct sockaddr_un *addr, const char *ls): m_socket(sock)
{
    memcpy(&m_addr, addr, sizeof(struct sockaddr_un));
    memcpy(m_local, ls, strlen(ls));
}

cttr_local_socket::~cttr_local_socket()
{
}

int cttr_local_socket::startup(const char *ls)
{
    if(!ls) {
        printf("argument error. ls = NULL\n");
        return -1;
    }

    if(m_type == TTR_LOCAL_SERVER) {
        return startup_server(ls);
    } else {
        return startup_client(ls);
    }
    return 0;
}

cttr_local_socket *cttr_local_socket::accept_client()
{
    socklen_t len = 0;
    struct sockaddr_un client;
    int sock = 0;

    len = sizeof(client);

    sock = accept(m_socket, (struct sockaddr *)&client, &len);
    if(sock == -1) {
        perror("accept error.");
        return NULL;
    }
    
    return new cttr_local_socket(sock, &client, m_local);
}

int cttr_local_socket::connect_server()
{
    int ret = 0;

    if(m_socket == -1) {
        printf("socket not create.\n");
        return -1;
    }

    ret = connect(m_socket, (struct sockaddr *)&m_addr, sizeof(m_addr));
    if(ret == -1) {
        perror("connect error.");
        return -1;
    }
    
    return 0;
}

int cttr_local_socket::request(char *buf, int len)
{
    int ret = 0;

    ret = read(m_socket, buf, len);
    if(ret == -1) {
        perror("read error.");
        return -1;
    }
    
    return ret;
}

int cttr_local_socket::response(char *buf, int len)
{
    int ret = 0;

    ret = write(m_socket, buf, len);
    if(ret == -1) {
        perror("write error.");
        return -1;
    }
    return ret;
}

int cttr_local_socket::startup_server(const char *ls)
{
    int ret = 0;
    int on = 1;
    
    unlink(ls);
    bzero(&m_addr, sizeof(struct sockaddr_un));

    m_addr.sun_family = AF_UNIX;
    strncpy(m_addr.sun_path, ls, sizeof(m_addr.sun_path));

    memcpy(m_local, ls, strlen(ls));
    m_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if(m_socket == -1) {
        perror("socket error.");
        return -1;
    }

    ret = setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
    if(ret < 0) {
        perror("setsockopt error.");
        return -1;
    }

    ret = bind(m_socket, (struct sockaddr *)&m_addr, sizeof(m_addr));
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

int cttr_local_socket::startup_client(const char *ls)
{
    bzero(&m_addr, sizeof(m_addr));

    m_addr.sun_family = AF_UNIX;
    strncpy(m_addr.sun_path, ls, sizeof(m_addr.sun_path));

    memcpy(m_local, ls, strlen(ls));
    m_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if(m_socket == -1) {
        perror("create socket error.");
        return -1;
    }
    
    return 0;
}
