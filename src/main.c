#include "scu_clk_cfg.h"
#include "ports.h"
#include "pwm.h"
#include "hbp.h"
#include "rtos.h"
#include <IfxCpu.h>
#include <IfxScuWdt.h>
#include <stdlib.h>
#include "model.h"


int main(void) {
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	IfxCpu_disableInterrupts();

	_init_vectab();

	scu_clk_init();

	ports_init();

	rtos_init();

	MulticanBasic_init();

	pwm_init();

	IfxCpu_enableInterrupts();

	txd.buf[0] = 0b101001110101;
	txd.num = 1;

	while(1) {
		if(CAN_InBox_Buffer8_data[0] == 0xFF) {
			CAN_InBox_Buffer8_data[0] = 0;
			hbp_tx();
			txd.num = 1;
		}
	}

	return 0;
}

