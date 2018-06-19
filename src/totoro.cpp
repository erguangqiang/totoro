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

int main(int argc, char** argv)
{
    int ret = 0;
    int cworker = 0;
    int status = 0;
    pid_t pid = 0;
    cttr_startup *startup = NULL;

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
            waitpid(-1, &status, 0);
            printf("one work exit, one worker restart...\n");
        }

        pid = fork();
        if(pid < 0) return -127;
        else if(pid == 0) break;

        cworker++;
    }

    for(;;){
        sleep(5);
        printf("i'm pid : %d, server process.\n", getpid());
    }

    return 0;
}
