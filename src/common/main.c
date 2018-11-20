/* Author: t.me/dmedser */

#include "global_cfg.h"
#include "scu_clk_cfg.h"
#include "gtm.h"
#include "can.h"
#include "hbp_tx.h"
#include "hbp_rx.h"
#include "ports.h"
#include <IfxCpu.h>

/* TC212L */

int main(void) {
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	IfxCpu_disableInterrupts();

	_init_vectab();

	scu_clk_init();

	gtm_init();

	#if(OP_MODE == TRANSMITTER)

	hbp_tx_init();

	#else

	download_from_dflash(BITRATE);

	download_from_dflash(SYNC_WORDS);

	hbp_rx_init();

	#endif

	can_init();

	ports_init();

	IfxCpu_enableInterrupts();

	while(1);

	return 0;
}

