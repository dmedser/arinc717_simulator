/* Author: t.me/dmedser */

#ifndef HBP_RX_H_
#define HBP_RX_H_

#include "global_cfg.h"
#include <Platform_Types.h>
#include <stdint.h>

typedef struct rx_buf {
	uint16_t buf[FRAME_LEN];
	uint16_t idx;
	uint8_t  lsh;
} rx_buf;

void hbp_rx_init(void);
void hbp_rx_process(void);

#endif /* HBP_RX_H_ */
