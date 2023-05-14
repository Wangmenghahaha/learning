#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include "cJSON.h"

int main(int argc, char* argv[])
{
    int ret = 0;
    char *buffer;
    cJSON *value = NULL;
    char *keylist[5] = {"name", "age", "phone", "address", "list"};
    int keynum = 5;
    struct stat st;
    off_t buffer_len;
    char *filename = "../wangmeng.json";


    printf("JSON Test\n");

    /* 打开文件 */
    FILE *fp = fopen(filename, "r+");
    if (fp == NULL) {
      perror("file open failed");
      return -1;
    }
    /* 读文件 */
    ret = stat(filename, &st);    //
    if (ret != 0) {
        printf("stat failed\n");
    }

    buffer_len = st.st_size + 1;    //获取文件大小
    printf("buffer_len:%d\n", buffer_len);
    buffer = malloc(buffer_len);    //神清内存
    memset(buffer, 0, buffer_len);  //申请内存
    buffer[buffer_len] = '\0';      //
    ret = fread(buffer, 1, buffer_len, fp);

    if (ret < 0) {
      perror("file read failed");
      fclose(fp);
      free(buffer);
      return -1;
    }
    /* 关闭文件 */
    fclose(fp);

    /* 将字符串数据转为JSON数据 */
    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL) {
        printf("json parse error\n");
        free(buffer);
        return -1;
    }

    /* 解析json文件内容 */
    for (int i = 0; i < keynum; i++) {
        /* 根据key值获取value值 */
        value = cJSON_GetObjectItem(root, keylist[i]);
        if (value == NULL) {
            printf("GetObject error\n");
            goto error;
        }
        /* 解析list数组 */
        // if (keylist[i] == "list") {
        if (strcmp(keylist[i], "list") == 0) {
            /* 获取数组长度 */
            int len = cJSON_GetArraySize(value);
            printf("list{\n");
            for (int i = 0; i < len; i++) {
                cJSON *listvalue = NULL;
                cJSON *subjectvalue;
                cJSON *scorevalue;
                /* 根据索引值获取数组值 */
                listvalue = cJSON_GetArrayItem(value, i);
                /* 根据key值获取value值 */
                subjectvalue = cJSON_GetObjectItem(listvalue, "subject");
                scorevalue = cJSON_GetObjectItem(listvalue, "score");
                if (subjectvalue == NULL || scorevalue == NULL) {
                    printf("GetObject error\n");
                    goto error;
                }
                /* 把数据转为字符串 */
                char *subjectdata = cJSON_Print(subjectvalue);
                char *scoredata = cJSON_Print(scorevalue);
                
                printf("  subject:%s  score:%s \n",subjectdata, scoredata);

            }
            printf("}\n");
        } else {
            /* 将数据转换成字符串输出 */
            char *data = cJSON_Print(value);
            printf("%s: %s \n", keylist[i], data);
        }
    }
#if 0
    value = cJSON_GetObjectItem(root, "list");
    if (value == NULL) {
            printf("GetObject error\n");
            return -1;
        }
    /* 获取数组对象大小 */
    int len = cJSON_GetArraySize(value);
    // printf("数组元素个数len = %d \n", len);
    printf("list{\n");
    for (int i = 0; i < len; i++) {
        cJSON *listvalue = NULL;
        cJSON *subjectvalue;
        cJSON *scorevalue;
        listvalue = cJSON_GetArrayItem(value, i);
        subjectvalue = cJSON_GetObjectItem(listvalue, "subject");
        scorevalue = cJSON_GetObjectItem(listvalue, "score");
        if (subjectvalue == NULL || scorevalue == NULL) {
            printf("GetObject error\n");
            cJSON_Delete(root);
            return -1;
        }
        char *subjectdata = cJSON_Print(subjectvalue);
        char *scoredata = cJSON_Print(scorevalue);
        
        printf("  subject:%s  score:%s \n",subjectdata, scoredata);

    }
    printf("}\n");
#endif

error:
    {
        cJSON_Delete(root);
        free(buffer);
        return -1;
    }
    
    cJSON_Delete(root);
    free(buffer);
    return 0;
}