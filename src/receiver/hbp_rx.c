/* Author: t.me/dmedser */

#include "hbp_rx.h"
#include "edge_capture.h"
#include "bit_capture.h"
#include "sync.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>

#define ALL_SYNC_FLAGS_ARE_SET				((sync_flags & SYNC_FLAGS_MASK) == SYNC_FLAGS_MASK)
#define NEW_2_WORDS_IN_BIT_STREAM_READY		((frame_bit_counter % (BITS_IN_WORD * 2)) == 0)
#define SUBFRAME_RECEIVED 					(frame_bit_counter == (BITRATE_BPS - 1))

boolean superframe_is_empty(void);
uint64_t superframe_get_8_bytes_from(frame_t *frame);
void bitstream_put_2_decoded_words_into(frame_t *frame);

static boolean first_edge = TRUE;
static uint32_t frame_bit_counter = 0;

superframe_t superframe = {{{{0}, 0, 0}, {{0}, 0, 0}, {{0}, 0, 0}, {{0}, 0, 0}}, 0,	0,
							superframe_is_empty,
							superframe_get_8_bytes_from};

bitstream_t bitstream = {0, 0, bitstream_put_2_decoded_words_into};

boolean superframe_is_empty(void) {
	return (superframe.tx_idx == superframe.rx_idx);
}

uint64_t superframe_get_8_bytes_from(frame_t *frame) {
	uint64_t words[4] = {0};
	uint8_t i = 0;
	for(; i < 4; i++) {
		words[i] = (uint64_t)(frame->buf[frame->tx_idx + i]);
	}
	return (words[0] << 48) | (words[1] << 32) | (words[2] << 16) | (words[3] << 0);
}

void bitstream_put_2_decoded_words_into(frame_t *frame) {
	#define BITS_31_20(u32)	(u32 & 0xFFF00000)
	#define BITS_19_8(u32)	(u32 & 0x000FFF00)
	#define BIT_31(u32)		(u32 & 0x80000000)

	uint32_t lw = BITS_31_20(bitstream.bits);
	uint32_t rw = BITS_19_8(bitstream.bits) << 12;
	uint8_t i = 0;
	for(; i < 13; i++) {
		if(BIT_31(lw) != 0) {
			frame->buf[frame->rx_idx] |= (1 << i);
		}
		if(BIT_31(rw) != 0) {
			frame->buf[frame->rx_idx + 1] |= (1 << i);
		}
		lw <<= 1;
		rw <<= 1;
	}
}


void hbp_rx_init(void) {
	edge_capture_init();
	bit_capture_init();
}


void hbp_rx_process(void) {
	while(1);
}

/*
void bitstream_put_2_raw_words_into(frame_t *frame) {
	#define BITS_31_20(u32)	(u32 & 0xFFF00000)
	#define BITS_19_8(u32)	(u32 & 0x000FFF00)

	frame->buf[frame->rx_idx++] = (uint16_t)(BITS_31_20(bitstream.bits) >> 20);
	frame->buf[frame->rx_idx++] = (uint16_t)(BITS_19_8(bitstream.bits) >> 8);
}
*/


inline void bitrate_error_handling(void) {
	btc_off();
	btc_reset();
	clear_sync_flags();
	first_edge = TRUE;
}


void ISR_edge_capture(void) {
	IfxCpu_forceDisableInterrupts();

	/* Flag must be cleared manually */
	GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = 0b1;

	if(first_edge) {
		btc_on();
		first_edge = FALSE;
	}
	else {
		uint16_t btc_value = get_btc_value();

		/* Half period */
		if((HALF_BIT_TX_PERIOD_LOWER_BOUND < btc_value) && (btc_value < HALF_BIT_TX_PERIOD_UPPER_BOUND)) {
			bitstream.bits |= 1;
		}

		/* Full period */
		else if((BIT_TX_PERIOD_LOWER_BOUND < btc_value) && (btc_value < BIT_TX_PERIOD_UPPER_BOUND)) {
				btc_reset();
				bitstream.bits <<= 1;
				bitstream.counter++;

				sync_flags_t sync_flags = sw_tracking();

				if(ALL_SYNC_FLAGS_ARE_SET) {

					#define idx_of_subframe_to_rx 	superframe.rx_idx
					#define subframe_to_rx 			superframe.subframes[idx_of_subframe_to_rx]
					#define	idx_of_word_to_rx 		subframe_to_rx.rx_idx

					if(NEW_2_WORDS_IN_BIT_STREAM_READY) {
						bitstream.put_2_decoded_words_into(&subframe_to_rx);
						idx_of_word_to_rx += 2;
					}

					if(SUBFRAME_RECEIVED) {
						idx_of_word_to_rx = 0;
						idx_of_subframe_to_rx = (idx_of_subframe_to_rx + 1) % NUM_OF_SUBFRAMES;
						frame_bit_counter = 0;
					}

					else {
						frame_bit_counter++;
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
	GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU0TC = 0b1;

	bitrate_error_handling();

	IfxCpu_enableInterrupts();
}

