#ifndef HBP_RX_H_
#define HBP_RX_H_

#include <Platform_Types.h>
#include <stdint.h>

#define	FRAME_MAX_LEN	10

typedef struct rx_buf {
	uint16_t buf[FRAME_MAX_LEN];
	uint16_t idx;
	uint8_t  lsh;
	boolean  busy;
} rx_buf;

extern rx_buf rxd;

#endif /* HBP_RX_H_ */
