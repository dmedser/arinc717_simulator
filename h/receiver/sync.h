/* Author: t.me/dmedser */

#ifndef SYNC_H_
#define SYNC_H_

#include <stdint.h>
#include <Platform_Types.h>

#define ADJACENT_SW12_CAPTURED_FLAG			0
#define ADJACENT_SW23_CAPTURED_FLAG			1
#define ADJACENT_SW34_CAPTURED_FLAG			2
#define ADJACENT_SW41_CAPTURED_FLAG			3
#define BUS_DISCONNECT_FLAG					4

#define SYNCHRONIZED						((sync_flags & SUCCESS_FLAGS) == SUCCESS_FLAGS)

#define SUCCESS_FLAGS						0x0F

#define SET_SYNC_FLAG(SYNC_FLAG_POS)		sync_flags |= (1 << SYNC_FLAG_POS)
#define CLEAR_SYNC_FLAG(SYNC_FLAG_POS)		sync_flags &= ~(1 << SYNC_FLAG_POS)
#define CLEAR_SYNC_FLAGS(MASK)				sync_flags &= ~MASK

#define STAMPS_BUF_SIZE						100

typedef enum sws_tracking_states {
	IDLE,
	SW12,
	SW23,
	SW34,
	SW41
} sws_tracking_states;

typedef struct buf_u32_t {
	uint32_t buf[STAMPS_BUF_SIZE];
	uint16_t idx;
} buf_u32_t;

void sws_tracking(void);

extern uint8_t sync_flags;

#endif /* SYNC_H_ */
