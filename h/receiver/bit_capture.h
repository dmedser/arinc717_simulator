/* Author: t.me/dmedser */

#ifndef BIT_CAPTURE_H_
#define BIT_CAPTURE_H_

#include "global_cfg.h"
#include <stdint.h>

#define BITRATE_ACCEPTANCE_PERCENT			10
#define BITRATE_ACCEPTANCE_ABSOLUTE			((bit_tx_period / 100) * BITRATE_ACCEPTANCE_PERCENT)

#define HALF_BIT_TX_PERIOD_LOWER_BOUND		((bit_tx_period / 2) - BITRATE_ACCEPTANCE_ABSOLUTE)
#define HALF_BIT_TX_PERIOD_UPPER_BOUND		((bit_tx_period / 2) + BITRATE_ACCEPTANCE_ABSOLUTE)

#define BIT_TX_PERIOD_LOWER_BOUND			(bit_tx_period - BITRATE_ACCEPTANCE_ABSOLUTE)
#define BIT_TX_PERIOD_UPPER_BOUND			(bit_tx_period + BITRATE_ACCEPTANCE_ABSOLUTE)

void bit_capture_timer_init(void);
inline void bit_capture_timer_update(void);

/* bct = Bit Capture Timer */
inline void bct_on(void);
inline void bct_off(void);
inline void bct_reset(void);
uint16_t get_bct_value(void);

void ISR_bit_tx_timeout(void);

#endif /* BIT_CAPTURE_H_ */
