#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#include "file_wr.h"
#include "udp.h"


#define FAIL_PATH       "../log/Log_"       /* 文件路径 */
#define FILE_SIZE       1024 //(10*1024*1024)

char filename[64];
char sendname[64];

char *filelist[6];
char receive_ptr = 0;
char send_ptr = 0;

char send_flag = 0;         //发送文件标志


/* 
 * @brief : udp连接
 * @param - sock：本机ip和端口号
 * @ruturn : 成功：sockfd      失败：-1
 */
int udp_connect(sock_t sock)
{
    int ret = 0;
    struct sockaddr_in srv_addr;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sockfd) {
        perror("socket error");
        return -1;
    }

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr(sock.ip);
    srv_addr.sin_port = htons(sock.port);

    ret = bind(sockfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if (0 > ret) {
        perror("bind error");
        return -1;
    }

    sock.sockfd =sockfd;
    return sockfd;
}

/* 
 * @brief : udp接收数据
 * @param - sock：发送端ip和端口号
 * @param - buffer：接收数据数组
 * @ruturn : 成功：0      失败：-1
 */
// int udp_revice_data(int sockfd, sock_t sock, char* buffer)
void* udp_revice_data(void *sock_info)
{
    printf("udp_revice_data\n");
    int ret = 0;
    int file_size = 0;
    struct sockaddr_in addr;
    sock_t *sock = (sock_t *)sock_info;
    char buffer[1024];
    char time_stamp[32];

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(sock->ip);
    addr.sin_port = htons(sock->port);
    socklen_t addr_len = sizeof(addr);

    get_time_stamp(time_stamp); /* 获取时间戳 */
    strcat(filename, FAIL_PATH);
    strcat(filename,time_stamp); /* 拼接字符串，获得文件名 */

    while (1) {
        ret = recvfrom(sock->sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&(addr), &addr_len);  /* 阻塞接收 */
        if (0 > ret) {
            perror("read data failed");
            close(sock->sockfd);
            return (void*)-1;
        } else if (0 == ret) {
            printf("close connect");
            return (void*)-1;
        }
        printf("filename %s\n", filename);
        //将数据写入文件
        ret = write_file(filename, buffer, ret);    //不能写入sizeof(buffer)个数据，要写入实际接收到的数据个数，不然有乱码
        if (0 > ret) {
            perror("file write failed, close connect");
            break;  
        } 
        file_size += ret;
        /* 文件大于10M 更新文件名 */
        if (FILE_SIZE <= file_size) {
            printf("更换文件名\n");
            // receive_ptr = add_file_to_filelist(filename, receive_ptr);
            strcpy(sendname,filename);    /* 先将要发送的文件名赋值 */
            file_size = 0;
            memset(filename, 0, sizeof(filename));
            strcat(filename, FAIL_PATH);
            get_time_stamp(time_stamp); /* 获取时间戳 */
            strcat(filename,time_stamp); /* 拼接字符串，获得文件名 */
            send_flag = 1;
        }
    }
    
    close(sock->sockfd);
}

/* 
 * @brief : udp接收数据
 * @param - sock：接收端ip和端口号
 * @param - buffer：要发送的数据
 * @ruturn : 成功：0      失败：-1
 */
// int udp_send_data(int sockfd, sock_t sock, char *buffer)
void* udp_send_data(void *sock_info)
{
    int ret = 0;
    struct sockaddr_in addr;
    sock_t *sock = (sock_t *)sock_info;
    socklen_t addrlen = sizeof(addr);
    char buffer[1024];

    FILE *fp;
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(sock->ip);
    addr.sin_port = htons(sock->port);

    while (1) {
        // while ()
        if (send_flag == 1) {
            /* 打开文件 */
            fp = fopen(sendname, "r+"); 
            if (fp == NULL) {
                perror("open file failed");
                return (void *)-1;
            }
            
            while (!feof(fp)) {  /* 判断文件是否读完 */
                ret = fread(buffer, 1, 1024, fp); 
                if (0 > ret) {
                    fclose(fp);
                    return (void *)-1;
                }
                /* 发送数据 */
                ret = sendto(sock->sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&(addr), addrlen);
                if (0 >= ret) {
                    perror("send data failed");
                    close(sock->sockfd);
                }
            }
            // close(sock->sockfd);   /* 不能关闭连接，关闭连接后会出现 read data failed: Bad file descriptor 错误 */
            send_flag = 0;  //清除发送标志位

            /* 下面这个会一直从头开始读文件，所以他不会跳出while循环 */
            // while (read_file(filename, buffer, 1024)) {
            //     ret = sendto(sock->sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&(addr), addrlen);
            //     if (0 >= ret) {
            //         perror("send data failed");
            //         close(sock->sockfd);
            //     }
            // }
        }
        
    }
    close(sock->sockfd);
}

/* 
 * @brief : 获取时间戳
 * @param - timer：存放时间戳字符串的数组，
 * @ruturn :
 */
void get_time_stamp(char* timer)
{

    time_t t;
    struct tm *p;

    time(&t);
    p = localtime(&t);

    sprintf(timer, "%d%02d%02d%02d%02d%02d", 
                    p->tm_year + 1900, p->tm_mon + 1, p->tm_mday,
                    p->tm_hour, p->tm_min, p->tm_sec);
}


char add_file_to_filelist(char *filename, char index)
{
    filelist[index] = filename;
    index++;
    if (index >= 6) {
        index = 0;
    }

    return index;


    // int temp_num = receive_ptr;    /* */

    // while (filelist[receive_ptr] == NULL) {
    //     filelist[receive_ptr] = filename;
    //     receive_ptr++;
    //     if (temp_num == receive_ptr) {
    //         printf("filelist is full\n");
    //         break;
    //     }
    //     if (receive_ptr >= 6) {
    //         receive_ptr = 0;
    //     }
    // }
}

char delete_file_from_filelist(char index)
{
    if (filelist[index] != NULL) {
        filelist[index] = NULL;
    }
    index++;
    if (index >= 6) {
        index = 0;
    }

    return index;
}