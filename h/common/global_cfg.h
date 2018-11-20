/* Author: t.me/dmedser */

#ifndef GLOBAL_CFG_H_
#define GLOBAL_CFG_H_

#include <stdint.h>

#define TRANSMITTER							0
#define RECEIVER							1

#define OP_MODE								RECEIVER

// 768   bps = 64   wps
// 1536  bps = 128  wps
// 3072  bps = 256  wps
// 6144  bps = 512  wps
// 12288 bps = 1024 wps

#define BITRATE_BPS_MAX						12288
#define BITS_IN_WORD						12
#define SUBFRAME_LEN						(bitrate_bps / BITS_IN_WORD)
#define FRAME_LEN							(SUBFRAME_LEN * 4)
#define NUMBER_OF_CAN_MSG_IN_FRAME			(FRAME_LEN / 4)
#define NUMBER_OF_WORDS_IN_CAN_MSG			4
#define SUBFRAME_LEN_MAX					(BITRATE_BPS_MAX / BITS_IN_WORD)
#define NUMBER_OF_SUBFRAMES					4

#define CAN_DST_MO_MSG_ID_TRANSMITTER		0x601
#define CAN_DST_MO_MSG_ID_RECEIVER			0x602

typedef uint8_t param_no;

#define BITRATE								0
#define SYNC_WORDS							1

extern uint16_t bitrate_bps;
extern uint16_t bit_tx_period;
extern uint16_t sync_words[NUMBER_OF_SUBFRAMES];
extern uint32_t sync_words_reversed[NUMBER_OF_SUBFRAMES];

void update_sync_word(uint8_t idx, uint16_t value);
void update_bitrate_bps(uint16_t value);
void upload_into_dflash(param_no param);
void download_from_dflash(param_no param);

#endif /* GLOBAL_CFG_H_ */
