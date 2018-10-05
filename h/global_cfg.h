#ifndef GLOBAL_CFG_H_
#define GLOBAL_CFG_H_

#include <IfxScuCcu.h>
#include <stdint.h>

#define BITRATE_BPS	12288
// 768   bps = 64   wps
// 1536  bps = 128  wps
// 3072  bps = 256  wps
// 6144  bps = 512  wps
// 12288 bps = 1024 wps

#define F_GTM			100000000
#define BIT_TX_PERIOD	((uint16_t)(F_GTM/*IfxScuCcu_getGtmFrequency()*/ / (BITRATE_BPS * 2)))

#endif /* GLOBAL_CFG_H_ */
