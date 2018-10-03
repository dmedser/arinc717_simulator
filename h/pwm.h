#ifndef PWM_H_
#define PWM_H_

void pwm_init(void);
void pwm_on(void);
void pwm_off_reset(void);
void ISR_GTM_TOM0_12_cmp_match(void);

#endif /* PWM_H_ */
