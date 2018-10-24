/* Author: t.me/dmedser */

#ifndef GLITCH_H_
#define GLITCH_H_

void gg_update(void);
void glitch_generator_init(void);
void gg_update(void);
inline void gg_on(void);
inline void gg_off_reset(void);
void ISR_glitch(void);

#endif /* GLITCH_H_ */
