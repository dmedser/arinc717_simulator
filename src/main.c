#include "scu_clk_cfg.h"
#include "ports.h"
#include "pwm.h"
#include "edge_bit_capture.h"
#include "hbp_tx.h"
#include "hbp_rx.h"
#include "rtos.h"
#include <IfxCpu.h>
#include <IfxScuWdt.h>
#include <stdlib.h>
#include "model.h"


void hbp_tx_init(void) {
	pwm_init();
	tx_ports_init();
	rtos_init();
	MulticanBasic_init();
}


void hbp_rx_init(void) {
	edge_bit_capture_init();
	rx_ports_init();
}


void hbp_tx_process(void) {
	txd.buf[0] = 0b101001110101;
	txd.buf[1] = 0b111000111000;
	txd.num = 2;

	while(1) {
		if(CAN_InBox_Buffer8_data[0] == 0xFF) {
			CAN_InBox_Buffer8_data[0] = 0;
			hbp_tx();
			txd.num = 2;
		}
	}
}


void hbp_rx_process(void) {
	while(1);
}


int main(void) {
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	IfxCpu_disableInterrupts();

	_init_vectab();

	scu_clk_init();

	//hbp_tx_init();
	hbp_rx_init();

	IfxCpu_enableInterrupts();

	//hbp_tx_process();
	hbp_rx_process();

	return 0;
}

