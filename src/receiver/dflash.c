/* Author: t.me/dmedser */

#include "dflash.h"
#include <IfxFlash.h>

void dflash_wr_u64(uint32_t addr, uint64_t data) {

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


uint64_t dflash_rd_u64(uint32_t addr) {
	return (uint64_t)*((uint32_t *)addr);
}

