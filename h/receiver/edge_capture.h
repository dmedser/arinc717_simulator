/* Author: t.me/dmedser */

#ifndef EDGE_CAPTURE_H_
#define EDGE_CAPTURE_H_

void edge_capture_timer_init(void);
inline void edge_capture_timer_update(void);
void ISR_edge_capture(void);

#endif /* EDGE_CAPTURE_H_ */
