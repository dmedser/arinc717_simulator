#include "ports.h"
#include <IfxPort_Io.h>
#include <IfxPort_PinMap.h>
#include <IfxGtm_PinMap.h>

void tx_ports_init(void) {
	/* HBP_TX  */
	{
		const IfxPort_Io_ConfigPin HBP_OUT_cfg_pin[1] = {{&IfxPort_P15_1, IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed1}};
		const IfxPort_Io_Config HBP_OUT_cfg = {
			sizeof(HBP_OUT_cfg_pin) / sizeof(IfxPort_Io_ConfigPin),
			(IfxPort_Io_ConfigPin *)HBP_OUT_cfg_pin
		};
		IfxPort_Io_initModule(&HBP_OUT_cfg);
		SET_PIN_LOW(HBP_OUT);
	}
}

void rx_ports_init(void) {
	/* HBP_RX */
	{
		IfxGtm_PinMap_setTimTin(&IfxGtm_TIM0_0_TIN32_P33_10_IN, IfxPort_InputMode_noPullDevice);
	}
}
