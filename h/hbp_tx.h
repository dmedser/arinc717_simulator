#ifndef HBP_TX_H_
#define HBP_TX_H_

#include <stdint.h>
#include <Platform_Types.h>

#define	FRAME_MAX_LEN		1024

typedef struct tx_buf {
	uint16_t buf[FRAME_MAX_LEN];
	uint16_t idx;
	uint16_t num;
	uint8_t  rsh;
	boolean  busy;
} tx_buf;

extern tx_buf txd;

void hbp_tx(void);

#endif /* HBP_TX_H_ */
