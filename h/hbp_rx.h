/* Author: t.me/dmedser */

#ifndef HBP_RX_H_
#define HBP_RX_H_

#include <Platform_Types.h>
#include <stdint.h>

#define	RX_FRAME_MAX_LEN	10

typedef struct rx_buf {
	uint16_t buf[RX_FRAME_MAX_LEN];
	uint16_t idx;
	uint8_t  lsh;
	boolean  busy;
} rx_buf;

void hbp_rx_init(void);
void hbp_rx_process(void);

#endif /* HBP_RX_H_ */
