/* Author: t.me/dmedser */

#include "pwm.h"
#include "isr_priorities.h"
#include "global_cfg.h"
#include <IfxGtm_reg.h>
#include <IfxSrc_reg.h>
#include <machine/cint.h>

#define PWM_PERIOD	bit_tx_period

void pwm_timer_init(void) {
	/* Enable FXCLK */
	GTM_CMU_CLK_EN.B.EN_FXCLK = 0b10;

	GTM_TOM0_TGC1_FUPD_CTRL.B.FUPD_CTRL4 = 0b10;
	GTM_TOM0_TGC1_GLB_CTRL.B.UPEN_CTRL4  = 0b10;
	GTM_TOM0_CH12_SR0.B.SR0 = PWM_PERIOD;
	GTM_TOM0_CH12_SR1.B.SR1 = PWM_PERIOD / 2;
	GTM_TOM0_CH12_CN0.B.CN0 = 0;
	GTM_TOM0_CH12_CTRL.B.CLK_SRC_SR = 0;

	GTM_TOM0_CH12_IRQ_EN.B.CCU0TC_IRQ_EN = 0b1;
	GTM_TOM0_CH12_IRQ_EN.B.CCU1TC_IRQ_EN = 0b1;

	/* Service request priority number */
	MODULE_SRC.GTM.GTM[0].TOM[0][6].B.SRPN = ISR_PN_BIT_TX_PWM;
	/* Enable service request */
	MODULE_SRC.GTM.GTM[0].TOM[0][6].B.SRE = 0b1;
	_install_int_handler(ISR_PN_BIT_TX_PWM, (void (*) (int))ISR_bit_tx, 0);

	/* Apply the updates */
	GTM_TOM0_TGC1_GLB_CTRL.B.HOST_TRIG = 0b1;
}


inline void pwm_timer_update(void) {
	GTM_TOM0_CH12_SR0.B.SR0 = PWM_PERIOD;
	GTM_TOM0_CH12_SR1.B.SR1 = PWM_PERIOD / 2;
}


inline void pwm_on(void) {
	GTM_TOM0_TGC1_ENDIS_CTRL.B.ENDIS_CTRL4 = 0b10;
	GTM_TOM0_TGC1_GLB_CTRL.B.HOST_TRIG = 0b1;
}


inline void pwm_off_reset(void) {
	GTM_TOM0_TGC1_ENDIS_CTRL.B.ENDIS_CTRL4 = 0b01;
	GTM_TOM0_TGC1_GLB_CTRL.B.HOST_TRIG = 0b1;
	GTM_TOM0_CH12_CN0.B.CN0 = 0;
}

