# nws helloword

set(UV_A /usr/lib/x86_64-linux-gnu/libuv.a)

#add_executable (nws_echo${etD} nws_echo.cpp)
#target_link_libraries (nws_echo${etD} ${UV_A} -lpthread -lrt)

add_executable (nws_http_hello${etD} nws_http_hello.cpp)
target_link_libraries (nws_http_hello${etD} nw${etD} ${UV_A} http_parser${etD} -lpthread -lrt)
