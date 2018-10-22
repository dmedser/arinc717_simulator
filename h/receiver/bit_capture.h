/* Author: t.me/dmedser */

#ifndef BIT_CAPTURE_H_
#define BIT_CAPTURE_H_

#include <stdint.h>

void bit_capture_init(void);
inline void bit_capture_timer_on(void);
inline void bit_capture_timer_off_reset(void);
uint32_t bit_capture_timer_value(void);

//inline void update_encoded_bit_stream(void);

void ISR_bit_capture(void);

#endif /* BIT_CAPTURE_H_ */
