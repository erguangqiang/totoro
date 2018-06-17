#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ttr_startup.h"

cttr_startup *cttr_startup::m_instance = NULL;

cttr_startup::cttr_startup()
{
}

cttr_startup::~cttr_startup()
{
}

cttr_startup *cttr_startup::instance()
{
    if(!m_instance) {
        m_instance = new cttr_startup();
    }
    return m_instance;
}

int cttr_startup::startup_deamon()
{
    pid_t pid = 0;
    int nullfd = 0;

    pid = fork();
    if(pid < 0) {
        perror("fork error.");
        return -1;
    } else if(pid > 0) {
        exit(0);
    }

    nullfd = open("/dev/null", O_WRONLY, 0);
    if(nullfd == -1) {
        perror("open null dev failed.");
        return -1;
    }
    dup2(nullfd, 0);
    dup2(nullfd, 1);
    dup2(nullfd, 2);

    close(nullfd);
    if(setsid() == -1) {
        perror("setsid error.");
        return -1;
    }

    umask(0);
    if(chdir("/") == -1) {
        perror("chdir error.");
        return -1;
    }
    return 0;
}

int cttr_startup::startup_lockfile()
{
    int ret = 0;
    struct flock fl;
    int fd = -1;
    char lock[16] = {0};

    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    fl.l_pid = getpid();

    fd = open("/tmp/totoro.pid", O_CREAT|O_RDWR, (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH));
    if(fd == -1) {
        perror("open /tmp/totoro.pid error.");
        return -1;
    }

    ret = fcntl(fd, F_SETLK, &fl);
    if(ret == -1) {
        if(errno == EACCES || errno == EAGAIN) {
            perror("this proccess already run in system.\n");
            return -1;
        }
    }

    if(ftruncate(fd, 0) == 1) {
        perror("ftruncate error.");
        return -1;
    }

    snprintf(lock, 16, "%ld", (long)getpid());
    if(write(fd, lock, strlen(lock) + 1) != (int)(strlen(lock) + 1)) {
        perror("write pid file error.");
        return -1;
    }
    return 0;
}

