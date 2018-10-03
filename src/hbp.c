#include "hbp.h"
#include "ports.h"
#include "pwm.h"
#include <IfxGtm.h>
#include <IfxPort.h>

#define RSH_MAX				11
#define WORD_IS_PASSED		(txd.rsh == RSH_MAX)
#define BIT_TO_PASS			((txd.buf[txd.idx] >> txd.rsh) & 0x0001)

tx_buf txd = {{0}, 0, 0, 0, FALSE};

void start_hbp_tx(void) {
	txd.busy = TRUE;
	SET_PIN_HIGH(HBP1IN);
	pwm_on();
}

inline void stop_hbp_tx(void) {
	pwm_off_reset();
	SET_PIN_LOW(HBP1IN);
}

void hbp_tx(void) {
	start_hbp_tx();
	while(txd.busy);
}


void ISR_GTM_TOM0_12_cmp_match(void) {
	/* Half-period */
	if(GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU1TC == 0b1) {
		GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU1TC = 0b1;
		if(BIT_TO_PASS == 1) {
			TOGGLE_PIN(HBP1IN);
		}
	}

	/* Period */
	if(GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU0TC == 0b1) {
		GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU0TC = 0b1;

		if(WORD_IS_PASSED) {
			txd.rsh = 0;
			txd.num--;
			if(txd.num == 0) {
				stop_hbp_tx();
				txd.busy = FALSE;
				txd.idx = 0;
			}
			else {
				txd.idx++;
			}
		}
		else {
			TOGGLE_PIN(HBP1IN);
			txd.rsh++;
		}
	}
}
