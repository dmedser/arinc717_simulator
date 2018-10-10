#include "hbp_rx.h"
#include "ports.h"
#include "edge_bit_capture.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>
#include <IfxPort.h>

#define LSH_MAX			11
#define WORD_IS_PASSED	(rxd.lsh == LSH_MAX)

static rx_buf rxd = {{0}, 0, 0, FALSE};
static boolean bit_val_1_received = FALSE;


void hbp_rx_init(void) {
	edge_bit_capture_init();
	rx_ports_init();
}


void hbp_rx_process(void) {
	while(1);
}


void ISR_edge_capture(void) {

	IfxCpu_forceDisableInterrupts();

	if(rxd.busy == FALSE) {
		bit_capture_timer_on();
		rxd.busy = TRUE;
	}
	else {
		if(BIT_CAPTURE_TIMER_VALUE < SECOND_HALF_OF_BIT_TX_PERIOD) {
			bit_val_1_received = TRUE;
		}
		else {
			bit_capture_timer_reset();
		}
	}

	/* Flag must be cleared manually */
	GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = 0b1;

	IfxCpu_enableInterrupts();

}


void ISR_bit_capture(void) {

	IfxCpu_forceDisableInterrupts();

	/* Second half of bit tx period */
	if(GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU1TC == 1) {
		GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU1TC = 0b1;

		if(bit_val_1_received) {
			rxd.buf[rxd.idx] |= (1 << rxd.lsh);
			bit_val_1_received = FALSE;
		}

		if(WORD_IS_PASSED) {
			rxd.lsh = 0;
			rxd.idx++;
		}
		else {
			rxd.lsh++;
		}
	}

	/* Over bit tx period */
	else if(GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU0TC == 1) {
		GTM_TOM0_CH0_IRQ_NOTIFY.B.CCU0TC = 0b1;

		bit_capture_timer_off();
		bit_capture_timer_reset();

		rxd.lsh = 0;
		rxd.busy = FALSE;
	}

	IfxCpu_enableInterrupts();
}
