/* Author: t.me/dmedser */

#ifndef GLITCH_H_
#define GLITCH_H_

void glitch_timer_init(void);
inline void gt_update(void);
inline void gt_on(void);
inline void gt_off_reset(void);
void ISR_glitch(void);

#endif /* GLITCH_H_ */
