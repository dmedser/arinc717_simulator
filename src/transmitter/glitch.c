/* Author: t.me/dmedser */

#include "glitch.h"
#include "isr_priorities.h"
#include "global_cfg.h"
#include <IfxGtm_reg.h>
#include <IfxSrc_reg.h>
#include <stdlib.h>
#include <stdint.h>
#include <machine/cint.h>

#define GLITCH_RESOLUTION		1000
#define GLITCH_TIME_UNIT		(bit_tx_period / GLITCH_RESOLUTION)
#define GLITCH_DURATION			(GLITCH_TIME_UNIT * 2)
#define GLITCH_START_MIN		GLITCH_DURATION
#define GLITCH_START_MAX		(bit_tx_period - (2 * GLITCH_DURATION))

static uint16_t range_rand(uint16_t range) {
	return abs(rand() % (range + 1));
}


inline void gg_update(void) {
	uint16_t raw_rand = range_rand(bit_tx_period);
	uint16_t tuned_rand = (raw_rand < GLITCH_START_MIN) ? GLITCH_START_MIN :
						  ((raw_rand > GLITCH_START_MAX) ? GLITCH_START_MAX : raw_rand);

	if(tuned_rand <= (bit_tx_period / 2)) {
		if(tuned_rand > ((bit_tx_period / 2) - (2 * GLITCH_DURATION))) {
			tuned_rand = (bit_tx_period / 2) - (2 * GLITCH_DURATION);
		}
	}
	else {
		if(tuned_rand < ((bit_tx_period / 2) + (2 * GLITCH_DURATION))) {
			tuned_rand = (bit_tx_period / 2) + (2 * GLITCH_DURATION);
		}
	}

	GTM_TOM0_CH1_SR1.B.SR1 = tuned_rand;
	GTM_TOM0_CH1_SR0.B.SR0 = tuned_rand + GLITCH_DURATION;
}


void gg_init(void) {
	/* Enable FXCLK */
	GTM_CMU_CLK_EN.B.EN_FXCLK = 0b10;

	GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL1 = 0b10;
	GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL1  = 0b10;

	gg_update();

	GTM_TOM0_CH1_CN0.B.CN0 = 0;
	GTM_TOM0_CH1_CTRL.B.CLK_SRC_SR = 0;

	GTM_TOM0_CH1_IRQ_EN.B.CCU0TC_IRQ_EN = 0b1;
	GTM_TOM0_CH1_IRQ_EN.B.CCU1TC_IRQ_EN = 0b1;

	/* Service request priority number */
	MODULE_SRC.GTM.GTM[0].TOM[0][0].B.SRPN = ISR_PN_GLITCH;
	/* Enable service request */
	MODULE_SRC.GTM.GTM[0].TOM[0][0].B.SRE = 0b1;
	_install_int_handler(ISR_PN_GLITCH, (void (*) (int))ISR_glitch, 0);

	/* Apply the updates */
	GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
}


inline void gg_on(void) {
	GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL1 = 0b10;
	GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
}


inline void gg_off_reset(void) {
	GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL1 = 0b01;
	GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
	GTM_TOM0_CH1_CN0.B.CN0 = 0;
}

