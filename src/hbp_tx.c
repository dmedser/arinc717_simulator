#include "hbp_tx.h"
#include "ports.h"
#include "pwm.h"
#include <IfxGtm_reg.h>
#include <IfxPort.h>
#include <IfxCpu.h>

#define RSH_MAX				11
#define WORD_IS_PASSED		(txd.rsh == RSH_MAX)
#define IT_WAS_LAST_WORD	(txd.num == 0)
#define BIT_TO_PASS			((txd.buf[txd.idx] >> txd.rsh) & 0x0001)

tx_buf txd = {{0}, 0, 0, 0, FALSE};

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