/* Author: t.me/dmedser */

#ifndef RTOS_H_
#define RTOS_H_

#include "global_cfg.h"

void rtos_init(void);

void ISR_rtos_100_us(void);

#if(OP_MODE == TRANSMITTER)
void ISR_rtos_1_ms_transmitter(void);
#else
void ISR_rtos_1_ms_receiver(void);
#endif

#endif /* RTOS_H_ */
