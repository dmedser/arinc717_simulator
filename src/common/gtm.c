/* Author: t.me/dmedser */

#include "gtm.h"
#include <IfxGtm.h>
#include <IfxGtm_Cmu.h>

#define GCLK_FREQUENCY	40000000 /* 40 MHz */

/* fGCLK  = (fSRC / GTMDIV) * (GCLK_DEN / GCLK_NUM)
 * fSRC   by default = fPLL = IFX_CFG_SCU_PLL_FREQUENCY = 200 MHz
 * GTMDIV by default = 2
 * GCLK_DEN необходимо принять за числитель, а GCLK_NUM за знаменатель
 * Проектная частота fGCLK = GCLK_FREQUENCY */

uint32_t f_gclk = 0;

void gtm_init(void) {
	Ifx_GTM *gtm = &MODULE_GTM;

	IfxGtm_enable(gtm);

	IfxGtm_Cmu_setGclkFrequency(gtm, GCLK_FREQUENCY);

	f_gclk = (uint32_t)IfxGtm_Cmu_getGclkFrequency(gtm);
}

