/* Author: t.me/dmedser */

#include "hbp_tx.h"
#include "global_cfg.h"
#include "ports.h"
#include "pwm.h"
/* Glitch mode */
/*
#include "glitch.h"
*/
#include "dd_ram.h"
#include <IfxGtm_reg.h>
#include <IfxPort.h>
#include <IfxCpu.h>
#include <stdlib.h>
#include <Platform_Types.h>

#define RSH_MAX             11
#define WORD_IS_PASSED		(txd.rsh == RSH_MAX)
#define IT_WAS_LAST_WORD	(txd.num == 0)
#define BIT_TO_PASS			((txd.buf[txd.idx] >> txd.rsh) & 0x0001)

#define RECEIVED_OPCODE		CAN_InBox_Buffer8_data[0]
#define START				0xFF
#define STOP				0x01
/* Glitch mode */
/*
#define GLITCHES_ENABLE		0x02
#define GLITCHES_DISABLE	0x03
*/
#define ND					0x00

#define BUF_U16_SIZE		NUM_OF_SUBFRAMES

typedef struct txd_t {
	uint16_t buf[FRAME_LEN];
	uint16_t idx;
	uint8_t  rsh;
} txd_t;

typedef struct buf_u16_t {
	uint16_t buf[BUF_U16_SIZE];
	uint8_t  idx;
} buf_u16_t;

static buf_u16_t sws = {{SW1, SW2, SW3, SW4}, 0};
static txd_t txd = {{0}, 0, 0};
static uint16_t increment = 0;
/* Glitch mode */
/*
static boolean glitches_enabled = FALSE;
*/

void txd_init(){
	txd.buf[0] = sws.buf[0];

	uint16_t init_val = 0;

	uint16_t i = 1;
	for(; i < FRAME_LEN; i++) {
		txd.buf[i] = init_val++;
	}

	txd.idx = 0;
	txd.rsh = 0;
}


void hbp_tx_init(void) {
	pwm_init();
	txd_init();
	/* Glitch mode */
	/*
	glitch_generator_init();
	*/
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
			txd_init();
			increment = 0;
			sws.idx = 0;
			break;
		/* Glitch mode */
		/*
		case GLITCHES_ENABLE:
			RECEIVED_OPCODE = ND;
			glitches_enabled = TRUE;
			break;
		case GLITCHES_DISABLE:
			RECEIVED_OPCODE = ND;
			glitches_enabled = FALSE;
			break;
		*/
		}
	}
}


void ISR_bit_tx(void) {
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
			/* Transmit single word  */
			/*
			stop_hbp_tx();
			*/

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

		/* Transmit single word  */
		/*
		}
		else {
			txd.rsh++;
			TOGGLE_PIN(HBP_OUT);
		}
		*/

		/* Glitch mode */
		/*
		if(glitches_enabled) {
			if(abs(rand()) % 2 == 1) {
				gg_on();
			}
		}
		*/
	}

	IfxCpu_enableInterrupts();
}

/* Glitch mode */
/*
void ISR_glitch(void) {
	IfxCpu_forceDisableInterrupts();
	TOGGLE_PIN(HBP_OUT);

	// Glitch end
	if(GTM_TOM0_CH1_IRQ_NOTIFY.B.CCU0TC == 0b1) {

		gg_off_reset();

		gg_update();
	}

	// Flag must be cleared manually
	GTM_TOM0_CH1_IRQ_NOTIFY.B.CCU0TC = 0b1;
	GTM_TOM0_CH1_IRQ_NOTIFY.B.CCU1TC = 0b1;

	IfxCpu_enableInterrupts();
}
*/

