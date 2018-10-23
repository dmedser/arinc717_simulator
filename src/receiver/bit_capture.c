/* Author: t.me/dmedser */

#include "bit_capture.h"
#include "isr_priorities.h"
#include "global_cfg.h"
#include <IfxGtm_reg.h>
#include <IfxSrc_reg.h>

void bit_capture_init(void) {
	/* Enable FXCLK */
	GTM_CMU_CLK_EN.B.EN_FXCLK = 0b10;

	GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL0 = 0b10;
	GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL0  = 0b10;
	GTM_TOM0_CH0_SR1.B.SR1               = SECOND_HALF_OF_BIT_TX_PERIOD;
	GTM_TOM0_CH0_SR0.B.SR0               = OVER_BIT_TX_PERIOD;
	GTM_TOM0_CH0_CN0.B.CN0               = 0;
	GTM_TOM0_CH0_CTRL.B.CLK_SRC_SR       = 0;

	GTM_TOM0_CH0_IRQ_EN.B.CCU1TC_IRQ_EN  = 0b1;
	GTM_TOM0_CH0_IRQ_EN.B.CCU0TC_IRQ_EN  = 0b1;

	/* Service request priority number (0 - lowest, 0xFF - highest priority) */
	MODULE_SRC.GTM.GTM[0].TOM[0][0].B.SRPN = ISR_PN_GTM_TOM0_CH0;
	/* Enable service request */
	MODULE_SRC.GTM.GTM[0].TOM[0][0].B.SRE = 0b1;
	_install_int_handler(ISR_PN_GTM_TOM0_CH0, (void (*) (int))ISR_bit_capture, 0);

	/* Apply the updates */
	GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
}


inline void btc_on(void) {
	GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL0 = 0b10;
	GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
}

inline void btc_off(void) {
	GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL0 = 0b01;
	GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
}


inline void btc_reset(void) {
	GTM_TOM0_CH0_CN0.B.CN0 = 0;
}


uint32_t btc_value(void) {
	return (uint32_t)GTM_TOM0_CH0_CN0.B.CN0;
}

