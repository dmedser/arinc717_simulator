/* Author: t.me/dmedser */

#ifndef RTOS_H_
#define RTOS_H_

void rtos_core_timer_100_us_init(void);
void rtos_service_requests_init(void);
void rtos_init(void);

void ISR_100_us(void);
void ISR_1_ms_tx(void);
void ISR_1_ms_rx(void);

#endif /* RTOS_H_ */
