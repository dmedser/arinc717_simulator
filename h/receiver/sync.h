/* Author: t.me/dmedser */

#ifndef SYNC_H_
#define SYNC_H_

#include <stdint.h>
#include <Platform_Types.h>

#define STAMPS_BUF_SIZE		100

typedef enum sync_states {
	IDLE,
	SW12,
	SW23,
	SW34,
	SW41
} sync_states;

typedef struct buf_u32_t {
	uint32_t buf[STAMPS_BUF_SIZE];
	uint16_t idx;
} buf_u32_t;

void sync_process(void);

#endif /* SYNC_H_ */
