#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


/* 
 * @brief ：读文件数据
 * @param - filename：文件名
 * @param - read_buf：保存读到的数据
 * @param - len ：读取的数据长度
 * @return ：成功：读到的字节数   失败：-1
 */
int read_file(const char *filename, char *read_buf, int len)
{
    FILE *fp;
    int ret = 0;
    
    fp = fopen(filename, "r+"); 
    if (fp == NULL) {
        perror("open file failed");
        return -1;
    }

    ret = fread(read_buf, 1, len, fp); 
    if (0 > ret) {
        fclose(fp);
        return -1;
    }

    fclose(fp);

    return ret;
}

/* 
 * @brief : 向文件中以追加的方式写入数据
 * @param - filename：文件名
 * @param - write_buf：写入的数据
 * @param - len：写入的长度
 * @ruturn : 成功：写入的字节数      失败：-1
 */
int write_file(const char *filename, char *write_buf, int len)
{
    FILE *fp;
    int ret = 0;

    fp = fopen(filename, "a+"); /* 追加的方式写入数据 */
    if (fp == NULL) {
        perror("open file failed ");
        return -1;
    }

    ret = fwrite(write_buf, 1, len, fp);
    if (0 > ret) {
        fclose(fp);
        return -1;
    }

    fclose(fp);

    return ret;
}