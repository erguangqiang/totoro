
#ifndef TTR_LOCAL_SOCKET_H
#define TTR_LOACL_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

enum {
    TTR_LOCAL_SERVER = 0,
    TTR_LOCAL_CLIENT,
};

class cttr_local_socket {
public:
    cttr_local_socket(int type);
    ~cttr_local_socket();

protected:
    cttr_local_socket(int sock, struct sockaddr_un *addr, const char *ls);

public:
    int startup(const char *ls);
    cttr_local_socket *accept_client();
    int connect_server();

    int request(char *buf, int len);
    int response(char *buf, int len);

private:
    int startup_server(const char *ls);
    int startup_client(const char *ls);
    
private:
    int m_type;// TTR_LOCAL_SERVER or TTR_LOCAL_CLIENT
    
    char m_local[255];
    int m_socket;

    struct sockaddr_un m_addr;
};

#endif//TTR_LOCAL_SOCKET_H
