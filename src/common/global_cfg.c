/* Author: t.me/dmedser */

#include "global_cfg.h"
#include "gtm.h"
#include "dflash.h"

#define BITRATE_BPS_DEFAULT					BITRATE_BPS_MAX

#define SW_1_DEFAULT						0b001001000111	// 0x0247
#define SW_2_DEFAULT						0b010110111000	// 0x05B8
#define SW_3_DEFAULT						0b101001000111	// 0x0A47
#define SW_4_DEFAULT						0b110110111000	// 0x0DB8

#define BITRATE_DFLASH_ADDRESS				DFLASH_ADDRESS(IFXFLASH_DFLASH_START, 0)
#define SYNC_WORD_DFLASH_ADDRESS(NUMBER)	DFLASH_ADDRESS(IFXFLASH_DFLASH_START, NUMBER)

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


void upload_into_dflash(param_no param) {
	switch(param){
		case BITRATE: {
			dflash_wr_u64(BITRATE_DFLASH_ADDRESS, (uint64_t)bitrate_bps);
			break;
		}
		case SYNC_WORDS: {
			uint8_t i = 0;
			for(; i < NUMBER_OF_SUBFRAMES; i++) {
				dflash_wr_u64(SYNC_WORD_DFLASH_ADDRESS(i + 1), (uint64_t)sync_words[i]);
			}
			break;
		}
	}
}


void download_from_dflash(param_no param) {
	switch(param){
		case BITRATE: {
			update_bitrate_bps((uint16_t)dflash_rd_u64(BITRATE_DFLASH_ADDRESS));
			break;
		}
		case SYNC_WORDS: {
			uint8_t i = 0;
			for(; i < NUMBER_OF_SUBFRAMES; i++) {
				update_sync_word(i, (uint16_t)dflash_rd_u64(SYNC_WORD_DFLASH_ADDRESS((i + 1))));
			}
			break;
		}
	}
}

