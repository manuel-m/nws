#include "nw/nw.h"

static const char _data[] = "<html><head></head><body>needle web server</body></html>";

static int on_stats_response_cb(struct httpCli *c_) {
    NW_HTTP_RESPONSE_SET(c_, (unsigned char *) _data, STR_FIXED_N(_data), "text/html",
                         c_->pub.m_mess.request_url, HTTP_STATUS_200, "nws_start");
}

int main() {

    Nw nw;
    return nw.init(&on_stats_response_cb).listen();

}

