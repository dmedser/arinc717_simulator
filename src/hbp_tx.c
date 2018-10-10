/* Author: t.me/dmedser */

#include "hbp_tx.h"
#include "ports.h"
#include "pwm.h"
#include "rtos.h"
#include "MulticanBasic.h"
#include <IfxGtm_reg.h>
#include <IfxPort.h>
#include <IfxCpu.h>
#include "dd_ram.h"

#define RSH_MAX			11
#define WORD_IS_PASSED		(txd.rsh == RSH_MAX)
#define IT_WAS_LAST_WORD	(txd.num == 0)
#define BIT_TO_PASS		((txd.buf[txd.idx] >> txd.rsh) & 0x0001)

static tx_buf txd = {{0}, 0, 0, 0, FALSE};

void hbp_tx_init(void) {
	pwm_init();
	tx_ports_init();
	rtos_init();
	MulticanBasic_init();
}


void start_hbp_tx(void) {
	txd.busy = TRUE;
	SET_PIN_HIGH(HBP_OUT);
	pwm_on();
}


inline void stop_hbp_tx(void) {
	pwm_off_reset();
	SET_PIN_LOW(HBP_OUT);
}


void hbp_tx(void) {
	start_hbp_tx();
	while(txd.busy);
}


void hbp_tx_process(void) {
	txd.buf[0] = 0b101001110101;
	txd.buf[1] = 0b111000111000;
	txd.num = 2;

	while(1) {
		if(CAN_InBox_Buffer8_data[0] == 0xFF) {
			CAN_InBox_Buffer8_data[0] = 0;
			hbp_tx();
			txd.num = 2;
		}
	}
}


void ISR_GTM_TOM0_CH12_cmp_match(void) {

	IfxCpu_forceDisableInterrupts();

	/* Half-period */
	if(GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU1TC == 0b1) {
		GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU1TC = 0b1;
		if(BIT_TO_PASS == 1) {
			TOGGLE_PIN(HBP_OUT);
		}
	}

	/* Period */
	if(GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU0TC == 0b1) {
		GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU0TC = 0b1;

		if(WORD_IS_PASSED) {
			txd.rsh = 0;
			txd.num--;
			if(IT_WAS_LAST_WORD) {
				stop_hbp_tx();
				txd.busy = FALSE;
				txd.idx = 0;
			}
			else {
				TOGGLE_PIN(HBP_OUT);
				txd.idx++;
			}
		}
		else {
			TOGGLE_PIN(HBP_OUT);
			txd.rsh++;
		}
	}

	IfxCpu_enableInterrupts();
}
