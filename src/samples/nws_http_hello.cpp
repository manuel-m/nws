#include "nw/nw.h"

static int on_stats_response_cb(struct httpCli *c_) {
    int res = 0;
    const char *url;
    if (NULL == c_) log_ginternal();
    url = c_->pub.m_mess.request_url;

    if (!strcmp(url, "/error")) {
        NW_HTTP_RESPONSE_NOBODY_SET(c_, url, HTTP_STATUS_400, "helloword");
    } else {
        static const char FAKERESP[] =
                "<!DOCTYPE html>"
                        "<html lang=\"en\">"
                        "<head>"
                        "<meta charset=\"utf-8\"/>"
                        "</head>"
                        "<body>"
                        "needle web server"
                        "</body>"
                        "</html>";

        NW_HTTP_RESPONSE_SET(c_, (unsigned char *) FAKERESP, STR_FIXED_N(FAKERESP), \
            "text/html", url, HTTP_STATUS_200, "helloword");
    }

    end:
    return res;

    err:
    res = -1;
    goto end;
}

int main() {

    Nw nw;
    NwConf conf;

    if (conf.load("src/tests/data/server.json") != 0) log_return_1("invalid configuration");
    if (nw.init(&on_stats_response_cb, conf.get_c()).listen() != 0) log_return_1("server shutdown");

    return 0;

}

