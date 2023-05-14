#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//#define SERVICE_TEST    //服务端
#define CLIENT_TEST     //客户端

typedef struct sockaddr* SP;

/* 服务端测试 */
#ifdef SERVICE_TEST
int main(int argc, char*argv[])
{
    int ret = 0;
    int sockfd, clifd;
    struct sockaddr_in srv_addr, cli_addr;
    socklen_t cli_len = 0;
    char revice_buf[1024] = {0};
    char *send_buf = "service test";
    //char send_buf[10] = {1,2,3,4,5,6,7,8,9,10};

    /* 创建socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //TCP
    if (0 > sockfd) {
        perror("creat socket failed");
        return EXIT_FAILURE;
    }

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr("192.168.1.15");
    srv_addr.sin_port = htons(8080);

    /* 绑定 */
    ret = bind(sockfd, (SP)&srv_addr, sizeof(srv_addr));
    if (-1 == ret) {
        perror("bind failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    /* 监听 */
    ret = listen(sockfd,5);
    if(-1 == ret) {
        perror("listen failed");
        close(sockfd);
	    return EXIT_FAILURE;
    }

    /* 接收客户端连接 */
    clifd = accept(sockfd, (SP)&cli_addr, &cli_len);
    if (-1 == clifd) {
        perror("accept failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    while (1) {
        memset(revice_buf, 0, sizeof(revice_buf));
        /* 接收数据 */
    	ret = recv(clifd, revice_buf, sizeof(revice_buf),0);
        if (0 < ret) {
            printf("from %s recv:[%s] \n", inet_ntoa(cli_addr.sin_addr), revice_buf);
        } else if (0 > ret) {
            perror("recv failed");
            close(sockfd);
            close(clifd);
            return EXIT_FAILURE;
        } else {
            printf("client close\n");
            close(sockfd);
            return EXIT_FAILURE;
        }

        /* 发送数据 */
        ret = send(clifd, send_buf, strlen(send_buf) + 1, 0);
        if (-1 == ret) {
            perror("send failed");
            close(sockfd);
            close(clifd);
            return EXIT_FAILURE;
        }
    }

    /* 关闭客户端和服务端连接 */
    close(sockfd);
    close(clifd);
    return EXIT_SUCCESS;
}
#endif

/* 客户端测试 */
#ifdef CLIENT_TEST
int main(int argc, char*argv[])
{
    int ret = 0;
    int sockfd;
    struct sockaddr_in srv_addr;
    socklen_t srv_len = 0;
    char revice_buf[1024] = {0};
    char *send_buf = "client test";
    //char send_buf[10] = {1,2,3,4,5,6,7,8,9,10};

    /* 创建socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //TCP
    if (0 > sockfd) {
        perror("creat socket failed");
        return EXIT_FAILURE;
    }

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr("192.168.1.14");
    srv_addr.sin_port = htons(8080);

    /* 连接服务端 */
    ret = connect(sockfd, (SP)&srv_addr, sizeof(srv_addr));
    if (0 > ret) {
        perror("connect failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    while (1) {
        memset(revice_buf, 0, sizeof(revice_buf));
        /* 接收数据 */
    	ret = recv(sockfd, revice_buf, sizeof(revice_buf),0);
        if (0 < ret) {
            printf("from %s recv:[%s] \n", inet_ntoa(srv_addr.sin_addr), revice_buf);
        } else if (0 > ret) {
            perror("recv failed");
            close(sockfd);
            return EXIT_FAILURE;
        } else {
            printf("client close\n");
            close(sockfd);
            return EXIT_FAILURE;
        }

        /* 发送数据 */
        ret = send(sockfd, send_buf, strlen(send_buf) + 1, 0);
        if (-1 == ret) {
            perror("send failed");
            close(sockfd);
            return EXIT_FAILURE;
        }
        // sleep(1);
    }

    /* 关闭客户端和服务端连接 */
    close(sockfd);
    // close(clifd);
    return EXIT_SUCCESS;
}
#endif
