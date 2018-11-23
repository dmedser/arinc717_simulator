/* Author: t.me/dmedser */

#include "ports.h"
#include "global_cfg.h"

void ports_init(void) {
	#if(OP_MODE == TRANSMITTER)

	/* HBP_TX  */
	const IfxPort_Io_ConfigPin hbp_out_cfg_pin[1] = {{HBP_OUT_CFG, IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed1}};

	const IfxPort_Io_Config hbp_out_cfg = {
		sizeof(hbp_out_cfg_pin) / sizeof(IfxPort_Io_ConfigPin),
		(IfxPort_Io_ConfigPin *)hbp_out_cfg_pin
	};

	IfxPort_Io_initModule(&hbp_out_cfg);

	SET_PIN_LOW(HBP_OUT);

	#else

	/* HBP_RX */
	IfxGtm_PinMap_setTimTin(HBP_IN_CFG, IfxPort_InputMode_noPullDevice);

	#endif
}

