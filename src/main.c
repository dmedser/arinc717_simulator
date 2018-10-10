/* Author: t.me/dmedser */

#include "scu_clk_cfg.h"
#include "hbp_tx.h"
#include "hbp_rx.h"
#include <IfxCpu.h>

/* TC212L */

int main(void) {
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	IfxCpu_disableInterrupts();

	_init_vectab();

	scu_clk_init();

	hbp_tx_init();
	//hbp_rx_init();

	IfxCpu_enableInterrupts();

	hbp_tx_process();
	//hbp_rx_process();

	return 0;
}

