/* Author: t.me/dmedser */

#include "ports.h"
#include "global_cfg.h"

void ports_init(void) {
	#if(OP_MODE	== TRANSMITTER)
	/* HBP_TX  */
	{
		const IfxPort_Io_ConfigPin HBP_OUT_cfg_pin[1] = {{&IfxPort_P15_0, IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed1}};
		const IfxPort_Io_Config HBP_OUT_cfg = {
			sizeof(HBP_OUT_cfg_pin) / sizeof(IfxPort_Io_ConfigPin),
			(IfxPort_Io_ConfigPin *)HBP_OUT_cfg_pin
		};
		IfxPort_Io_initModule(&HBP_OUT_cfg);
		SET_PIN_LOW(HBP_OUT);
	}
	#else
	/* HBP_RX */
	{
		IfxGtm_PinMap_setTimTin(&IfxGtm_TIM0_0_TIN32_P33_10_IN, IfxPort_InputMode_noPullDevice);
	}
	#endif
}

