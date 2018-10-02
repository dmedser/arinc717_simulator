#include "pwm.h"
#include "isr_priorities.h"
#include "ports.h"
#include <IfxScuCcu.h>
#include <IfxScu_reg.h>
#include <IfxGtm.h>

#define BITRATE_BPS	768
// 768   bps = 64   wps
// 1536  bps = 128  wps
// 3072  bps = 256  wps
// 6144  bps = 512  wps
// 12288 bps = 1024 wps

#define PWM_PERIOD	((uint16_t)(IfxScuCcu_getGtmFrequency() / (BITRATE_BPS * 2)))

void pwm_init(void) {
	/* fGTM = fPLL / 2 = 100 MHz */

	Ifx_GTM *gtm = &MODULE_GTM;
	IfxGtm_enable(gtm);

	/* Global divider = 1 */
	GTM_CMU_GCLK_NUM.B.GCLK_NUM = 1;
	GTM_CMU_GCLK_DEN.B.GCLK_DEN = 1;

	/* Enable FXCLK */
	GTM_CMU_CLK_EN.B.EN_FXCLK = 0b10;

	/* HBP1IN */
	{
		GTM_TOM0_TGC1_FUPD_CTRL.B.FUPD_CTRL4   = 0b10;
		GTM_TOM0_TGC1_GLB_CTRL.B.UPEN_CTRL4    = 0b10;
		GTM_TOM0_CH12_SR0.B.SR0 = PWM_PERIOD;
		GTM_TOM0_CH12_SR1.B.SR1 = PWM_PERIOD / 2;
		GTM_TOM0_CH12_CN0.B.CN0 = 0;
		GTM_TOM0_CH12_CTRL.B.CLK_SRC_SR = 0;

		GTM_TOM0_CH12_IRQ_EN.B.CCU0TC_IRQ_EN = 0b1;
		GTM_TOM0_CH12_IRQ_EN.B.CCU1TC_IRQ_EN = 0b1;

		/* Service request priority number (0 - lowest, 0xFF - highest priority) */
		MODULE_SRC.GTM.GTM[0].TOM[0][6].B.SRPN = ISR_PN_GTM_TOM0_12_CMP_MATCH;
		/* Enable service request */
		MODULE_SRC.GTM.GTM[0].TOM[0][6].B.SRE  = 0b1;
		_install_int_handler(ISR_PN_GTM_TOM0_12_CMP_MATCH, (void (*) (int))ISR_GTM_TOM0_12_cmp_match, 0);
	}

	/* Apply the updates */
	GTM_TOM0_TGC1_GLB_CTRL.B.HOST_TRIG = 0b1;

}


void start_hbp_tx(void) {
	TOGGLE_PIN(HBP1IN);
	pwm_on();
}

void stop_hbp_tx(void) {
	pwm_off_reset();
	SET_PIN_LOW(HBP1IN);
}

void pwm_on(void) {
	GTM_TOM0_TGC1_ENDIS_CTRL.B.ENDIS_CTRL4 = 0b10;
	GTM_TOM0_TGC1_GLB_CTRL.B.HOST_TRIG = 0b1;
}

void pwm_off_reset(void) {
	GTM_TOM0_TGC1_ENDIS_CTRL.B.ENDIS_CTRL4 = 0b01;
	GTM_TOM0_TGC1_GLB_CTRL.B.HOST_TRIG = 0b1;
	GTM_TOM0_CH12_CN0.B.CN0 = 0;
}

uint16_t word = 0;
uint8_t  rsh_cntr = 0;

#define RSH_MAX_CNT		12
#define WORD_IS_PASSED	(rsh_cntr >= RSH_MAX_CNT)
#define BIT_TO_PASS		((word >> rsh_cntr) & 0x0001)

void hbp_tx_word(uint16_t val) {
	word = val;
	while(!WORD_IS_PASSED);

}

void hbp_tx_buf(uint16_t *buf, uint16_t len) {
	start_hbp_tx();
	uint16_t i = 0;
	for(; i < len; i++) {
		hbp_tx_word(buf[i]);
	}

}



void ISR_GTM_TOM0_12_cmp_match(void) {
	/* Half-period */
	if(GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU1TC == 0b1) {
		GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU1TC = 0b1;
		if(BIT_TO_PASS == 1) {
			TOGGLE_PIN(HBP1IN);
		}
	}

	/* Period */
	if(GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU0TC == 0b1) {
		GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU0TC = 0b1;
		TOGGLE_PIN(HBP1IN);
		rsh_cntr++;
	}
}

