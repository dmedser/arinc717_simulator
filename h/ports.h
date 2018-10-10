/* Author: t.me/dmedser */

#ifndef PORTS_H_
#define PORTS_H_

#define SET_PIN_HIGH(x)		IfxPort_setPinHigh(x)
#define SET_PIN_LOW(x)		IfxPort_setPinLow(x)
#define TOGGLE_PIN(x)		IfxPort_togglePin(x)

#define HBP_OUT				&MODULE_P15, 1
#define HBP_IN				&MODULE_P33, 10

void tx_ports_init(void);
void rx_ports_init(void);

#endif /* PORTS_H_ */
