/* Author: t.me/dmedser */

#ifndef EDGE_BIT_CAPTURE_H_
#define EDGE_BIT_CAPTURE_H_

#include "global_cfg.h"

#define FLT_ACCEPTANCE_TIME				((1 * BIT_TX_PERIOD) / 5)
#define SECOND_HALF_OF_BIT_TX_PERIOD	((4 * BIT_TX_PERIOD) / 5)
#define OVER_BIT_TX_PERIOD				((5 * BIT_TX_PERIOD) / 4)
#define BIT_CAPTURE_TIMER_VALUE			GTM_TOM0_CH0_CN0.B.CN0


void edge_bit_capture_init(void);
void ISR_edge_capture(void);
void ISR_bit_capture(void);
inline void bit_capture_timer_on(void);
inline void bit_capture_timer_off(void);
inline void bit_capture_timer_reset(void);

#endif /* EDGE_BIT_CAPTURE_H_ */
