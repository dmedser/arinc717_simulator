#ifndef PORTS_H_
#define PORTS_H_

#define SET_PIN_HIGH(x)		IfxPort_setPinHigh(x)
#define SET_PIN_LOW(x)		IfxPort_setPinLow(x)
#define TOGGLE_PIN(x)		IfxPort_togglePin(x)
#define HBP1IN				&MODULE_P15, 1

void ports_init(void);

#endif /* PORTS_H_ */
