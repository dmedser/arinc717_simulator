/* Author: t.me/dmedser */

#ifndef H_CAN_H_
#define H_CAN_H_

#include "global_cfg.h"
#include <stdint.h>

void can_init(void);

#if(OP_MODE == RECEIVER)
void can_tx(uint32_t id, uint64_t data);
void ISR_can_tx(void);
#endif
void ISR_can_rx(void);

#endif /* H_CAN_H_ */
