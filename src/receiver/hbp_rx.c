/* Author: t.me/dmedser */

#include "hbp_rx.h"
#include "edge_capture.h"
#include "bit_capture.h"
#include "sync.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>
#include <Platform_Types.h>

static boolean first_edge = TRUE;

static boolean bit_value_1_received;

uint32_t bit_counter = 0;

uint32_t bit_stream = 0;

static uint32_t frame_bit_counter = 0;

static frame_t superframe[NUM_OF_SUBFRAMES] = {{{0}, 0}, {{0}, 0}, {{0}, 0}, {{0}, 0}};

static uint8_t subframe_no = 0;

void hbp_rx_init(void) {
	edge_capture_init();
	bit_capture_init();
}


void hbp_rx_process(void) {
	while(1);
}

/*
inline void put_raw_2_words_into(frame_t *frame) {
	frame->buf[frame->idx++] = (uint16_t)((bit_stream & 0xFFF00000) >> 20);
	frame->buf[frame->idx++] = (uint16_t)((bit_stream & 0x000FFF00) >> 8);
}
*/

void put_decoded_2_words_into(frame_t *frame) {
	uint32_t lw = (bit_stream & 0xFFF00000);
	uint32_t rw = (bit_stream & 0x000FFF00) << 12;
	uint8_t i = 0;
	for(; i < 13; i++) {
		if((lw & 0x80000000) != 0) {
			frame->buf[frame->idx] |= (1 << i);
		}
		if((rw & 0x80000000) != 0) {
			frame->buf[frame->idx + 1] |= (1 << i);
		}
		lw <<= 1;
		rw <<= 1;
	}
	frame->idx += 2;
}


void ISR_edge_capture(void) {
	IfxCpu_forceDisableInterrupts();

	if(first_edge) {
		btc_on();
		first_edge = FALSE;
	}
	else {
		if(btc_value() < SECOND_HALF_OF_BIT_TX_PERIOD) {
			bit_value_1_received = TRUE;
		}
		else {
			btc_reset();
		}
	}

	/* Flag must be cleared manually */
	GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = 0b1;

	IfxCpu_enableInterrupts();
}


void ISR_bit_capture(void) {
	IfxCpu_forceDisableInterrupts();

	/* Bit capture interrupt */
	if(GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU1TC == 0b1) {
		/* Flag must be cleared manually */
		GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU1TC = 0b1;

		CLEAR_SYNC_FLAG(BUS_DISCONNECT_FLAG);

		if(bit_value_1_received == TRUE) {
			bit_stream |= 1;
			bit_value_1_received = FALSE;
		}

		bit_stream = bit_stream << 1;

		bit_counter++;

		sws_tracking();

		if(SYNCHRONIZED) {
			if(frame_bit_counter % (BITS_IN_WORD * 2) == 0) {
				put_decoded_2_words_into(&superframe[subframe_no]);
			}
			frame_bit_counter++;
			if(frame_bit_counter == BITRATE_BPS) {
				superframe[subframe_no].idx = 0;
				subframe_no = (subframe_no + 1) % NUM_OF_SUBFRAMES;
				frame_bit_counter = 0;
			}
		}

	}

	/* Over BIT_TX_PERIOD interrupt = bus disconnect */
	else if(GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU0TC == 0b1) {
		/* Flag must be cleared manually */
		GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU0TC = 0b1;

		btc_off();

		btc_reset();

		first_edge = TRUE;

		SET_SYNC_FLAG(BUS_DISCONNECT_FLAG);

		CLEAR_SYNC_FLAGS(SUCCESS_FLAGS);
	}

	IfxCpu_enableInterrupts();
}

