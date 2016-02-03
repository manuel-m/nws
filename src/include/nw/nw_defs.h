
#ifndef LOG_H
#define	LOG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>


#define ARRAY_N(a) sizeof (a) / sizeof ((a)[0])
#define STR_FIXED_N(a) (ARRAY_N(a) - 1)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define xxMMFILE__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define log_err(M, ...) fprintf(stderr, "[ ERR] (%s:%4d) " M "\n", xxMMFILE__, __LINE__, ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%4d) " M "\n", xxMMFILE__, __LINE__, ##__VA_ARGS__)
#define log_user(M, ...) do{fprintf(stdout,  M "\n", ##__VA_ARGS__);}while(0);
#define log_guser(M, ...) do{fprintf(stdout,  M "\n", ##__VA_ARGS__);goto err;}while(0);
#define log_ruser(M, ...) do{fprintf(stdout,  M "\n", ##__VA_ARGS__);return;}while(0);
#ifdef NDEBUG
#define log_info(M, ...) fprintf(stdout, "[INFO] " M "\n", ##__VA_ARGS__)
#else
#define log_info(M, ...) fprintf(stdout, "[INFO] (%s:%4d) " M "\n", xxMMFILE__, __LINE__, ##__VA_ARGS__)
#endif
#define log_gerr(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto err; }
#define log_internal(M, ...)  { log_err("internal", ##__VA_ARGS__); }
#define log_ginternal(M, ...)  { log_gerr("internal", ##__VA_ARGS__); }
#define log_rinternal(M, ...)  { log_err("internal", ##__VA_ARGS__); return;}
#define log_rerr(M, ...)  { log_err(M, ##__VA_ARGS__); return 1; }
#define log_return(M, ...)  { log_err(M, ##__VA_ARGS__); return; }
#define log_return_NULL(M, ...)  { log_err(M, ##__VA_ARGS__); return NULL; }
#define log_return_1(M, ...)  { log_err(M, ##__VA_ARGS__); return 1; }
#define die(M, ...)  { log_err(M, ##__VA_ARGS__); exit(1); }
#define die_internal(M, ...)  { die("internal", ##__VA_ARGS__); }



#ifdef	__cplusplus
}
#endif

#endif	/* LOG_H */

