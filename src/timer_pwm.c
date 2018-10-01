#include "timer_pwm.h"
#include <IfxScuWdt.h>
#include <IfxScu_reg.h>
#include <IfxGtm.h>
#include <IfxGtm_PinMap.h>
#include <IfxPort_Io.h>
#include <IfxPort_PinMap.h>

#define BITRATE_BPS	768
// 768   bps = 64   wps
// 1536  bps = 128  wps
// 3072  bps = 256  wps
// 6144  bps = 512  wps
// 12288 bps = 1024 wps

#define PWM_PERIOD	((uint16_t)(IfxScuCcu_getGtmFrequency() / (BITRATE_BPS * 2)))

void timer_pwm_init(void) {
	/* fGTM = fPLL / 2 = 100 MHz */

	Ifx_GTM *gtm = &MODULE_GTM;
	IfxGtm_enable(gtm);

	/* Global divider = 1 */
	GTM_CMU_GCLK_NUM.B.GCLK_NUM = 1;
	GTM_CMU_GCLK_DEN.B.GCLK_DEN = 1;

	/* Enable FXCLK */
	GTM_CMU_CLK_EN.B.EN_FXCLK = 0b10;

	/* HBP1IN
	 * P15.1
	 */
	IfxGtm_PinMap_setTomTout(&IfxGtm_TOM0_12_TOUT72_P15_1_OUT, IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);
	GTM_TOM0_TGC1_ENDIS_CTRL.B.ENDIS_CTRL4 = 0b10;
	GTM_TOM0_TGC1_FUPD_CTRL.B.FUPD_CTRL4   = 0b10;
	GTM_TOM0_TGC1_GLB_CTRL.B.UPEN_CTRL4    = 0b10;
	GTM_TOM0_CH12_SR0.B.SR0 = PWM_PERIOD;
	GTM_TOM0_CH12_SR1.B.SR1 = PWM_PERIOD / 2;
	GTM_TOM0_CH12_CN0.B.CN0 = 0;
	GTM_TOM0_CH12_CTRL.B.CLK_SRC_SR = 0;

}

