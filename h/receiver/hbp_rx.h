/* Author: t.me/dmedser */

#ifndef HBP_RX_H_
#define HBP_RX_H_

#include "global_cfg.h"
#include <stdint.h>

typedef struct frame_t {
	uint16_t buf[FRAME_LEN];
	uint16_t idx;
} frame_t;

void hbp_rx_init(void);
void hbp_rx_process(void);

extern uint32_t bit_counter;
extern uint32_t bit_stream;

#endif /* HBP_RX_H_ */
