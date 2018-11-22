/* Author: t.me/dmedser */

#include "hbp_rx.h"
#include "edge_capture.h"
#include "bit_capture.h"
#include "sync.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>

static boolean frame_is_empty(void);
static uint64_t frame_get_8_bytes_from(subframe_t *subframe);
static void bitstream_put_2_decoded_words_into(subframe_t *subframe);
/*static void bitstream_put_2_raw_words_into(subframe_t *subframe);*/

static boolean first_edge = TRUE;
static uint32_t frame_bit_counter = 0;

frame_t frame = {{{{0}, 0, 0}, {{0}, 0, 0}, {{0}, 0, 0}, {{0}, 0, 0}}, 0,	0,
					frame_is_empty,
					frame_get_8_bytes_from};

bitstream_t bitstream = {0, 0, bitstream_put_2_decoded_words_into};

static boolean frame_is_empty(void) {

	return (idx_of_subframe_to_rx == idx_of_subframe_to_tx) &&
		   ((idx_of_word_to_rx - idx_of_word_to_tx) < NUMBER_OF_WORDS_IN_CAN_MSG);
}


static uint64_t frame_get_8_bytes_from(subframe_t *subframe) {
	uint64_t words[4] = {0};
	uint8_t i = 0;
	for(; i < 4; i++) {
		words[i] = (uint64_t)(subframe->buf[subframe->tx_idx + i]);
	}
	return (words[3] << 48) | (words[2] << 32) | (words[1] << 16) | (words[0] << 0);
}


static void bitstream_put_2_decoded_words_into(subframe_t *subframe) {
	#define BITS_31_20(u32)	(u32 & 0xFFF00000)
	#define BITS_19_8(u32)	(u32 & 0x000FFF00)
	#define BIT_31(u32)		(u32 & 0x80000000)

	uint32_t lw = BITS_31_20(bitstream.bits);
	uint32_t rw = BITS_19_8(bitstream.bits) << 12;
	uint8_t i = 0;
	for(; i < 13; i++) {
		if(BIT_31(lw) != 0) {
			subframe->buf[subframe->rx_idx] |= (1 << i);
		}
		if(BIT_31(rw) != 0) {
			subframe->buf[subframe->rx_idx + 1] |= (1 << i);
		}
		lw <<= 1;
		rw <<= 1;
	}
}


void hbp_rx_init(void) {
	edge_capture_timer_init();
	bit_capture_timer_init();
}


/*static void bitstream_put_2_raw_words_into(frame_t *frame) {
	#define BITS_31_20(u32)	(u32 & 0xFFF00000)
	#define BITS_19_8(u32)	(u32 & 0x000FFF00)

	frame->buf[frame->rx_idx++] = (uint16_t)(BITS_31_20(bitstream.bits) >> 20);
	frame->buf[frame->rx_idx++] = (uint16_t)(BITS_19_8(bitstream.bits) >> 8);
}*/


static inline void bitrate_error_handling(void) {
	bct_off();
	bct_reset();
	clear_sync_flags();
	first_edge = TRUE;
}


static inline void call_can_tx_routine(void) {
	MODULE_SRC.GPSR.GPSR[0].SR1.B.SETR = 0b1;
}


void ISR_edge_capture(void) {
	IfxCpu_forceDisableInterrupts();

	/* Flag must be cleared manually */
	GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = 0b1;

	if(first_edge) {
		bct_on();
		first_edge = FALSE;
	}
	else {
		uint16_t bct_value = get_bct_value();

		/* Half period */
		if((HALF_BIT_TX_PERIOD_LOWER_BOUND < bct_value) && (bct_value < HALF_BIT_TX_PERIOD_UPPER_BOUND)) {
			bitstream.bits |= 1;
		}

		/* Full period */
		else if((BIT_TX_PERIOD_LOWER_BOUND < bct_value) && (bct_value < BIT_TX_PERIOD_UPPER_BOUND)) {
			bct_reset();
			bitstream.bits <<= 1;
			bitstream.counter++;

			sync_flags_t sync_flags = sw_tracking();

			#define ALL_SYNC_FLAGS_ARE_SET				((sync_flags & SYNC_FLAGS_MASK) == SYNC_FLAGS_MASK)
			#define NEW_2_WORDS_IN_BIT_STREAM_READY		((frame_bit_counter % (BITS_IN_WORD * 2)) == 0)
			#define SUBFRAME_RECEIVED 					(frame_bit_counter == (uint32_t)(bitrate_bps - 1))

			if(ALL_SYNC_FLAGS_ARE_SET) {

				if(NEW_2_WORDS_IN_BIT_STREAM_READY) {
					bitstream.put_2_decoded_words_into(&subframe_to_rx);
					idx_of_word_to_rx += 2;
				}

				if(SUBFRAME_RECEIVED) {
					idx_of_word_to_rx = 0;
					idx_of_subframe_to_rx = (idx_of_subframe_to_rx + 1) % NUMBER_OF_SUBFRAMES;
					frame_bit_counter = 0;
				}
				else {
					frame_bit_counter++;
				}

				if(!frame.is_empty()) {
					call_can_tx_routine();
				}

			}
		}

		else {
			bitrate_error_handling();
		}

	}

	IfxCpu_enableInterrupts();
}


void ISR_bit_tx_timeout(void) {
	IfxCpu_forceDisableInterrupts();

	/* Flag must be cleared manually */
	GTM_TIM0_CH1_IRQ_NOTIFY.B.NEWVAL = 0b1;

	bitrate_error_handling();

	IfxCpu_enableInterrupts();
}

