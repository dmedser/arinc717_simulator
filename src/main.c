#include "scu_clk_cfg.h"
#include "ports.h"
#include "pwm.h"
#include "hbp.h"
#include <IfxCpu.h>
#include <IfxScuWdt.h>
#include <stdlib.h>

int main(void) {
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	IfxCpu_disableInterrupts();

	_init_vectab();

	scu_clk_init();

	ports_init();

	pwm_init();

	IfxCpu_enableInterrupts();

	txd.buf[0] = 0b101001110101;
	txd.buf[1] = 0b100110101011;
	txd.buf[2] = 0b000000000000;
	txd.num = 3;

	hbp_tx();


	while(1) {

	}

	return EXIT_SUCCESS;
}

