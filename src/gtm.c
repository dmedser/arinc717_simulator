/* Author: t.me/dmedser */

#include "gtm.h"
#include "global_cfg.h"
#include <IfxGtm.h>
#include <IfxScuCcu.h>

/* fGTM = fPLL / GTMDIV / (GCLK_NUM / GCLK_DEN)
 * fPLL = 200 MHz
 * By default GTMDIV = 2
 * Target fGTM = 100 MHz */

void gtm_init(void) {
	Ifx_GTM *gtm = &MODULE_GTM;
	IfxGtm_enable(gtm);

	/* Global divider = 1 */
	GTM_CMU_GCLK_NUM.B.GCLK_NUM = 1;
	GTM_CMU_GCLK_DEN.B.GCLK_DEN = 1;

	f_gtm = IfxScuCcu_getGtmFrequency();
}
