/* Author: t.me/dmedser */

#include "edge_capture.h"
#include "isr_priorities.h"
#include "global_cfg.h"
#include <IfxGtm_reg.h>
#include <IfxSrc_reg.h>
#include <machine/cint.h>

#define DEGLITCH_TIME_PERCENT		10
#define	DEGLITCH_TIME_ABSOLUTE		(BIT_TX_PERIOD / 100) * DEGLITCH_TIME_PERCENT

void edge_capture_init(void) {
	/* Enable CMU_CLK0
	 * CMU_CLK0 = GCLK
	 */
	GTM_CMU_CLK_EN.B.EN_CLK0 = 0b10;

	/* Generates NEWVAL_IRQ_EN on each edge excluding glitches */
	GTM_TIM0_CH0_CTRL.B.TIM_EN   	= 0b1;		/* Enable TIM0_0 */
	GTM_TIM0_CH0_CTRL.B.TIM_MODE 	= 0b010;	/* TIM Input Event Mode (TIEM) */
	GTM_TIM0_CH0_CTRL.B.ISL     	= 0b1;		/* Ignore DSL and treat both edges as active edge */
	GTM_TIM0_CH0_CTRL.B.FLT_EN		= 0b1;		/* Enable FLT */
	GTM_TIM0_CH0_CTRL.B.FLT_CNT_FRQ = 0b00;		/* FLT_CNT counts with CMU_CLK0 = 50 MHz */
	GTM_TIM0_CH0_CTRL.B.FLT_MODE_RE = 0b1;		/* Individual de-glitch mode */
	GTM_TIM0_CH0_CTRL.B.FLT_MODE_FE = 0b1;		/* Individual de-glitch mode */
	GTM_TIM0_CH0_CTRL.B.FLT_CTR_RE  = 0b0;		/* Up/down counter */
	GTM_TIM0_CH0_CTRL.B.FLT_CTR_FE  = 0b0;		/* Up/down counter */

	GTM_TIM0_CH0_FLT_RE.B.FLT_RE 	= DEGLITCH_TIME_ABSOLUTE;
	GTM_TIM0_CH0_FLT_FE.B.FLT_FE 	= DEGLITCH_TIME_ABSOLUTE;

	GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = 0b1;

	/* Service request priority number */
	MODULE_SRC.GTM.GTM[0].TIM[0][0].B.SRPN = ISR_PN_EDGE_CAPTURE;
	/* Enable service request */
	MODULE_SRC.GTM.GTM[0].TIM[0][0].B.SRE = 0b1;
	_install_int_handler(ISR_PN_EDGE_CAPTURE, (void (*) (int))ISR_edge_capture, 0);
}

