# nws sample

set(UV_A /usr/lib/x86_64-linux-gnu/libuv.a)


add_executable (nws_http_start${etD} nws_http_start.cpp)
target_link_libraries (nws_http_start${etD} nw${etD} ${UV_A} http_parser${etD} jsoncpp${etD} -lpthread -lrt)

add_executable (nws_http_hello${etD} nws_http_hello.cpp)
target_link_libraries (nws_http_hello${etD} nw${etD} ${UV_A} http_parser${etD} jsoncpp${etD} -lpthread -lrt)
