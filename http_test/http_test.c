#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#if 1
int main(int argc, char *argv[])
{
    int ret;
    int servefd = 0, clientfd = 0;              /* 服务端和客户端fd */
    struct sockaddr_in srv_addr, cli_addr;  /* 服务端和客户端socket地址 */
    socklen_t cli_len;
    char revice_buf[1024];      /* 接收数据 */
    char filePath[1024];    /* 发送数据 */
    int revice_len = sizeof(revice_buf);    /* 接收长度 */

    /* 创建socket  */
    servefd = socket(AF_INET, SOCK_STREAM, 0);
    if (servefd < 0) {
        perror("create sock failed");
        return -1;
    }

    /* 准备服务端地址信息 */
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr("192.168.122.139");
    srv_addr.sin_port = htons(8080);

    /* 绑定本机ip */
    ret = bind(servefd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if (ret < 0) {
        perror("bind failed");
        close(servefd);
        return -1;
    }

    /* 监听客户端连接 */
    ret = listen(servefd, 5);
    if (ret < 0) {
        perror("listen failed");
        close(servefd);
        return -1;
    }

    while (1) {
        /* 接收客户端连接 */
        clientfd = accept(servefd, (struct sockaddr* )&cli_addr, &cli_len);
        if (clientfd < 0) {
            perror("accept failed");
            close(servefd);
            return -1;
        }

        /* 接收数据 */
        memset(revice_buf, 0, revice_len);          /* 清空接收区 */
        ret = recv(clientfd, revice_buf, sizeof(revice_buf), 0);
        if (ret <= 0) {
            perror("revive failed");
            close(servefd);
            return -1;
        }
        

        strcpy(filePath, "../index.html");      /* 网页路径 */
 
		ret = access(filePath, 0);//0 代表判断文件是否存在  如果存在返回0 否则返回-1
		if (ret != 0) {
			//未找到文件
			char sendBuf[1024] = { 0 };
 
			sprintf(sendBuf, "HTTP/1.1 404 NOT FOUND\r\n");
			send(clientfd, sendBuf, strlen(sendBuf), 0);
 
			sprintf(sendBuf, "Content-type:text/html\r\n");
			send(clientfd, sendBuf, strlen(sendBuf), 0);
 
			sprintf(sendBuf, "\r\n");
			send(clientfd, sendBuf, strlen(sendBuf), 0);
 
 
			sprintf(sendBuf, "not find hahahha\r\n");
			send(clientfd, sendBuf, strlen(sendBuf), 0);
		} else {
            //找到相关网页文件
			FILE* fs = fopen(filePath, "r");
			if (fs == NULL) {
				printf("打开网页文件失败\r\n");
				return -1;
			} else {
				char dataBuf[1024] = { 0 };
 
				sprintf(dataBuf, "http/1.1 200 success\r\n");
				send(clientfd, dataBuf, strlen(dataBuf), 0);
 
				sprintf(dataBuf, "content-type:text/html\r\n");
				send(clientfd, dataBuf, strlen(dataBuf), 0);
 
				sprintf(dataBuf, "\r\n");
				send(clientfd, dataBuf, strlen(dataBuf), 0);
 
				/* 发送网页内容 */
                do {
					fgets(dataBuf, 1024, fs);
					send(clientfd, dataBuf, strlen(dataBuf), 0);
				} while (!feof(fs));
 
				fclose(fs);
			}
		}
    }

    close(servefd);
    close(clientfd);
    return 0;
}

#endif





