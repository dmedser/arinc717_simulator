/* Author: t.me/dmedser */

#ifndef PWM_H_
#define PWM_H_

void pwm_init(void);
void pwm_on(void);
inline void pwm_off_reset(void);
void ISR_GTM_TOM0_CH12_cmp_match(void);

#endif /* PWM_H_ */
