#ifndef __UDP_H
#define __UDP_H

#include <netinet/in.h>

typedef struct sock {
    int     sockfd;
    char    *ip;
    int     port; 
}sock_t;



// typedef struct s_info {
//     int  fd;
//     struct sockaddr_in addr;
//     sock_t sock;
// }s_info_t;


int udp_connect(sock_t sock);
void* udp_revice_data(void *s_info);
void* udp_send_data(void *s_info);
void get_time_stamp(char* timer);
#endif 