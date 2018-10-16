/* Author: t.me/dmedser */

#include "hbp_rx.h"
#include "edge_capture.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>

#define SW1_ENCODED			0xFC660000
#define SW2_ENCODED			0b1FBD8000
#define SW1_ENCODE_MASK		0xFFFF8000
#define SW2_ENCODE_MASK		0xFFFFC000


void hbp_rx_init(void) {
	edge_capture_init();
}


void hbp_rx_process(void) {
	while(1);
}


void ISR_edge_capture(void) {
	IfxCpu_forceDisableInterrupts();



	/* Flag must be cleared manually */
	GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = 0b1;

	IfxCpu_enableInterrupts();
}

