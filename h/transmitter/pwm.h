/* Author: t.me/dmedser */

#ifndef PWM_H_
#define PWM_H_

void pwm_init(void);
inline void pwm_on(void);
inline void pwm_off_reset(void);
void ISR_bit_tx(void);

#endif /* PWM_H_ */
