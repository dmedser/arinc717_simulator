/* Author: t.me/dmedser */

#include "hbp_rx.h"
#include "edge_bit_capture.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>

#define LSH_MAX			11
#define WORD_IS_PASSED	(rxd.lsh == LSH_MAX)


void hbp_rx_init(void) {
	edge_bit_capture_init();
}


void hbp_rx_process(void) {
	while(1);
}


#define SW1_ENCODED			0b11111100011001100000000000000000
#define SW2_ENCODED			0b00011111101111011000000000000000

#define SW1_ENCODE_MASK		0xFFFF8000
#define SW2_ENCODE_MASK		0xFFFFC000


void ISR_edge_capture(void) {

	IfxCpu_forceDisableInterrupts();


	/* Flag must be cleared manually */
	GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = 0b1;

	IfxCpu_enableInterrupts();

}

