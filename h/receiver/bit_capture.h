/* Author: t.me/dmedser */

#ifndef BIT_CAPTURE_H_
#define BIT_CAPTURE_H_

#include <stdint.h>

void bit_capture_init(void);
inline uint16_t	get_bit_capture_timer_value(void);
inline void bit_capture_timer_on(void);
inline void bit_capture_timer_reset(void);

#endif /* BIT_CAPTURE_H_ */
