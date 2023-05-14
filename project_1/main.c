#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "udp.h"


#define SERVE_IP        "192.168.1.14"
#define SERVE_PORT      8080

#define CLIENT_IP       "192.168.1.14"
#define CLIENT_PORT     8080

#define LOCAL_IP        "192.168.1.15"
#define LOCAL_PORT      8080


int main(int argc, char *argv[])
{
    int ret;
    pthread_t thread_send, thread_recv;
    sock_t serve, client, local;
    
    serve.ip = SERVE_IP;
    serve.port = SERVE_PORT;

    client.ip = CLIENT_IP;
    client.port = CLIENT_PORT;

    local.ip = LOCAL_IP;
    local.port = LOCAL_PORT;

    /* 创建一个连接即可 */
    ret = udp_connect(local);
    if (0 > ret) {
        printf("serve connetc failed\n");
        return -1;
    }
    client.sockfd = ret;
    serve.sockfd = ret;

    ret  = pthread_create(&thread_recv, NULL, udp_revice_data, (void *)&client);
    if (ret) {
        printf("thread_recv create failed \n");
        return -1;
    } 

    ret = pthread_create(&thread_send, NULL, udp_send_data, (void *)&serve);
    if (ret) {
        printf("thread_send create failed \n");
        return -1;
    }

    pthread_join(thread_recv, NULL);
    pthread_join(thread_send, NULL);

    return  0;
}