#ifndef NWSVERSION_H
#define	NWSVERSION_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "nw_defs.h"

#define NWS_ID nws
#define NWS_VERSION_MAJOR 0
#define NWS_VERSION_MINOR 1

#define NWS_STR_VERSION STR(NWS_VERSION_MAJOR) "." STR(NWS_VERSION_MINOR)
#define NWS_STR_ID STR(NWS_ID) "/" NWS_STR_VERSION


#ifdef	__cplusplus
}
#endif

#endif	/* NWSVERSION_H */

