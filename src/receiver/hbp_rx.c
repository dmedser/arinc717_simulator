/* Author: t.me/dmedser */

#include "hbp_rx.h"
#include "edge_capture.h"
#include "bit_capture.h"
#include <IfxGtm_reg.h>
#include <IfxCpu.h>
#include <Platform_Types.h>

#define SW1_ENCODED					0xFC660000
#define SW2_ENCODED					0x1FBD8000
#define SW3_ENCODED
#define SW4_ENCODED

#define SW1_ENCODE_MASK				0xFFFF8000
#define SW2_ENCODE_MASK				0xFFFFC000
#define SW3_ENCODE_MASK
#define SW4_ENCODE_MASK


typedef enum sync_states {
	IDLE,
	CATCH_SW12,
	CATCH_SW23,
	CATCH_SW34,
	CATCH_SW41
} sync_states;

extern uint32_t cnt_1_ms;
static uint32_t timestamp_ms = 0;
static uint32_t delta = 0;
static boolean	first_edge_captured = FALSE;
static uint32_t sliding_periods = 0;
static sync_states sync_state = IDLE;


void hbp_rx_init(void) {
	edge_capture_init();
	bit_capture_init();
}


void hbp_rx_process(void) {
	while(1);
}


void update_sliding_periods(void) {
	if(first_edge_captured) {
		if(get_bit_capture_timer_value() < SECOND_HALF_OF_BIT_TX_PERIOD) {
			sliding_periods |= 1;
		}
	}
	else {
		bit_capture_timer_on();
		first_edge_captured = TRUE;
	}
	sliding_periods = sliding_periods << 1;
	bit_capture_timer_reset();
}


void bit_stream_synchronization(void) {
	switch(sync_state){
	case IDLE:
		if((sliding_periods & SW1_ENCODE_MASK) == SW1_ENCODED) {
			timestamp_ms = cnt_1_ms;
			sync_state = CATCH_SW12;
		}
		//else if((sliding_periods & SW2_ENCODE_MASK) == SW2_ENCODED) {
		//	sync_state = CATCH_SW23;
		//}
		break;
	case CATCH_SW12:

		break;
	case CATCH_SW23:
		break;
	case CATCH_SW34:
		break;
	case CATCH_SW41:
		break;
	}
}


void ISR_edge_capture(void) {
	IfxCpu_forceDisableInterrupts();

	update_sliding_periods();

	bit_stream_synchronization();

	/* Flag must be cleared manually */
	GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = 0b1;

	IfxCpu_enableInterrupts();
}

