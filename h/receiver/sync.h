/* Author: t.me/dmedser */

#ifndef SYNC_H_
#define SYNC_H_

#include <stdint.h>

#define SYNC_FLAGS_MASK		0x0F

typedef uint8_t sync_flags_t;

sync_flags_t sw_tracking(void);

inline void clear_sync_flags(void);

#endif /* SYNC_H_ */
