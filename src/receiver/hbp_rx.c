/* Author: t.me/dmedser */

#include "hbp_rx.h"
#include "edge_capture.h"
#include "bit_capture.h"
#include "sync.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>
#include <Platform_Types.h>

#define ON		TRUE
#define OFF		FALSE

typedef boolean switch_action;

static switch_action bit_capture_timer_state = OFF;

boolean bit_value_1_received;

uint32_t bit_counter = 0;

uint32_t bit_stream = 0;

void hbp_rx_init(void) {
	edge_capture_init();
	bit_capture_init();
}


void hbp_rx_process(void) {
	while(1);
}


void ISR_edge_capture(void) {
	IfxCpu_forceDisableInterrupts();

	if(bit_capture_timer_state == OFF) {
		bit_capture_timer_on();
		bit_capture_timer_state = ON;
	}
	else {
		if(bit_capture_timer_value() < SECOND_HALF_OF_BIT_TX_PERIOD) {
			bit_value_1_received = TRUE;
		}
	}

	/* Flag must be cleared manually */
	GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = 0b1;

	IfxCpu_enableInterrupts();
}


void ISR_bit_capture(void) {
	IfxCpu_forceDisableInterrupts();

	bit_capture_timer_off_reset();
	bit_capture_timer_state = OFF;

	if(bit_value_1_received == TRUE) {
		bit_stream |= 1;
		bit_value_1_received = FALSE;
	}

	bit_stream = bit_stream << 1;

	bit_counter++;

	sync_process();

	/* Flag must be cleared manually */
	GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU0TC = 0b1;
	IfxCpu_enableInterrupts();
}

