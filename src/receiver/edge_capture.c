/* Author: t.me/dmedser */

#include "edge_capture.h"
#include "isr_priorities.h"
#include "global_cfg.h"
#include <IfxGtm_reg.h>
#include <IfxSrc_reg.h>

#define FLT_ACCEPTANCE_TIME		((1 * BIT_TX_PERIOD) / 5)

void edge_capture_init(void) {
	/* Enable CMU_CLK0
	 * CMU_CLK0 = GCLK
	 */
	GTM_CMU_CLK_EN.B.EN_CLK0 = 0b10;

	/* TIM Input Prescaler Mode */
	GTM_TIM0_CH0_CTRL.B.TIM_EN   	= 0b1;	/* Enable TIM0_0 */
	GTM_TIM0_CH0_CTRL.B.TIM_MODE 	= 0b11; /* TIPM */
	GTM_TIM0_CH0_CTRL.B.ISL     	= 0b1;  /* Ignore DSL and treat both edges as active edge */
	GTM_TIM0_CH0_CTRL.B.FLT_EN		= 0b1;	/* Enable FLT */
	GTM_TIM0_CH0_CTRL.B.FLT_CNT_FRQ = 0b00;	/* FLT_CNT counts with CMU_CLK0 */
	GTM_TIM0_CH0_CTRL.B.FLT_MODE_RE = 0b0;	/* Immediate edge propagation mode (acceptance time) */
	GTM_TIM0_CH0_CTRL.B.FLT_MODE_FE = 0b0;	/* Immediate edge propagation mode (acceptance time) */
	GTM_TIM0_CH0_CNTS.B.CNTS 		= 0;	/* After every 1 edge an TIM0_NEWVAL[x]_IRQ is raised */
	GTM_TIM0_CH0_FLT_RE.B.FLT_RE 	= FLT_ACCEPTANCE_TIME;
	GTM_TIM0_CH0_FLT_FE.B.FLT_FE 	= FLT_ACCEPTANCE_TIME;

	GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = 0b1;

	/* Service request priority number (0 - lowest, 0xFF - highest priority) */
	MODULE_SRC.GTM.GTM[0].TIM[0][0].B.SRPN = ISR_PN_GTM_TIM0_CH0;
	/* Enable service request */
	MODULE_SRC.GTM.GTM[0].TIM[0][0].B.SRE = 0b1;
	_install_int_handler(ISR_PN_GTM_TIM0_CH0, (void (*) (int))ISR_edge_capture, 0);

}

