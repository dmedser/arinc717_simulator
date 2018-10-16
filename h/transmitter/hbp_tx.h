/* Author: t.me/dmedser */

#ifndef HBP_TX_H_
#define HBP_TX_H_

#include "global_cfg.h"
#include <Platform_Types.h>
#include <stdint.h>

typedef struct tx_buf {
	uint16_t buf[FRAME_LEN];
	uint16_t idx;
	uint8_t  rsh;
} tx_buf;

void hbp_tx_init(void);
void hbp_tx_process(void);

#endif /* HBP_TX_H_ */
