#include <string.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "pthread.h"


struct msgbuf {
    long mtype;
    char buf[100];
};


void *send_thread(void *argv)
{
    struct msgbuf sendbuf;
    int msgid = (int)argv;
    int flag;

    sendbuf.mtype = 1;
    strcpy(sendbuf.buf, "Hello World!");
    int sendlen = sizeof(sendbuf.buf);

    while (1) {
        flag = msgsnd(msgid, &sendbuf, sendlen, 0);
        if (flag < 0) {
            printf("send message error\n");
        }

        sleep(2);
    }

}

void *recv_thread(void *argv)
{
    struct msgbuf recvbuf;
    int msgid = (int)argv;
    int flag;

    recvbuf.mtype = 1;
    memset(&recvbuf, 0, sizeof(recvbuf));
    int recvlen = sizeof(recvbuf.buf);

    while (1) {
        flag = msgrcv(msgid, &recvbuf, recvlen, 0, 0);
        if (flag < 0) {
            printf("recv message error\n");
        }

        printf("message: %s\n",recvbuf.buf);

    }
}





int main(int argc, char *argv[])
{
    int ret;
    int msgid;
    pthread_t send_id, recv_id;

    msgid = msgget(IPC_PRIVATE, IPC_CREAT| 0666);
    if (msgid < 0) {
        printf("create queue failed\n");
        return -1;
    }

    ret = pthread_create(&send_id, NULL, send_thread, msgid);
    if (ret < 0) {
        printf("create send thread failed\n");
        return -1;
    }

    ret = pthread_create(&recv_id, NULL, recv_thread, msgid);
    if (ret < 0) {
        printf("create recv thread failed\n");
        return -1;
    }

    pthread_join(send_id, NULL);
    pthread_join(recv_id, NULL);


    return 0;
}