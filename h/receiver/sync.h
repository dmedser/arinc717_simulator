/* Author: t.me/dmedser */

#ifndef SYNC_H_
#define SYNC_H_

#include <stdint.h>

#define BUF_SIZE		100

typedef enum sync_states {
	IDLE,
	SW12,
	SW23,
	SW34,
	SW41
} sync_states;

typedef struct buf_t {
	uint32_t buf[BUF_SIZE];
	uint16_t idx;
} buf_t;

void sync_process(void);

#endif /* SYNC_H_ */
