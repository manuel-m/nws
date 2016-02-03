#include "nw/nw.h"

/* ==========================================================================
 * string compat
 * ========================================================================== */
static size_t strlncat(char *dst, size_t len, const char *src, size_t n) {
    size_t slen;
    size_t dlen;
    size_t rlen;
    size_t ncpy;
    slen = strnlen(src, n);
    dlen = strnlen(dst, len);
    if (dlen < len) {
        rlen = len - dlen;
        ncpy = slen < rlen ? slen : (rlen - 1);
        memcpy(dst + dlen, src, ncpy);
        dst[dlen + ncpy] = '\0';
    }
    assert(len > slen + dlen);
    return slen + dlen;
}

/* ==========================================================================
 * memory management
 * ========================================================================== */
static void on_alloc_buffer(uv_handle_t *h_, size_t suggested_sz_, uv_buf_t *buf_) {
    (void) h_;
    buf_->base = (char *) calloc(suggested_sz_, sizeof(char));
    buf_->len = suggested_sz_;
}

/* ==========================================================================
 * tcp callbacks
 * ========================================================================== */
static void on_tcp_close(uv_handle_t *handle_) {
    struct httpCli *cli = (struct httpCli *) handle_->data;

    free(cli->m_resbuf.base);
    free(cli);
}

static void on_tcp_after_write(uv_write_t *req_, int status_) {
    if (0 > status_) {
        log_warn("internal: %s", uv_strerror(status_));
        /* to avoid error on close */
        if (status_ == -ECANCELED) return;
    }
    uv_close((uv_handle_t *) req_->handle, on_tcp_close);
}

static void on_tcp_read(uv_stream_t *handle_, ssize_t nread_, const uv_buf_t *buf_) {

    size_t parsed;
    struct httpCli *cli = (struct httpCli *) handle_->data;
    struct httpSrv *srv = cli->m_srv;

    if (nread_ >= 0) {
        parsed = http_parser_execute(
                &cli->m_parser, &srv->m_parser_settings, buf_->base, (size_t) nread_);

        if (parsed < (size_t) nread_) {
            log_err("parse error");
            uv_close((uv_handle_t *) handle_, on_tcp_close);
        }

    } else {
        uv_close((uv_handle_t *) handle_, on_tcp_close);
    }
    free(buf_->base);
}

/* ==========================================================================
 * client
 * ========================================================================== */
static void client_init(struct httpSrv *srv_, struct httpCli *cli_) {
    cli_->m_request_num = srv_->m_request_num;
    cli_->m_srv = srv_;
    cli_->m_parser.data = cli_;
    cli_->m_handle.data = cli_;
    http_parser_init(&cli_->m_parser, HTTP_REQUEST);
}

static void on_tcp_connect(uv_stream_t *handle_, int status_) {

    /* something wrong with status .... exit */
    if (0 > status_) log_rinternal();

    struct httpSrv *server = (struct httpSrv *) handle_->data;

    /* check ip */
    int r;
    struct sockaddr_storage sockCli; /* fix error with struct sockaddr */
    int namelen;
    struct sockaddr_in check_addr;
    char check_ip[17];

    ++(server->m_request_num);

    struct httpCli *cli = (struct httpCli *) calloc(1, sizeof(struct httpCli));

    /* nothing todo ... exit */
    if (NULL == cli) log_rinternal();

    client_init(server, cli);
    uv_tcp_init(uv_default_loop(), &cli->m_handle);

    r = uv_accept(handle_, (uv_stream_t * ) & cli->m_handle);
    if (0 != r) {
        log_info("connect:%s", uv_strerror(r));
        goto err;
    }

    /* get client ip */
    if (0 != uv_tcp_getpeername((uv_tcp_t * ) & cli->m_handle, (struct sockaddr *) &sockCli, &namelen)) {
        log_ginternal();
    }

    check_addr = *(struct sockaddr_in *) &sockCli;

    if (0 != uv_ip4_name(&check_addr, (char *) check_ip, sizeof check_ip)) {
        log_gerr("invalide ip");
    }

    /* [!] in some case receive 0.0.0.0 ip on reconnect ... */
    if (0 != strncmp(check_ip, "0.0.0.0", sizeof("0.0.0.0") - 1)) {
        //        if (0 != http_blockip(check_ip)) {
        //            /* not err unwanted connections ... to avoid big logs */
        //            log_info("reject connection from %s", check_ip);
        //            goto err;
        //        }
    }

    /* accept */
#ifdef XXULTRA_VERBOSE
    log_info("accept connection from %s", check_ip);
#endif
    uv_read_start((uv_stream_t * ) & cli->m_handle, on_alloc_buffer, on_tcp_read);
    return;

    /* reject */
    err:
    /* [!] free also cli structure */
    uv_close((uv_handle_t * ) & cli->m_handle, on_tcp_close);
    return;
}
/* ==========================================================================
 * http callbacks
 * ========================================================================== */

