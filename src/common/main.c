/* Author: t.me/dmedser */

#include "scu_clk_cfg.h"
#include "rtos.h"
#include "gtm.h"
#include "hbp_tx.h"
#include "hbp_rx.h"
#include "ports.h"
#include <IfxCpu.h>

/* TEST */
#include "dynamic_buf.h"
#define DBUF_INITIAL_SIZE	3

/* TC212L */

int main(void) {
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	IfxCpu_disableInterrupts();

	_init_vectab();

	scu_clk_init();
/*
	rtos_init();

	gtm_init();

	#if(OP_MODE == TRANSMITTER)
	hbp_tx_init();
	#else
	hbp_rx_init();
	#endif

	ports_init();

	IfxCpu_enableInterrupts();

	#if(OP_MODE == TRANSMITTER)
	hbp_tx_process();
	#else
	hbp_rx_process();
	#endif
*/

	/* TEST */
	dbuf_t dbuf1 = {(uint32_t *)malloc(DBUF_INITIAL_SIZE), 0, DBUF_INITIAL_SIZE};

	add_to_dbuf(&dbuf1, 1);
	add_to_dbuf(&dbuf1, 2);
	add_to_dbuf(&dbuf1, 3);
	add_to_dbuf(&dbuf1, 4);
	clear_dbuf(&dbuf1);

	while(1);

	return 0;
}

