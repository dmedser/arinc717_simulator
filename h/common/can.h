/* Author: t.me/dmedser */

#ifndef H_CAN_H_
#define H_CAN_H_

#include <stdint.h>

void can_init(void);

void can_tx(uint32_t id, uint64_t data);

#endif /* H_CAN_H_ */
