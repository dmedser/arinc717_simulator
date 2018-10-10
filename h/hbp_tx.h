/* Author: t.me/dmedser */

#ifndef HBP_TX_H_
#define HBP_TX_H_

#include <Platform_Types.h>
#include <stdint.h>

#define TX_FRAME_MAX_LEN	1024

typedef struct tx_buf {
	uint16_t buf[TX_FRAME_MAX_LEN];
	uint16_t idx;
	uint16_t num;
	uint8_t  rsh;
	boolean  busy;
} tx_buf;

void hbp_tx_init(void);
void hbp_tx_process(void);

#endif /* HBP_TX_H_ */
