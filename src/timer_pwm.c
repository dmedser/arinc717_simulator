#include "timer_pwm.h"
#include <IfxScuCcu.h>
#include <IfxScu_reg.h>
#include <IfxGtm.h>
#include <IfxGtm_PinMap.h>
#include <IfxPort_Io.h>
#include <IfxPort_PinMap.h>
#include <stdint.h>

#define ISR_PRIORITY_GTM_TOM0_12_CM		128

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

	GTM_TOM0_TGC1_OUTEN_CTRL.B.OUTEN_CTRL4 = 0b10;

	GTM_TOM0_CH12_IRQ_EN.B.CCU0TC_IRQ_EN = 0b1;
	GTM_TOM0_CH12_IRQ_EN.B.CCU1TC_IRQ_EN = 0b1;

	/* Service request priority number (0 - lowest, 0xFF - highest priority) */
	MODULE_SRC.GTM.GTM[0].TOM[0][6].B.SRPN = ISR_PRIORITY_GTM_TOM0_12_CM;
	/* Enable service request */
	MODULE_SRC.GTM.GTM[0].TOM[0][6].B.SRE  = 0b1;
	_install_int_handler(ISR_PRIORITY_GTM_TOM0_12_CM, (void (*) (int))ISR_GTM_TOM0_12_CM, 0);

	/* Apply the updates */
	GTM_TOM0_TGC1_GLB_CTRL.B.HOST_TRIG = 0b1;
}

uint32_t test = 0;
uint32_t test2 = 0;

void ISR_GTM_TOM0_12_CM(void) {
	if(GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU0TC == 0b1) {
		GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU0TC = 0b1;
		test++;
	}
	if(GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU1TC == 0b1) {
		GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU1TC = 0b1;
		test2++;
	}
}

