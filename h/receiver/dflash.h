/* Author: t.me/dmedser */

#ifndef DFLASH_H_
#define DFLASH_H_

#include <stdint.h>
#include <IfxFlash_cfg.h>

#define	DFLASH_PAGE_SIZE						sizeof(uint64_t)
#define DFLASH_ADDRESS(START_ADDRESS, OFFSET)	(START_ADDRESS + (OFFSET * DFLASH_PAGE_SIZE))

/* Writes 64-bit word to an dflash absolute address (8-byte aligned) */
void dflash_wr_u64(uint32_t addr, uint64_t data);

uint64_t dflash_rd_u64(uint32_t addr);

#endif /* DFLASH_H_ */
