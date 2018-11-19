/* Author: t.me/dmedser */

#ifndef H_CAN_H_
#define H_CAN_H_

#include "global_cfg.h"

void can_init(void);

#if(OP_MODE == RECEIVER)
void ISR_can_tx(void);
#endif

void ISR_can_rx(void);

#endif /* H_CAN_H_ */
