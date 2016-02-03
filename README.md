# Needle web server

A simple CPP web server on top of [libuv](https://github.com/libuv/libuv)

### Usage

```
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
```
or check [samples](https://github.com/manuel-m/nws/tree/master/src/samples)

* configuration
```
make configure
```

* build on change
```
make watch
```

* unit tests
```
make tests
```

### Pre-requisites

* [cmake](https://cmake.org)
* [gyp](https://gyp.gsrc.io)
* [ninja build system](https://ninja-build.org)
