
#ifndef TTR_LOCAL_SOCKET_H
#define TTR_LOACL_SOCKET_H

typedef enum {
    TTR_LOCAL_SERVER = 0,
    TTR_LOCAL_CLIENT,
};

class cttr_local_socket {
protected:
    cttr_local_socket(int type);
    ~cttr_local_socket();

public:
    static cttr_local_socket *instance(int type);

    int startup(const char *ls);
    int accept();
    int connect();
    
private:
    static cttr_local_socket *m_instance;
    int m_type;// TTR_LOCAL_SERVER or TTR_LOCAL_CLIENT
    
    char m_local[255];
    int m_socket;

    struct sockadd_un m_server;
    struct sockadd_un m_client;
};

#endif//TTR_LOCAL_SOCKET_H
