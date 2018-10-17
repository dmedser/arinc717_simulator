/* Author: t.me/dmedser */

#include "bit_capture.h"
#include "global_cfg.h"
#include <IfxGtm_reg.h>
#include <IfxSrc_reg.h>

/* Bit capture timer measures time between two
 * adjacent edges of input signal, if it equals
 * the half of BIT_TX_PERIOD, 1 is added to the
 * encoded_bit_stream. Thus in this stream dual
 * 1 means bit 1 and single 0 means bit 0. */

uint32_t encoded_bit_stream = 0;

#define BIT_CAPTURE_TIMER_VALUE 	GTM_TOM0_CH0_CN0.B.CN0

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


inline void bit_capture_timer_on(void) {
	GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL0 = 0b10;
	GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
}


inline void bit_capture_timer_reset(void) {
	GTM_TOM0_CH0_CN0.B.CN0 = 0;
}


inline void update_encoded_bit_stream(void) {
	if(BIT_CAPTURE_TIMER_VALUE < SECOND_HALF_OF_BIT_TX_PERIOD) {
		encoded_bit_stream |= 1;
	}
	encoded_bit_stream = encoded_bit_stream << 1;
}

