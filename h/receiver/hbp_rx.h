/* Author: t.me/dmedser */

#ifndef HBP_RX_H_
#define HBP_RX_H_

#include "global_cfg.h"
#include <stdint.h>
#include <Platform_Types.h>

typedef struct subframe_t {
	uint16_t buf[SUBFRAME_LEN_MAX];
	uint16_t tx_idx;						/* Transmit to CAN */
	uint16_t rx_idx;						/* Receive from ARINC */
} subframe_t;

typedef struct frame_t {
	subframe_t subframes[NUMBER_OF_SUBFRAMES];
	uint8_t tx_idx;							/* Transmit to CAN */
	uint8_t rx_idx;							/* Receive from ARINC */
	boolean (*is_empty)(void);
	uint64_t (*get_8_bytes_from)(subframe_t *subframe);
} frame_t;

typedef struct bitstream_t {
	uint32_t bits;
	uint32_t counter;
	void (*put_2_decoded_words_into)(subframe_t *subframe);
} bitstream_t;

#define idx_of_subframe_to_rx	frame.rx_idx
#define idx_of_subframe_to_tx	frame.tx_idx
#define subframe_to_rx 			frame.subframes[idx_of_subframe_to_rx]
#define subframe_to_tx			frame.subframes[idx_of_subframe_to_tx]
#define idx_of_word_to_rx		subframe_to_rx.rx_idx
#define idx_of_word_to_tx		subframe_to_tx.tx_idx

void hbp_rx_init(void);

extern frame_t frame;
extern bitstream_t bitstream;

#endif /* HBP_RX_H_ */
