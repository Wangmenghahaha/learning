#include <stdio.h>
#include <string.h>

#include "sqlite3.h"


sqlite3 *db;

/* 打开sqlite */
int db_init(void)
{
    int ret;
    ret = sqlite3_open("test.db", &db);
    if (ret != SQLITE_OK) {
        printf("sqlite open failed\n");
    }
    return ret;
}

/* 创建表 */
int create_table(sqlite3 *pdb) 
{
    char *sql;
    char *errmsg;
    int ret;

    /* 
     * 1、"\" 后面不能有空格，
     * 2、sql语句的最后一个不就能加","
     * if not exists
     * autoincrement //自增长
     */
    sql = "CREATE TABLE IF NOT EXISTS mytable(\
        id INTEGER PRIMARY KEY autoincrement NOT NULL,\
        name text NOT NULL,\
        age int NOT NULL,\
        phone text NOT NULL,\
        address text NOT NULL\
        );";
    // printf("sql %s\n", sql);

    // sql = "CREATE TABLE IF NOT EXISTS mytable(id INTEGER PRIMARY KEY autoincrement NOT NULL,name text NOT NULL,age int NOT NULL,phone text NOT NULL,address text NOT NULL );";
    ret = sqlite3_exec(pdb, sql, NULL, NULL, &errmsg);
    if (ret != SQLITE_OK) {
        perror("create table failed");
        return -1;
    }

    return 0;
}

int delete_table(sqlite3 *db, char *table_name)
{
    int ret;
    char sql[128];  //要给出长度值
    char *errmsg;

    sprintf(sql, "DROP TABLE %s;", table_name);
    printf("sql is %s",sql);
    ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (ret != SQLITE_OK) {
        printf("%s表删除失败",table_name);
        return -1;
    }

    printf("%s表删除成功\n",table_name);
    return 0;
}

/* 插入数据 */
int insert_data(sqlite3 *db)
{
    int ret = 0;
    char *sql;
    sqlite3_stmt *stmt;
    const char *zTail;

    int id = 4;
    char *name = "wangmeng";
    int age = 18;
    char *phone = "12348888888";
    char *adder = "zhongguo";

    sql = "insert into mytable (id, name, age, phone,address) values(?,?,?,?,?);";  //id为自增模式，需要在这个位置写入id字段
    sqlite3_prepare_v2(db, sql, -1, &stmt, &zTail);     //准备sql语句
    // sqlite3_bind_int(stmt, 1, id);       //id为自增模式，所以这个地方不用给id值

    /* 绑定数据 */
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, age);
    sqlite3_bind_text(stmt, 4, phone, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, adder, -1, SQLITE_STATIC);
    //执行
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE) {
        printf("%s\n", sqlite3_errmsg(db));
        return -1;
    }
    /* 清空内存 */
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return 0;
}

/* 查询数据 */
int  query_data(sqlite3 *db)
{
    int ret;
    sqlite3_stmt *stmt;
    const char *zTail;
    char *sql;

    int id;
    const char *name;
    int age;
    const char *phone;
    const char *adder;


    sql = "select * from mytable;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, &zTail);
    ret = sqlite3_step(stmt);
    while (ret == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);   //为啥从0开始呢
        name = sqlite3_column_text(stmt, 1);
        age = sqlite3_column_int(stmt, 2);
        phone = sqlite3_column_text(stmt, 3);
        adder = sqlite3_column_text(stmt, 4);
        printf("id %d\n", id);
        printf("name %s\n", name);
        printf("age %d\n", age);
        printf("phone %s\n", phone);
        printf("adder %s\n", adder);
        /* 执行，调用一次返回一条数据 */
        ret = sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}

/* 删除数据 */
void delete_data(sqlite3 *db,int id)
{
    int ret;
    sqlite3_stmt *stmt;
    const char *zTail;
    char sql[128];

    sprintf(sql,"delete from mytable where id = %d;", id);
    sqlite3_prepare_v2(db, sql, -1, &stmt, &zTail);
    ret = sqlite3_step(stmt);
    while (ret == SQLITE_ROW) {
        ret = sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}

/* 更新数据 */
void updata_data(sqlite3 *db, char *addr,char *newaddr)
{
    int ret;
    sqlite3_stmt *stmt;
    const char *zTail;
    char sql[128];

    sprintf(sql,"update mytable set address = \"%s\" where address = \"%s\";", newaddr, addr);
    printf("sql %s\n",sql);
    sqlite3_prepare_v2(db, sql, -1, &stmt, &zTail);
    ret = sqlite3_step(stmt);
    while (ret == SQLITE_ROW) {
        ret = sqlite3_step(stmt);
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}

int main(int argc, char *argv[])
{
    char *tablename = "mytable";
    printf("sqlite test \n");
    db_init();          //打开数据库
    create_table(db);   //创建表
    insert_data(db);    //添加数据
    // query_data(db);
    delete_data(db,4);   //删除数据
    // updata_data(db,"zhongguo","hebei");  //更新数据
    query_data(db);     //查询数据
    // delete_table(db,tablename);  //删除表
    sqlite3_close(db);  //关闭数据库
}