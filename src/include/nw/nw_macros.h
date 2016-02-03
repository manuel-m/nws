#ifndef NWS_MACROS_H
#define	NWS_MACROS_H

#ifdef	__cplusplus
extern "C" {
#endif



#define NW_HTTP_RESPONSE_FMT(status____,serverid____)                          \
"HTTP/1.1 " status____ "\r\n"                                                  \
"Server: " serverid____ "\r\n"                                                 \
"Content-Type: " "%s"                                                          \
"\r\n"                                                                         \
"X-Content-Type-Options: nosniff \r\n"                                         \
"Connection: close\r\n"                                                        \
"\r\n"

#define NW_HTTP_RESPONSE_NOBODY(status____,serverid____)                       \
"HTTP/1.1 " status____ "\r\n"                                                  \
"Server: " serverid____ "\r\n"                                                 \
"Content-Type: " "text/html"                                                   \
"\r\n"                                                                         \
"X-Content-Type-Options: nosniff \r\n"                                         \
"Connection: close\r\n"                                                        \
"\r\n"


#define NW_HTTP_RESPONSE_SET(c_, r_, r_n_, r_type_, url_, status_, serverid_)  \
do{                                                                            \
    char* r_header = NULL;                                                     \
    size_t r_header_n = asprintf(&r_header,                                    \
    NW_HTTP_RESPONSE_FMT(status_, serverid_), r_type_);                        \
    c_->m_resbuf.len = r_header_n + r_n_;                                      \
    c_->m_resbuf.base = (char*) malloc(c_->m_resbuf.len);                      \
    if(NULL == c_->m_resbuf.base) log_ginternal();                             \
    memcpy(c_->m_resbuf.base, r_header, r_header_n);                           \
    memcpy(c_->m_resbuf.base + r_header_n, r_, r_n_);                          \
    log_info(" + REP %s status:%s length:%zu", url_, status_,c_->m_resbuf.len);\
    if (r_header) free(r_header);                                              \
}while(0)

#undef etCAST_CHAR_P

#define NW_HTTP_RESPONSE_NOBODY_SET(c_, url_, status_, serverid_)              \
do{                                                                            \
   c_->m_resbuf.len = STR_FIXED_N(NW_HTTP_RESPONSE_NOBODY(status_,serverid_)); \
   c_->m_resbuf.base = strdup(NW_HTTP_RESPONSE_NOBODY(status_, serverid_));    \
   log_info(" + REP %s status:%s empty body", url_, status_);                  \
}while(0)



#ifdef	__cplusplus
}
#endif

#endif	/* NWS_MACROS_H */

