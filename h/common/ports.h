/* Author: t.me/dmedser */

#ifndef PORTS_H_
#define PORTS_H_

#include <IfxPort_Io.h>
#include <IfxPort_PinMap.h>
#include <IfxGtm_PinMap.h>

#define SET_PIN_HIGH(x)   IfxPort_setPinHigh(x)
#define SET_PIN_LOW(x)    IfxPort_setPinLow(x)
#define TOGGLE_PIN(x)     IfxPort_togglePin(x)

#define HBP_OUT           &MODULE_P13, 0
#define HBP_OUT_CFG       &IfxPort_P13_0

#define HBP_IN            &MODULE_P33, 0
#define HBP_IN_CFG        &IfxGtm_TIM0_4_TIN22_P33_0_IN

void ports_init(void);

#endif /* PORTS_H_ */
