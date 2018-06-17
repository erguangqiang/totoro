#ifndef TTR_STARTUP_H
#define TTR_STARTUP_H

#include "ttr_local_socket.h"

class cttr_startup {
protected:
    cttr_startup();
    ~cttr_startup();

public:
    static cttr_startup *instance();

    int startup_deamon();
    int startup_lockfile();

private:
    static cttr_startup *m_instance;
    cttr_local_socket *m_lock;
};

#endif//TTR_STARTUP_H
