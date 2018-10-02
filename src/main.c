#include "scu_clk_cfg.h"
#include "ports.h"
#include "pwm.h"
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

	uint16_t buf[2] = {0b0000101001110101, 0b0000101011100101};
	hbp_tx_buf(buf, 2);

	while(1) {

	}

	return EXIT_SUCCESS;
}

