/* Author: t.me/dmedser */

#ifndef GLOBAL_CFG_H_
#define GLOBAL_CFG_H_

#define TRANSMITTER						0
#define RECEIVER						1

#define OP_MODE							0

#include <stdint.h>

// 768   bps = 64   wps
// 1536  bps = 128  wps
// 3072  bps = 256  wps
// 6144  bps = 512  wps
// 12288 bps = 1024 wps

#define BITRATE_BPS						12288
#define BITS_IN_WORD					12
#define FRAME_LEN						(BITRATE_BPS / BITS_IN_WORD)

#define BIT_TX_PERIOD					((uint16_t)(f_gtm / BITRATE_BPS))
#define SECOND_HALF_OF_BIT_TX_PERIOD	((3 * BIT_TX_PERIOD) / 4)
#define OVER_BIT_TX_PERIOD				((5 * BIT_TX_PERIOD) / 4)

#define NUM_OF_SUBFRAMES				4
#define SW1								0b001001000111
#define SW2								0b010110111000
#define SW3								0b101001000111
#define SW4								0b110110111000

typedef struct buf_u16_t {
	uint16_t buf[NUM_OF_SUBFRAMES];
	uint8_t  idx;
} buf_u16_t;

extern uint32_t f_gtm;
extern buf_u16_t sws;

#endif /* GLOBAL_CFG_H_ */
