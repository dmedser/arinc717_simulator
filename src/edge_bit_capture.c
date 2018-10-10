/* Author: t.me/dmedser */

#include "edge_bit_capture.h"
#include "gtm.h"
#include "isr_priorities.h"
#include <IfxGtm.h>
#include <IfxSrc_reg.h>

void edge_bit_capture_init(void) {

	gtm_init();

	/* Edge capture timer */
	{
		/* Enable CMU_CLK0
		 * CMU_CLK0 = GCLK
		 */
		GTM_CMU_CLK_EN.B.EN_CLK0 = 0b10;

		/* TIM Input Prescaler Mode */
		GTM_TIM0_CH0_CTRL.B.TIM_EN   	= 0b1;	/* Enable TIM0_0 */
		GTM_TIM0_CH0_CTRL.B.TIM_MODE 	= 0b11; /* TIPM */
		GTM_TIM0_CH0_CTRL.B.ISL     	= 0b1;  /* Ignore DSL and treat both edges as active edge */
		GTM_TIM0_CH0_CTRL.B.FLT_EN	= 0b1;	/* Enable FLT */
		GTM_TIM0_CH0_CTRL.B.FLT_CNT_FRQ = 0b00;	/* FLT_CNT counts with CMU_CLK0 */
		GTM_TIM0_CH0_CTRL.B.FLT_MODE_RE = 0b0;	/* Immediate edge propagation mode (acceptance time) */
		GTM_TIM0_CH0_CTRL.B.FLT_MODE_FE = 0b0;	/* Immediate edge propagation mode (acceptance time) */
		GTM_TIM0_CH0_CNTS.B.CNTS 	= 0;	/* After 1 edge an TIM0_NEWVAL[x]_IRQ is raised */
		GTM_TIM0_CH0_FLT_RE.B.FLT_RE 	= FLT_ACCEPTANCE_TIME;
		GTM_TIM0_CH0_FLT_FE.B.FLT_FE 	= FLT_ACCEPTANCE_TIME;

		GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = 0b1;

		/* Service request priority number (0 - lowest, 0xFF - highest priority) */
		MODULE_SRC.GTM.GTM[0].TIM[0][0].B.SRPN = ISR_PN_GTM_TIM0_CH0;
		/* Enable service request */
		MODULE_SRC.GTM.GTM[0].TIM[0][0].B.SRE = 0b1;
		_install_int_handler(ISR_PN_GTM_TIM0_CH0, (void (*) (int))ISR_edge_capture, 0);
	}

	/* Bit capture timer */
	{
		/* Enable FXCLK */
		GTM_CMU_CLK_EN.B.EN_FXCLK = 0b10;

		GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL0 = 0b10;
		GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL0  = 0b10;
		GTM_TOM0_CH0_SR1.B.SR1               = SECOND_HALF_OF_BIT_TX_PERIOD;
		GTM_TOM0_CH0_SR0.B.SR0               = OVER_BIT_TX_PERIOD;
		GTM_TOM0_CH0_CN0.B.CN0               = 0;
		GTM_TOM0_CH0_CTRL.B.CLK_SRC_SR       = 0;

		GTM_TOM0_CH0_IRQ_EN.B.CCU1TC_IRQ_EN = 0b1;
		GTM_TOM0_CH0_IRQ_EN.B.CCU0TC_IRQ_EN = 0b1;

		/* Service request priority number (0 - lowest, 0xFF - highest priority) */
		MODULE_SRC.GTM.GTM[0].TOM[0][0].B.SRPN = ISR_PN_GTM_TOM0_CH0;
		/* Enable service request */
		MODULE_SRC.GTM.GTM[0].TOM[0][0].B.SRE = 0b1;
		_install_int_handler(ISR_PN_GTM_TOM0_CH0, (void (*) (int))ISR_bit_capture, 0);

		/* Apply the updates */
		GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
	}
}


inline void bit_capture_timer_on(void) {
	GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL0 = 0b10;
	GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
}


inline void bit_capture_timer_off(void) {
	GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL0 = 0b01;
	GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 0b1;
}


inline void bit_capture_timer_reset(void) {
	GTM_TOM0_CH0_CN0.B.CN0 = 0;
}

