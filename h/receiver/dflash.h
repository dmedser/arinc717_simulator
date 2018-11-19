/* Author: t.me/dmedser */

#ifndef DFLASH_H_
#define DFLASH_H_

#include <stdint.h>

/* Writes 64-bit word to an dflash absolute address (8-byte aligned) */
void dflash_wr_u64(uint32_t addr, uint64_t data);

uint64_t dflash_rd_u64(uint32_t addr);

#endif /* DFLASH_H_ */
