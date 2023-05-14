#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>


/* 线程1函数 */
void* task_1_thread(void *arg)
{
    while (1) {
        printf("this is thread 1 \n");
        sleep(1);
    }
}

/* 线程2函数 */
void* task_2_thread(void *arg)
{
    while (1) {
        printf("this is thread 2 \n");
        sleep(2);
    }
}

int main(int argc, char *argv[])
{
    pthread_t thread_id1, thread_id2;
    int ret = 0;

    /* 创建线程1 */
    ret = pthread_create(&thread_id1, NULL, task_1_thread, NULL);
    if (ret) {
        printf("thread 1 create failed \n");
        return 1;
    } 

    /* 创建线程2 */
    ret = pthread_create(&thread_id2, NULL, task_2_thread, NULL);
    if (ret) {
        printf("thread 2 create failed \n");
        return 1;
    }

    /* 等待线程1、2退出后在退出 main() 函数 */
    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);

    return 0;
}
