#ifndef NWS_NW_H
#define NWS_NW_H

#include "http_parser.h"
#include "uv.h"

#include "nw_version.h"
#include "nw_defs.h"
#include "nw_macros.h"
#include "nw_http_defs.h"

#define NW_LISTEN_DEFAULT_PORT 8080

enum LastHeaderElement {
    NONE = 0, BR_HEADER_FIELD, BR_HEADER_VALUE
};

struct httpSrvConf {
    size_t max_body_size;
    size_t max_connections;
    int port;
    int (*on_stats_response_cb)(struct httpCli *c_);
};

struct httpSrv {
    struct sockaddr_in m_addr;
    uv_tcp_t m_handler;
    size_t m_max_connections;
    size_t m_max_body_size;
    http_parser_settings m_parser_settings;
    int m_request_num;
    int m_port;
    int (*m_on_stats_response_cb)(struct httpCli *c_);
};

struct message {
    const char *name; // for debugging purposes
    enum http_parser_type type;
    unsigned int method;
    int status_code;
    char response_status[2048];
    char request_url[2048];
    size_t request_url_n;
    size_t body_size;
    int body_fragment;
    unsigned char *body;
    const char *host;
    const char *userinfo;
    uint16_t port;
    int num_headers;

    enum LastHeaderElement last_header_element;

    char headers[13][2][2048];
    int should_keep_alive;

    const char *upgrade; // upgraded body

    unsigned short http_major;
    unsigned short http_minor;

    int message_begin_cb_called;
    int headers_complete_cb_called;
    int message_complete_cb_called;
    int body_is_final;

};

struct httpCli {
    uv_tcp_t m_handle;
    http_parser m_parser;
    uv_write_t m_write_req;
    int m_request_num;
    struct httpSrv *m_srv;
    uv_buf_t m_resbuf;

    struct {
        struct message m_mess;
    } pub;

};

class Nw {

protected:

    struct httpSrv m_srv;

public:

//    virtual void args(int argc, char **argv) = 0;
    void init(struct httpSrvConf* conf_);
    int listen();


};

#endif //NWS_NW_H
