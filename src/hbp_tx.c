/* Author: t.me/dmedser */

#include "hbp_tx.h"
#include "global_cfg.h"
#include "ports.h"
#include "pwm.h"
#include "rtos.h"
#include "MulticanBasic.h"
#include <IfxGtm_reg.h>
#include <IfxPort.h>
#include <IfxCpu.h>
#include "dd_ram.h"

#define RSH_MAX             11
#define WORD_IS_PASSED		(txd.rsh == RSH_MAX)
#define IT_WAS_LAST_WORD	(txd.num == 0)
#define BIT_TO_PASS			((txd.buf[txd.idx] >> txd.rsh) & 0x0001)

#define RECEIVED_OPCODE		CAN_InBox_Buffer8_data[0]
#define START				0xFF
#define STOP				0x01
#define ND					0x00

static tx_buf txd = {{0}, 0, 0};

static uint16_t increment = 0;

void txd_init(){

	txd.buf[0] = sws.buf[0];

	uint16_t init_val = 0;

	uint16_t i = 1;
	for(; i < FRAME_LEN; i++) {
		txd.buf[i] = init_val++;
	}
}

void hbp_tx_init(void) {
	pwm_init();
	tx_ports_init();
	rtos_init();
	MulticanBasic_init();
	txd_init();
}


inline void start_hbp_tx(void) {
	SET_PIN_HIGH(HBP_OUT);
	pwm_on();
}


inline void stop_hbp_tx(void) {
	pwm_off_reset();
	SET_PIN_LOW(HBP_OUT);
}


void hbp_tx_process(void) {
	while(1) {
		switch(RECEIVED_OPCODE) {
		case START:
			RECEIVED_OPCODE = ND;
			start_hbp_tx();
			break;
		case STOP:
			RECEIVED_OPCODE = ND;
			stop_hbp_tx();
			break;
		}
	}
}


void ISR_GTM_TOM0_CH12_cmp_match(void) {

	IfxCpu_forceDisableInterrupts();

	/* Half-period */
	if(GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU1TC == 0b1) {
		GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU1TC = 0b1;
		if(BIT_TO_PASS == 1) {
			TOGGLE_PIN(HBP_OUT);
		}
	}

	/* Period */
	if(GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU0TC == 0b1) {
		GTM_TOM0_CH12_IRQ_NOTIFY.B.CCU0TC = 0b1;
		if(WORD_IS_PASSED) {
			txd.rsh = 0;
			if(txd.idx == 0) {
				sws.idx = (sws.idx + 1) % 4;
				increment = sws.idx * (FRAME_LEN - 1);
			}
			txd.buf[txd.idx] = (txd.idx == 0) ? sws.buf[sws.idx] : increment++;
			txd.idx = (txd.idx + 1) % FRAME_LEN;
		}
		else {
			txd.rsh++;
		}
		TOGGLE_PIN(HBP_OUT);
	}

	IfxCpu_enableInterrupts();

}
