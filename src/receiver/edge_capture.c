/* Author: t.me/dmedser */

#include "edge_capture.h"
#include "isr_priorities.h"
#include "global_cfg.h"
#include <IfxGtm_reg.h>
#include <IfxSrc_reg.h>
#include <machine/cint.h>

#define DEGLITCH_TIME_PERCENT   10
#define	DEGLITCH_TIME_ABSOLUTE  ((bit_tx_period / 100) * DEGLITCH_TIME_PERCENT)

/* Таймер захвата фронтов генерирует  NEWVAL  прерывание на каждый
 * фронт  (положительный  и  отрицательный) входящего  HBP сигнала,
 * который  предварительно  пропускается   через   антидребезговый
 * фильтр, разрешающая способность которого определяется величиной
 * DEGLITCH_TIME_ABSOLUTE
 */

void edge_capture_timer_init(void) {
	GTM_CMU_CLK_EN.B.EN_CLK0        = 0b10;     /* Enable CMU_CLK0 = GCLK */
	GTM_TIM0_CH4_CTRL.B.CLK_SEL     = 0b000;    /* Clock source for channel = CMU_CLK0 */
	GTM_TIM0_CH4_CTRL.B.TIM_EN      = 0b1;      /* Enable TIM0_4 */
	GTM_TIM0_CH4_CTRL.B.TIM_MODE    = 0b010;    /* TIM Input Event Mode (TIEM) */
	GTM_TIM0_CH4_CTRL.B.ISL         = 0b1;      /* Ignore DSL and treat both edges as active edge */
	GTM_TIM0_CH4_CTRL.B.FLT_EN      = 0b1;      /* Enable FLT */
	GTM_TIM0_CH4_CTRL.B.FLT_CNT_FRQ = 0b00;     /* FLT_CNT counts with CMU_CLK0 = 50 MHz */
	GTM_TIM0_CH4_CTRL.B.FLT_MODE_RE = 0b1;      /* Individual de-glitch mode */
	GTM_TIM0_CH4_CTRL.B.FLT_MODE_FE = 0b1;      /* Individual de-glitch mode */
	GTM_TIM0_CH4_CTRL.B.FLT_CTR_RE  = 0b0;      /* Up/down counter */
	GTM_TIM0_CH4_CTRL.B.FLT_CTR_FE  = 0b0;      /* Up/down counter */

	GTM_TIM0_CH4_FLT_RE.B.FLT_RE = DEGLITCH_TIME_ABSOLUTE;
	GTM_TIM0_CH4_FLT_FE.B.FLT_FE = DEGLITCH_TIME_ABSOLUTE;

	GTM_TIM0_CH4_IRQ_EN.B.NEWVAL_IRQ_EN = 0b1;

	MODULE_SRC.GTM.GTM[0].TIM[0][4].B.SRPN = ISR_PN_EDGE_CAPTURE;
	MODULE_SRC.GTM.GTM[0].TIM[0][4].B.SRE  = 0b1;
	_install_int_handler(ISR_PN_EDGE_CAPTURE, (void (*) (int))ISR_edge_capture, 0);
}


inline void edge_capture_timer_update(void) {
	GTM_TIM0_CH4_FLT_RE.B.FLT_RE = DEGLITCH_TIME_ABSOLUTE;
	GTM_TIM0_CH4_FLT_FE.B.FLT_FE = DEGLITCH_TIME_ABSOLUTE;
}