/* alloc and copy body content into cli->pub.m_mess.body
   [!] body buffer will be freed on stat_response end */
static int on_body_cb(http_parser *parser_, const char *p_, size_t sz_) {
    int res = 0;
    struct httpCli *cli = (struct httpCli *) parser_->data;

    const size_t max_body_size = cli->m_srv->m_max_body_size;
    struct message *m = &cli->pub.m_mess;

    if ((m->body_size + sz_) > max_body_size) {
        log_gerr("body response to large %zu/%zu ", m->body_size, max_body_size);
    }

    /* alloc on demand (wait to have a body) */
    if (!m->body_fragment) {
        m->body = (unsigned char *) malloc(max_body_size + 1);
        if (NULL == cli->pub.m_mess.body) {
            log_ginternal();
        }
    }
    /* append result */
    memcpy((m->body + m->body_size), p_, sz_);

    /* prepare for next round  */
    ++m->body_fragment;
    m->body_size += sz_;
    m->body[m->body_size] = '\0';

    end:
    return res;
    err:
    res = -1;
    goto end;
}

static int on_status_cb(http_parser *p_, const char *buf_, size_t sz_) {
    struct httpCli *cli = (struct httpCli *) p_->data;                              \
    struct message *m = &cli->pub.m_mess;
    strlncat(m->response_status, sizeof(m->response_status), buf_, sz_);
    return 0;
}

static int on_header_field_cb(http_parser *p_, const char *buf_, size_t sz_) {
    struct httpCli *cli = (struct httpCli *) p_->data;                              \
    struct message *m = &cli->pub.m_mess;
    if (m->last_header_element != BR_HEADER_FIELD) m->num_headers++;
    strlncat(m->headers[m->num_headers - 1][0], sizeof(m->headers[m->num_headers - 1][0]), buf_, sz_);
    m->last_header_element = BR_HEADER_FIELD;
    return 0;
}

static int on_message_begin_cb(http_parser *p_) {
    struct httpCli *cli = (struct httpCli *) p_->data;                              \
    struct message *m = &cli->pub.m_mess;
    m->body_fragment = 0;
    m->body_size = 0;
    m->message_begin_cb_called = 1;
    return 0;
}

static int on_url_cb(http_parser *p_, const char *buf_, size_t sz_) {
    struct httpCli *cli = (struct httpCli *) p_->data;                              \
    struct message *m = &cli->pub.m_mess;
    strlncat(m->request_url, sizeof(m->request_url), buf_, sz_);
    m->request_url_n = sz_;
    return 0;
}

static int on_headers_complete_cb(http_parser *p_) {
    struct httpCli *cli = (struct httpCli *) p_->data;
    struct message *m = &cli->pub.m_mess;
    m->method = p_->method;
    m->status_code = p_->status_code;
    m->http_major = p_->http_major;
    m->http_minor = p_->http_minor;
    m->headers_complete_cb_called = 1;
    m->should_keep_alive = http_should_keep_alive(p_);
    return 0;
}

