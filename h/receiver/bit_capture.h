/* Author: t.me/dmedser */

#ifndef BIT_CAPTURE_H_
#define BIT_CAPTURE_H_

#include <stdint.h>
#include "global_cfg.h"

#define BITRATE_ACCEPTANCE_PERCENT			10
#define BITRATE_ACCEPTANCE_ABSOLUTE			((BIT_TX_PERIOD / 100) * BITRATE_ACCEPTANCE_PERCENT)

#define HALF_BIT_TX_PERIOD_LOWER_BOUND		((BIT_TX_PERIOD / 2) - BITRATE_ACCEPTANCE_ABSOLUTE)
#define HALF_BIT_TX_PERIOD_UPPER_BOUND		((BIT_TX_PERIOD / 2) + BITRATE_ACCEPTANCE_ABSOLUTE)

#define BIT_TX_PERIOD_LOWER_BOUND			(BIT_TX_PERIOD - BITRATE_ACCEPTANCE_ABSOLUTE)
#define BIT_TX_PERIOD_UPPER_BOUND			(BIT_TX_PERIOD + BITRATE_ACCEPTANCE_ABSOLUTE)

void bit_capture_init(void);
inline void btc_on(void);
inline void btc_off(void);
inline void btc_reset(void);
uint16_t get_btc_value(void);

void ISR_bit_tx_timeout(void);

#endif /* BIT_CAPTURE_H_ */
