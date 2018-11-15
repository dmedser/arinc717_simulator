/* Author: t.me/dmedser */

#ifndef HBP_RX_H_
#define HBP_RX_H_

#include "global_cfg.h"
#include <stdint.h>
#include <Platform_Types.h>

typedef struct frame_t {
	uint16_t buf[FRAME_LEN];
	uint16_t tx_idx;						/* Transmit to CAN */
	uint16_t rx_idx;						/* Receive from ARINC */
} frame_t;

typedef struct superframe_t {
	frame_t subframes[NUM_OF_SUBFRAMES];
	uint8_t tx_idx;							/* Transmit to CAN */
	uint8_t rx_idx;							/* Receive from ARINC */
	boolean (*is_empty)(void);
	uint64_t (*get_8_bytes_from)(frame_t *frame);
} superframe_t;

typedef struct bitstream_t {
	uint32_t bits;
	uint32_t counter;
	void (*put_2_decoded_words_into)(frame_t *frame);
} bitstream_t;

void hbp_rx_init(void);

extern superframe_t superframe;
extern bitstream_t bitstream;

#endif /* HBP_RX_H_ */
