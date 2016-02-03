
#ifndef NW_HTTP_H
#define	NW_HTTP_H

#ifdef	__cplusplus
extern "C" {
#endif

/* ==========================================================================
 * http format
 * ========================================================================== */
#define api_http_response_types(XX)                                            \
    XX(html, text/html)                                                        \
    XX(js, text/javascript)                                                    \
    XX(md, text/plain)                                                         \
    XX(txt, text/plain)                                                        \
    XX(json, application/json)                                                 \
    XX(ico, image/x-icon)                                                      \
    XX(png, image/png)                                                         \
    XX(css, text/css)                                                          \
    XX(jpg, image/jpeg)                                                        \
    XX(ttf, font/ttf)                                                          \
    XX(txt, text/plain)                                                        \
    XX(eot, application/vnd.ms-fontobject)                                     \
    XX(svg, image/svg+xml)                                                     \
    XX(woff, application/font-woff)                                            \
    XX(otf, font/opentype)                                                     \
    XX(gif, image/gif)

#define api_http_status(XX)\
    XX(success,200,"OK")                                                       \
    XX(success,200,"OK")                                                       \
    XX(bad_request,400,"Bad Request")



#ifdef	__cplusplus
}
#endif

#endif	/* NW_HTTP_H */

