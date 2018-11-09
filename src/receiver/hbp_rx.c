/* Author: t.me/dmedser */

#include "hbp_rx.h"
#include "global_cfg.h"
#include "edge_capture.h"
#include "bit_capture.h"
#include "sync.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>
#include <Platform_Types.h>

#define ALL_SYNC_FLAGS_ARE_SET				((sync_flags & SYNC_FLAGS_MASK) == SYNC_FLAGS_MASK)
#define NEW_2_WORDS_IN_BIT_STREAM_READY		((frame_bit_counter % (BITS_IN_WORD * 2)) == 0)
#define SUBFRAME_RECEIVED 					(frame_bit_counter == (BITRATE_BPS - 1))

typedef struct frame_t {
	uint16_t buf[FRAME_LEN];
	uint16_t idx;
} frame_t;

uint32_t bit_counter = 0;
uint32_t bit_stream = 0;

static boolean first_edge = TRUE;
static uint32_t frame_bit_counter = 0;
static frame_t superframe[NUM_OF_SUBFRAMES] = {{{0}, 0}, {{0}, 0}, {{0}, 0}, {{0}, 0}};
static uint8_t rx_subframe_no = 0;

void hbp_rx_init(void) {
	edge_capture_init();
	bit_capture_init();
}


void hbp_rx_process(void) {
	while(1);
}

/*
inline void put_raw_2_words_into(frame_t *frame) {
	#define BITS_31_20(u32)	(u32 & 0xFFF00000)
	#define BITS_19_8(u32)	(u32 & 0x000FFF00)

	frame->buf[frame->idx++] = (uint16_t)(BITS_31_20(bit_stream) >> 20);
	frame->buf[frame->idx++] = (uint16_t)(BITS_19_8(bit_stream) >> 8);
}
*/

void put_decoded_2_words_into(frame_t *frame) {
	#define BITS_31_20(u32)	(u32 & 0xFFF00000)
	#define BITS_19_8(u32)	(u32 & 0x000FFF00)
	#define BIT_31(u32)		(u32 & 0x80000000)

	uint32_t lw = BITS_31_20(bit_stream);
	uint32_t rw = BITS_19_8(bit_stream) << 12;
	uint8_t i = 0;
	for(; i < 13; i++) {
		if(BIT_31(lw) != 0) {
			frame->buf[frame->idx] |= (1 << i);
		}
		if(BIT_31(rw) != 0) {
			frame->buf[frame->idx + 1] |= (1 << i);
		}
		lw <<= 1;
		rw <<= 1;
	}
	frame->idx += 2;
}


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
				bit_stream |= 1;
		}

		/* Full period */
		else if((BIT_TX_PERIOD_LOWER_BOUND < btc_value) && (btc_value < BIT_TX_PERIOD_UPPER_BOUND)) {
				btc_reset();
				bit_stream <<= 1;
				bit_counter++;

				sync_flags_t sync_flags = sw_tracking();

				if(ALL_SYNC_FLAGS_ARE_SET) {
					if(NEW_2_WORDS_IN_BIT_STREAM_READY) {
						put_decoded_2_words_into(&superframe[rx_subframe_no]);
					}
					if(SUBFRAME_RECEIVED) {
						superframe[rx_subframe_no].idx = 0;
						rx_subframe_no = (rx_subframe_no + 1) % NUM_OF_SUBFRAMES;
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

