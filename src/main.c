#include "scu_clk_cfg.h"
#include "timer_pwm.h"
#include <IfxCpu.h>
#include <stdlib.h>

int main(void) {
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	IfxCpu_disableInterrupts();

	_init_vectab();

	scu_clk_init();

	timer_pwm_init();

	IfxCpu_enableInterrupts();

	while(1);

	return EXIT_SUCCESS;
}

