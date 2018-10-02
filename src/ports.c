#include "ports.h"
#include <IfxPort_Io.h>
#include <IfxPort_PinMap.h>

void ports_init(void) {
	/* HBP1IN  */
	{
		const IfxPort_Io_ConfigPin HBP1IN_cfg_pin[1] = {{&IfxPort_P15_1, IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed1}};
		const IfxPort_Io_Config HBP1IN_cfg = {
			sizeof(HBP1IN_cfg_pin) / sizeof(IfxPort_Io_ConfigPin),
			(IfxPort_Io_ConfigPin *)HBP1IN_cfg_pin
		};
		IfxPort_Io_initModule(&HBP1IN_cfg);
		SET_PIN_LOW(HBP1IN);
	}
}
