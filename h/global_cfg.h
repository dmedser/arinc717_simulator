/* Author: t.me/dmedser */

#ifndef GLOBAL_CFG_H_
#define GLOBAL_CFG_H_

#include <stdint.h>

// 768   bps = 64   wps
// 1536  bps = 128  wps
// 3072  bps = 256  wps
// 6144  bps = 512  wps
// 12288 bps = 1024 wps

#define BITRATE_BPS			12288
#define BITS_IN_WORD		12
#define FRAME_LEN			(BITRATE_BPS / BITS_IN_WORD)

#define BIT_TX_PERIOD		((uint16_t)(f_gtm / BITRATE_BPS))

#define NUM_OF_SUBFRAMES	4
#define SYNC_WORD1			0b001001000111
#define SYNC_WORD2			0b010110111000
#define SYNC_WORD3			0b101001000111
#define SYNC_WORD4			0b110110111000

typedef struct sw_buf {
	uint16_t buf[NUM_OF_SUBFRAMES];
	uint8_t  idx;
} sw_buf;

extern uint32_t f_gtm;
extern sw_buf sws;

#endif /* GLOBAL_CFG_H_ */
