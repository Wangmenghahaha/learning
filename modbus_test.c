/* modbus test */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <modbus/modbus-tcp.h>

#define MODBUS_TCP_SERVER_IP    "192.168.1.14"
#define MODBUS_TCP_SERVER_PORT  502

typedef struct modbus_slave_device{
   modbus_t     *ctx;
   const char   *ip;
   int          port;
   int          id;
   uint16_t     addr;
   uint16_t     nb;
   uint16_t     *read_data;
   uint16_t     *write_data;

} *modbus_info_t;


/* 创建连接 */
modbus_t *modbus_connect_slave(const char *ip, int port)
{
    int  ret;

    modbus_t *ctx =  modbus_new_tcp(ip, port);
    if (!ctx) {
        return NULL;;
        perror("modbus_new_tcp fail");
    }

    ret = modbus_connect(ctx);
    if (-1 == ret) {
        return NULL;
        perror("Connect modbus slave failed!");
    }

    return ctx;   
}


/* 关闭连接 */
void close_connect_slave(modbus_t *ctx, const char *ip)
{
    modbus_close(ctx);
    modbus_free(ctx);
    printf("Close %s connect\n", ip);
}

/* 读寄存器值*/
int mb_read_registers(modbus_t *ctx, int ID, int addr, int nb, uint16_t *buff, const char *ip)
{
    int ret = 0;
    ret = modbus_set_slave(ctx, ID);
    if (-1 == ret) {
        perror("set slave failed");
        close_connect_slave(ctx, ip);
        return -1;
    }

    ret = modbus_read_registers(ctx, addr, nb, buff);
    if (nb != ret) {
        perror("modbus read register failed");
        close_connect_slave(ctx,ip);
        return -1;
    }
    
    return ret;
}

/**
 * @brief  : 写多寄存器
 * @param  : 
 * @param  : 
 * @return : 成功: 0
 *           失败: 其它
 */
int mb_write_registers(modbus_t *ctx, int ID, unsigned int addr,  int len, const uint16_t *buf,const char *ip)
{
    int ret = 0;

    /* 设置slave */
    ret = modbus_set_slave(ctx, ID);
    if (ret == -1) {
        perror("modbus_set_slave fail");

        close_connect_slave(ctx,ip);

        return -1;
    }

    /* 写入数据 */
    ret = modbus_write_registers(ctx, addr, len, buf);
    if (ret == -1) {
        perror("modbus_write_registers fail");

        close_connect_slave(ctx,ip);

        return -1;
    }

    return ret;
}



int main(int argc, char *argv[])
{
    int ret = 0;
    modbus_t *ctx = NULL;;
    uint16_t read_data[128] = {0};
    uint16_t write_data[10] = {0,1,2,3,4,5,6,7,8,9};

    ctx = modbus_connect_slave(MODBUS_TCP_SERVER_IP,MODBUS_TCP_SERVER_PORT);
    if (!ctx) {
        return -1;
    }

    while(1) {
        ret = mb_read_registers(ctx,1,1,10,read_data,MODBUS_TCP_SERVER_IP);
        if (-1  == ret) {
            return -1;
        }
        printf("read_data: ");
        for (uint8_t i = 0; i < ret; i++) {
            printf("0x%02X ",read_data[i]);
        }
        printf("\n");

        ret = mb_write_registers(ctx,2,0,10,write_data,MODBUS_TCP_SERVER_IP);
        if (-1  == ret) {
            return -1;
        }
        for (uint8_t i = 0; i< 10; i++) {
            write_data[i] += 10;
        }

        sleep(1);
    }
    
    close_connect_slave(ctx,MODBUS_TCP_SERVER_IP);
    return 0;
}


