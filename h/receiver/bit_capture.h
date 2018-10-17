/* Author: t.me/dmedser */

#ifndef BIT_CAPTURE_H_
#define BIT_CAPTURE_H_

void bit_capture_init(void);
inline void bit_capture_timer_on(void);
inline void bit_capture_timer_reset(void);
inline void update_encoded_bit_stream(void);

#endif /* BIT_CAPTURE_H_ */
