/* Author: t.me/dmedser */

#include "bit_capture.h"
#include "isr_priorities.h"
#include <IfxGtm_reg.h>
#include <IfxSrc_reg.h>
#include <machine/cint.h>

void bit_capture_init(void) {
	/* Enable FXCLK */
	GTM_CMU_CLK_EN.B.EN_FXCLK = 0b10;

	GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL0 = 0b10;
	GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL0  = 0b10;
	GTM_TOM0_CH0_SR0.B.SR0               = BIT_TX_PERIOD_UPPER_BOUND;
	GTM_TOM0_CH0_CN0.B.CN0               = 0;
	GTM_TOM0_CH0_CTRL.B.CLK_SRC_SR       = 0;

	GTM_TOM0_CH0_IRQ_EN.B.CCU0TC_IRQ_EN  = 0b1;

	/* Service request priority number */
	MODULE_SRC.GTM.GTM[0].TOM[0][0].B.SRPN = ISR_PN_BIT_TX_TIMEOUT;
	/* Enable service request */
	MODULE_SRC.GTM.GTM[0].TOM[0][0].B.SRE = 0b1;
	_install_int_handler(ISR_PN_BIT_TX_TIMEOUT, (void (*) (int))ISR_bit_tx_timeout, 0);

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


uint16_t get_btc_value(void) {
	return (uint16_t)GTM_TOM0_CH0_CN0.B.CN0;
}

