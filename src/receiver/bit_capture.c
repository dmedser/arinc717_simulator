/* Author: t.me/dmedser */

#include "bit_capture.h"
#include "global_cfg.h"
#include <IfxGtm_reg.h>
#include <IfxSrc_reg.h>

void bit_capture_init(void) {
	/* Enable FXCLK */
	GTM_CMU_CLK_EN.B.EN_FXCLK = 0b10;

	GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL0 = 0b10;
	GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL0  = 0b10;
	GTM_TOM0_CH0_SR0.B.SR0               = OVER_BIT_TX_PERIOD;
	GTM_TOM0_CH0_CN0.B.CN0               = 0;
	GTM_TOM0_CH0_CTRL.B.CLK_SRC_SR       = 0;

	/* Apply the updates */
	GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
}


inline uint16_t	get_bit_capture_timer_value(void) {
	return (uint16_t)GTM_TOM0_CH0_CN0.B.CN0;
}


inline void bit_capture_timer_on(void) {
	GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL0 = 0b10;
	GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
}


inline void bit_capture_timer_reset(void) {
	GTM_TOM0_CH0_CN0.B.CN0 = 0;
}

