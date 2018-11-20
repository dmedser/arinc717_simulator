/* Author: t.me/dmedser */

#include "dflash.h"
#include <IfxFlash.h>
#include <IfxFlash_cfg.h>

#define	DFLASH_PAGE_SIZE						sizeof(uint64_t)
#define DFLASH_ADDRESS(START_ADDRESS, OFFSET)	(START_ADDRESS + (OFFSET * DFLASH_PAGE_SIZE))
#define BITRATE_DFLASH_ADDRESS					DFLASH_ADDRESS(IFXFLASH_DFLASH_START, 0)
#define SYNC_WORD_DFLASH_ADDRESS(NUMBER)		DFLASH_ADDRESS(IFXFLASH_DFLASH_START, NUMBER)

/* Writes 64-bit word to an data flash absolute address (8-byte aligned) */
static void dflash_wr_u64(uint32_t addr, uint64_t data) {

	IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

	IfxFlash_erasePhysicalSector(addr);
	IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

	IfxFlash_enterPageMode(addr);
	IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

	uint32_t data_low  = (uint32_t)data;
	uint32_t data_high = (uint32_t)(data >> 32);

	IfxFlash_loadPage(addr, data_low, data_high);
	IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

	uint16_t password = IfxScuWdt_getSafetyWatchdogPassword();

	IfxScuWdt_clearSafetyEndinit(password);

	IfxFlash_writePage(addr);
	IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

	IfxScuWdt_setSafetyEndinit(password);

	IfxFlash_resetToRead(0);
	IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
}


static uint64_t dflash_rd_u64(uint32_t addr) {
	return (uint64_t)*((uint32_t *)addr);
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
				dflash_wr_u64(SYNC_WORD_DFLASH_ADDRESS((i + 1)), (uint64_t)sync_words[i]);
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

