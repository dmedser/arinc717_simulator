/* Author: t.me/dmedser */

#ifndef BIT_CAPTURE_H_
#define BIT_CAPTURE_H_

#include <stdint.h>

void bit_capture_init(void);
inline void btc_on(void);
inline void btc_off(void);
inline void btc_reset(void);
uint32_t btc_value(void);

void ISR_bit_capture(void);

#endif /* BIT_CAPTURE_H_ */
