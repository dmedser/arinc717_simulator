/* Author: t.me/dmedser */

#include "hbp_rx.h"
#include "edge_capture.h"
#include "bit_capture.h"
#include "sync.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>
#include <Platform_Types.h>

#define NEW_2_WORDS_IN_BIT_STREAM_READY		((frame_bit_counter % (BITS_IN_WORD * 2)) == 0)
#define SUBFRAME_RECEIVED 					(frame_bit_counter == (BITRATE_BPS - 1))

uint32_t bit_counter = 0;
uint32_t bit_stream = 0;
static boolean first_edge = TRUE;
static boolean bit_value_1_received;
static uint32_t frame_bit_counter = 0;
static frame_t superframe[NUM_OF_SUBFRAMES] = {{{0}, 0}, {{0}, 0}, {{0}, 0}, {{0}, 0}};
static uint8_t rx_subframe_no = 0;

uint8_t subframe_busy_flags = 0;

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


void ISR_edge_capture(void) {
	IfxCpu_forceDisableInterrupts();

	/* Flag must be cleared manually */
	GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = 0b1;

	if(first_edge) {
		btc_on();
		first_edge = FALSE;
	}
	else {
		/* Edge within BIT_TX_PERIOD means that bit 1 is received */
		if(btc_value() < SECOND_HALF_OF_BIT_TX_PERIOD) {
			bit_value_1_received = TRUE;
		}
		/* Border between bits */
		else {
			btc_reset();
		}
	}

	IfxCpu_enableInterrupts();
}


void ISR_bit_capture(void) {
	IfxCpu_forceDisableInterrupts();

	/* SECOND_HALF_OF_BIT_TX_PERIOD interrupt = bit capture */
	if(GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU1TC == 0b1) {
		/* Flag must be cleared manually */
		GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU1TC = 0b1;

		if(bit_value_1_received == TRUE) {
			bit_stream |= 1;
			bit_value_1_received = FALSE;
		}

		bit_stream = bit_stream << 1;

		bit_counter++;

		/* Updates sync flags */
		sw_tracking();

		if(SYNCHRONIZED) {
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

	/* OVER_BIT_TX_PERIOD interrupt = bus disconnect */
	else if(GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU0TC == 0b1) {
		/* Flag must be cleared manually */
		GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU0TC = 0b1;

		btc_off();

		btc_reset();

		first_edge = TRUE;

		/* Sync error by bus disconnect */
		CLEAR_SYNC_FLAGS();
	}

	IfxCpu_enableInterrupts();
}

