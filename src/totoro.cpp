#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ttr_startup.h"

#define TTR_WORKERS     (2)

typedef struct ttr_worker {
    pid_t pid;
    int enable;
}ttr_worker_t;

enum {
    TTR_WF_SERVER = 0,
    TTR_WF_CLIENT,
};

enum {
    FALSE = 0,
    TRUE = 1,
};

static ttr_worker_t *g_workers;

int main(int argc, char** argv)
{
    int ret = 0;
    int i = 0;
    int cworker = 0;
    int status = 0;
    pid_t pid = 0;
    cttr_startup *startup = NULL;
    cttr_local_socket *lsock = NULL, *lclient = NULL;
    int workerflag = TTR_WF_SERVER;

    startup = cttr_startup::instance();
    if(!startup) return -127;

#ifdef TTR_DEBUG
    ret = startup->startup_deamon();
    if(ret != 0) return -127;
#endif

    ret = startup->startup_lockfile();
    if(ret != 0) return -127;

    for(;;) {
        if(cworker >= TTR_WORKERS) {
            for(;;) {
            lsock = new cttr_local_socket(TTR_LOCAL_SERVER);
            if(!lsock) {
                perror("new cttr_local_socket error");
                return -127;
            }

            ret = lsock->startup("/tmp/totoro.sock");
            if(ret == -1) return -1;
                lclient = lsock->accept_client();
                if(!lclient) return -127;

                ret = lclient->request((char *)&pid, sizeof(pid));
                if(ret != sizeof(pid)) return -127;

                for(i = 0; i < 2; i++) {
                    if(g_workers[i].enable) continue;

                    g_workers[i].enable = TRUE;
                    g_workers[i].pid = pid;
                }

                if(i == 2) return -127;

                workerflag = (i == 0) ? TTR_WF_SERVER : TTR_WF_CLIENT;
                ret = lclient->response((char *)&workerflag, sizeof(workerflag));
                if(ret != sizeof(workerflag)) return -127;
            }
            
            waitpid(-1, &status, 0);
            printf("one work exit, one worker restart...\n");
        }

        pid = fork();
        if(pid < 0) return -127;
        else if(pid == 0) break;

        cworker++;
    }
#if 0
    //worker process
    delete lsock; lsock = NULL;
    lsock = new cttr_local_socket(TTR_LOCAL_CLIENT);
    if(!lsock) return -127;

    for(;;) {
        ret = lsock->connect_server();
        if(ret == -1) return -127;
        break;
    }

    pid = getpid();
    ret = lsock->response((char *)&pid, sizeof(pid));
    if(ret != sizeof(pid)) return -127;

    ret = lsock->request((char *)&workerflag, sizeof(workerflag));
    if(ret != sizeof(workerflag)) return -127;
#endif

    if(workerflag == TTR_WF_SERVER) {
        for(;;){
            sleep(5);
            printf("i'm pid : %d, server process.\n", getpid());
        }
    } else {
        for(;;) {
            sleep(5);
            printf("i'm pid : %d, client process.\n", getpid());
        }
    }
    
    return 0;
}
