#include <string.h>
#include "mongoose/mongoose.h"


/* 监听回调函数 */
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{   
    /* http连接 */
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;    //接收到的http数据
        if (mg_http_match_uri(hm, "html/index.html")) {
            mg_http_reply(c, 200, "", "{%m:%d}\n", mg_print_esc, 0, "status", 1);
        } else {
            struct mg_http_serve_opts opts = {.root_dir = "../html"};
            mg_http_serve_dir(c, hm, &opts);
        } 
    }
}



int main(int argc, char *argv[]) 
{
    struct mg_mgr mgr;      //实例化一个对象
    mg_mgr_init(&mgr);      //初始化
    mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, &mgr);   //监听
    for (;;) {
        mg_mgr_poll(&mgr, 1000);    //
    }
    mg_mgr_free(&mgr);  //释放
    return 0;
}