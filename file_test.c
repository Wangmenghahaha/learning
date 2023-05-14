#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

char *write_buf = "test";
char *write_buf1 = "读写文件测试 鹿柴 王维 空山不见人，但闻人语声。 返景入深林，复照青台上。";


int main(int argc, char *argv[])
{
    FILE *fp;
    FILE *fp1;
    size_t len;
    char str[1024];

    printf("file test\n");
    
    fp = fopen("../file_test.txt", "w+");   
    if (fp == NULL) {
        perror("file open failed");
        return EXIT_FAILURE;
    }

    // len = fputs(write_buf1, fp);
    len = fwrite(write_buf,  sizeof(write_buf), 1, fp);
    if (0 > len) {      
        perror("write fialed");
    }
   
    fclose(fp);

    fp1 = fopen("../file_test.txt", "r+"); 
    while (fgets(str, 1024, fp1) != NULL) {
        printf("%s\n", str);
    }

    fclose(fp);
}
