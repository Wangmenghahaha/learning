#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "taos.h"


static void queryDB(TAOS *taos, char *command);
void Test(TAOS *taos, char *qstr, int i);


int main(int argc, char *argv[])
{
    char qstr[1024];

//    if (argc < 2) {
//        printf("请输入服务器ip \n");
//    }

    TAOS *taos = taos_connect("localhost:6030", "root", "taosdata", NULL, 0);
    if (taos == NULL) {
        printf("数据库连接失败，原因为：%s \n", taos_errstr(NULL));
        exit(1);
    }
    
    for (int i = 0; i < 100; i++) {
        Test(taos, qstr, i);
    }

    taos_close(taos);
    taos_cleanup();
    return 0;
}


static void queryDB(TAOS *taos, char *command)
{
    int i;
    TAOS_RES *res = NULL;
    int code = -1;

    for (i=0; i<5; i++) {
        if (res != NULL) {
            taos_free_result(res);
            res = NULL;
        }

        res = taos_query(taos, command);
        code = taos_errno(res);
        if (code == 0) {
            break;
        }
    }

    if (code != 0) {
        fprintf(stderr, "Failed to run %s, reason:%s \n", command, taos_errstr(res));
        taos_free_result(res);
        taos_close(taos);
        exit(EXIT_FAILURE);
    }

    taos_free_result(res);
}

void Test(TAOS *taos, char *qstr, int index)
{
    printf("======================test at %d \n==================", index);

    queryDB(taos, "drop database if exists demo");
    queryDB(taos, "create database demo");
    queryDB(taos, "use demo");

    queryDB(taos, "create table m1 (ts timestamp, ti tinyint, si smallint, i int, bi bigint, f float, d double, b binary(10))"); 
    printf("success to create table\n");

    TAOS_RES *result;
    
    int i = 0;
    for (i=0; i<10; i++) {
        sprintf(qstr, "insert into m1 values (%" PRId64 ", %d, %d, %d, %d, %f, %lf, '%s')", (uint64_t)(1546300800000 + i * 1000), i, i, i, i*10000000, i*1.0, i*2.0, "hello");
        printf("qstr:%s\n", qstr);

        TAOS_RES *result1 = taos_query(taos, qstr);
        if (result == NULL || taos_errno(result1) != 0) {
            printf("failed to insert row, reason:%s\n", taos_errstr(result1));
            taos_free_result(result1);
            exit(1);
        } else {
            printf("insert row: %i\n", i);
        }
        taos_free_result(result1);
    }
    printf("success to insert rows, total %d rows\n", i);

    sprintf(qstr, "select * from m1");
    result = taos_query(taos, qstr);
    
    if (result == NULL || taos_errno(result) != 0) {
        printf("failed to select, reason:%s\n", taos_errstr(result));
        taos_free_result(result);
        exit(1);
    }

    TAOS_ROW    row;
    int         rows = 0;
    int         num_fields = taos_field_count(result);
    TAOS_FIELD  *fields = taos_fetch_fields(result);

    printf("num_fields = %d\n", num_fields);
    printf("select * from table, result:\n");

    while (row = taos_fetch_row(result)) {
        char temp[1024];
        rows ++;
        taos_print_row(temp, row, fields, num_fields);
        printf("%s\n", temp);
    }

    taos_free_result(result);
    printf("======demo end= ======\n\n");
    
}

