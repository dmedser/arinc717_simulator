/* Author: t.me/dmedser */

#include "hbp_rx.h"
#include "edge_capture.h"
#include "bit_capture.h"
#include "sync.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>
#include <Platform_Types.h>

boolean first_edge_captured = FALSE;

void hbp_rx_init(void) {
	edge_capture_init();
	bit_capture_init();
}


void hbp_rx_process(void) {
	while(1);
}


void ISR_edge_capture(void) {
	IfxCpu_forceDisableInterrupts();

	if(first_edge_captured) {
		update_encoded_bit_stream();
		bit_capture_timer_reset();
		sync_process();
	}
	else {
		first_edge_captured = TRUE;
		bit_capture_timer_on();
	}

	/* Flag must be cleared manually */
	GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = 0b1;

	IfxCpu_enableInterrupts();
}