static int on_header_value_cb(http_parser *p_, const char *buf_, size_t sz_) {
    struct httpCli *cli = (struct httpCli *) p_->data;                              \
    struct message *m = &cli->pub.m_mess;
    strlncat(m->headers[m->num_headers - 1][1],
             sizeof(m->headers[m->num_headers - 1][1]), buf_, sz_);
    m->last_header_element = BR_HEADER_VALUE;
    return 0;
}


static int on_message_complete_cb(http_parser *p_) {
    struct httpCli *cli = (struct httpCli *) p_->data;
    struct httpSrv *srv = cli->m_srv;
    struct message *m = &cli->pub.m_mess;
    int res = 0;

    if (0 > srv->m_on_stats_response_cb(cli)) {
        NW_HTTP_RESPONSE_NOBODY_SET(cli, m->request_url, "400 Bad Request", NWS_STR_ID);
    }

    if (0 > uv_write(&cli->m_write_req, (uv_stream_t * ) & cli->m_handle, &cli->m_resbuf,
                     1, on_tcp_after_write))
        log_ginternal();

    m->message_complete_cb_called = 1;

    end:
    /* purge body */
    if (cli->pub.m_mess.body) {
        free(cli->pub.m_mess.body);
        cli->pub.m_mess.body_size = 0;
    }
    return res;

    err:
    res = -1;
    goto end;
}

void Nw::init(struct httpSrvConf *conf_) {

    memset(&this->m_srv, 0, sizeof(struct httpSrv));

    this->m_srv.m_max_connections = conf_->max_connections;
    this->m_srv.m_max_body_size = conf_->max_body_size;
    this->m_srv.m_on_stats_response_cb = conf_->on_stats_response_cb;

    this->m_srv.m_request_num = 0;
    this->m_srv.m_parser_settings.on_message_begin = on_message_begin_cb;
    this->m_srv.m_parser_settings.on_url = on_url_cb;
    this->m_srv.m_parser_settings.on_status = on_status_cb;
    this->m_srv.m_parser_settings.on_header_field = on_header_field_cb;
    this->m_srv.m_parser_settings.on_header_value = on_header_value_cb;
    this->m_srv.m_parser_settings.on_headers_complete = on_headers_complete_cb;
    this->m_srv.m_parser_settings.on_body = on_body_cb;
    this->m_srv.m_parser_settings.on_message_complete = on_message_complete_cb;
    this->m_srv.m_port = NW_LISTEN_DEFAULT_PORT;
}

int Nw::listen() {

    log_info("+ http listen %d", this->m_srv.m_port);
    uv_loop_t *loop = uv_default_loop();
    struct httpSrv *srv = &this->m_srv;

    uv_tcp_init(loop, &srv->m_handler);
    srv->m_handler.data = srv;
    if (0 != uv_ip4_addr("0.0.0.0", srv->m_port, &srv->m_addr))die_internal();
    if (0 != uv_tcp_bind(&srv->m_handler, (const struct sockaddr *) &srv->m_addr, 0))
        log_return_1("bind port %d already in use", srv->m_port);
    if (0 != uv_listen((uv_stream_t * ) & srv->m_handler,
                       srv->m_max_connections, on_tcp_connect)) {
        log_return_1("listen port %d already in use", srv->m_port);
    }

    //    /* ref time update */
    //    {
    //        uv_timer_init(loop, &srv_->m_timeRefTimer);
    //        srv_->m_timeRefTimer.data = srv_;
    //        uv_timer_start(&srv_->m_timeRefTimer, http_timeRef_cb, 0, BR_HTTP_REF_TIMER_PULL_PERIOD);
    //    }

    //    /* needs tokens to activate */
    //    {
    //        tokenizer_t* tz = &srv_->m_tokenizer;
    //        if (tz->max_elems > 0) {
    //            uv_timer_init(loop, &srv_->m_tokChecktimer);
    //            srv_->m_tokChecktimer.data = srv_;
    //            uv_timer_start(&srv_->m_tokChecktimer, http_tok_check_cb, 0, BR_HTTP_TOK_PULL_PERDIOD);
    //        }
    //    }

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    return 0;
}

