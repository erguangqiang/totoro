#include <stdio.h>

#include "ttr_local_socket.h"

int main(int argc, char **argv)
{
    int ret = 0;
    char buf[128] = {0};
    char *response = "i'm the server. you can shutdown.";
    cttr_local_socket *sock = NULL;
    cttr_local_socket *client = NULL;

    sock = new cttr_local_socket(TTR_LOCAL_SERVER);
    if(!sock) {
        printf("new cttr_local_socket error.\n");
        return -127;
    }

    ret = sock->startup("./demo.sock");
    if(ret == -1) {
        printf("start up socket error.\n");
        return -127;
    }

    client =  sock->accept_client();
    if(!client) {
        printf("accept client error.\n");
        return -127;
    }

    ret = client->request(buf, 128);
    if(ret == -1) {
        printf("request error.\n");
        return -127;
    }

    ret = client->response(response, strlen(response));
    if(ret == -1) {
        printf("response error.\n");
        return -127;
    }

    return 0;
}
