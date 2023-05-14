#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

typedef struct sockaddr* SP;


int main(int argc, char* argv[])
{

    /* 创建socket连接 */
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sockfd) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in srv_addr = {}, cli_addr = {};    //地址结构体
    srv_addr.sin_family = AF_INET;  //地址族，和socket中的一至
    srv_addr.sin_port = htons(8080);    //端口号
    srv_addr.sin_addr.s_addr = inet_addr("192.168.1.15");  //本机ip地址
    socklen_t addrlen = sizeof(srv_addr);   //地址结构体长度

    cli_addr.sin_addr.s_addr = inet_addr("192.168.1.14");
    cli_addr.sin_port = htons(8080);
    cli_addr.sin_family = AF_INET;

    if (bind(sockfd,(SP)&srv_addr, addrlen)) {
        perror("bind");
        return EXIT_FAILURE;
    }


    char rece_buf[4096] = {};
    char send_buf[10] = {0,1,2,3,4,5,6,7,8,9};
    //char *send_buf = "udp test";

    while(1) {
        int ret = 0;
 
        ret = recvfrom(sockfd, rece_buf, sizeof(rece_buf), 0, (SP)&cli_addr, &addrlen);  //接收数据
        if (0 > ret) {
            printf("网络异常，通信结束！\n");
            close(sockfd);
            return EXIT_FAILURE;
        }
       
        printf("from %s recv:[%s] bits:%d\n",inet_ntoa(cli_addr.sin_addr), rece_buf, ret);

        ret = sendto(sockfd, send_buf, sizeof(send_buf), 0, (SP)&cli_addr, addrlen);
        printf("ret = %d\n",ret);
        if (0 >= ret) {
            printf("对方网络异常！\n");
            return EXIT_FAILURE;
        }

        /* 
        //循环发送
        ret = sendto(sockfd, send_buf, sizeof(send_buf), 0, (SP)&cli_addr, addrlen);
        printf("ret = %d\n",ret);
        if (0 >= ret) {
            printf("对方网络异常！\n");
            return EXIT_FAILURE;
        }
        sleep(1);
        */
    }

    return EXIT_SUCCESS;
}


