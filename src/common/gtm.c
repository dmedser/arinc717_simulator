/* Author: t.me/dmedser */

#include "gtm.h"
#include <IfxGtm.h>
#include <IfxScuCcu.h>

/* fGTM  = fPLL / GTMDIV
 * fPLL = 200 MHz
 * Target fGTM = 50 MHz */

uint32_t f_gtm = 0;

void gtm_init(void) {
	Ifx_GTM *gtm = &MODULE_GTM;
	IfxGtm_enable(gtm);

	Ifx_SCU_CCUCON1 ccucon1 = SCU_CCUCON1;
	uint16_t password = IfxScuWdt_getSafetyWatchdogPassword();

	/* Disable TRAP for SMU (oscillator watchdog and unlock detection) */
	IfxScuWdt_clearSafetyEndinit(password);

	ccucon1.B.GTMDIV = 4U;
	ccucon1.B.UP     = 1U;
	SCU_CCUCON1.U    = ccucon1.U;

	IfxScuWdt_setSafetyEndinit(password);

	f_gtm = IfxScuCcu_getGtmFrequency();
}
