/* Author: t.me/dmedser */

#include "hbp_rx.h"
#include "edge_capture.h"
#include "bit_capture.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>
#include <Platform_Types.h>

#define SW1_ENCODED			0xFC660000
#define SW2_ENCODED			0b1FBD8000
#define SW1_ENCODE_MASK		0xFFFF8000
#define SW2_ENCODE_MASK		0xFFFFC000

extern uint32_t cnt_1_ms;
boolean	first_edge_captured = FALSE;
uint32_t sliding_periods = 0;

void hbp_rx_init(void) {
	edge_capture_init();
	bit_capture_init();
}


void hbp_rx_process(void) {
	while(1);
}


/* TEST */
uint32_t TEST_match_cnt = 0;

void ISR_edge_capture(void) {
	IfxCpu_forceDisableInterrupts();

	if(!first_edge_captured) {
		bit_capture_timer_on();
		first_edge_captured = TRUE;
	}
	else {
		if(get_bit_capture_timer_value() < SECOND_HALF_OF_BIT_TX_PERIOD) {
			sliding_periods |= 1;
		}
	}

	sliding_periods = sliding_periods << 1;
	bit_capture_timer_reset();

	if((sliding_periods & SW1_ENCODE_MASK) == SW1_ENCODED) {
		TEST_match_cnt++;
	}

	/* Flag must be cleared manually */
	GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = 0b1;

	IfxCpu_enableInterrupts();
}

