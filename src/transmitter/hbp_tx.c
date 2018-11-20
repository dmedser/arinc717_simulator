/* Author: t.me/dmedser */

#include "hbp_tx.h"
#include "global_cfg.h"
#include "ports.h"
#include "pwm.h"
#include <IfxGtm_reg.h>
#include <IfxPort.h>
#include <IfxCpu.h>

#define RSH_MAX             11
#define WORD_IS_PASSED		(txd.rsh == RSH_MAX)
#define IT_WAS_LAST_WORD	(txd.num == 0)
#define BIT_TO_PASS			((txd.buf[txd.idx] >> txd.rsh) & 0x0001)

typedef struct txd_t {
	uint16_t buf[SUBFRAME_LEN_MAX];
	uint16_t idx;
	uint8_t  rsh;
} txd_t;


static uint8_t sws_idx = 0;
static txd_t txd = {{0}, 0, 0};
static uint16_t increment = 0;

static inline void txd_init(void){
	increment = 0;
	sws_idx = 0;
	txd.idx = 0;
	txd.buf[txd.idx] = sync_words[sws_idx];
	txd.rsh = 0;
}


void hbp_tx_init(void) {
	pwm_timer_init();
	txd_init();
}


inline void start_hbp_tx(void) {
	SET_PIN_HIGH(HBP_OUT);
	pwm_on();
}


inline void stop_hbp_tx(void) {
	pwm_off_reset();
	txd_init();
	SET_PIN_LOW(HBP_OUT);
}


void ISR_bit_tx(void) {
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
			if(txd.idx == 0) {
				sws_idx = (sws_idx + 1) % NUMBER_OF_SUBFRAMES;
				increment = sws_idx * (SUBFRAME_LEN - 1);
			}
			txd.buf[txd.idx] = (txd.idx == 0) ? sync_words[sws_idx] : increment++;
			txd.idx = (txd.idx + 1) % SUBFRAME_LEN;
		}
		else {
			txd.rsh++;
		}

		TOGGLE_PIN(HBP_OUT);

	}

	IfxCpu_enableInterrupts();
}

