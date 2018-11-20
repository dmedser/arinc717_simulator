/* Author: t.me/dmedser */

#include "global_cfg.h"
#include "gtm.h"

uint16_t bitrate_bps = 0;
uint16_t bit_tx_period = 0;
uint16_t sync_words[NUMBER_OF_SUBFRAMES] = {0};
uint32_t sync_words_reversed[NUMBER_OF_SUBFRAMES] = {0};

void update_bitrate_bps(uint16_t value) {
	bitrate_bps = value;
	bit_tx_period = (uint16_t)(f_gtm / bitrate_bps);
}


void update_sync_word(uint8_t idx, uint16_t value) {
	sync_words[idx] = value;
	uint32_t sw32 = (uint32_t)value;
	sync_words_reversed[idx] = (((sw32 & (1 << 0)) << (11 - 0)) | ((sw32 & (1 << 1))  << (10 - 1)) | ((sw32 & (1 << 2))  << (9 - 2)) |
							    ((sw32 & (1 << 3)) << (8 - 3))  | ((sw32 & (1 << 4))  << (7 - 4))  | ((sw32 & (1 << 5))  << (6 - 5)) |
							    ((sw32 & (1 << 6)) >> (6 - 5))  | ((sw32 & (1 << 7))  >> (7 - 4))  | ((sw32 & (1 << 8))  >> (8 - 3)) |
							    ((sw32 & (1 << 9)) >> (9 - 2))  | ((sw32 & (1 << 10)) >> (10 - 1)) | ((sw32 & (1 << 11)) >> (11 - 0))) << 20;
}

