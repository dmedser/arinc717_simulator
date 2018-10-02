#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

void pwm_init(void);
void pwm_on(void);
void pwm_off_reset(void);
void ISR_GTM_TOM0_12_cmp_match(void);

void tx(uint16_t *buf, uint16_t len);
void start_tx(void);

#endif /* PWM_H_ */
