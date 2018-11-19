/* Author: t.me/dmedser */

#include "sync.h"
#include "hbp_rx.h"
#include "global_cfg.h"
#include <Platform_Types.h>

#define SW1_REVERSED				0xE2400000
#define SW2_REVERSED				0x1DA00000
#define SW3_REVERSED				0xE2500000
#define SW4_REVERSED				0x1DB00000

#define SW_MASK						0xFFF00000

#define SW1_CAPTURED				((bitstream.bits & SW_MASK) == SW1_REVERSED)
#define SW2_CAPTURED				((bitstream.bits & SW_MASK) == SW2_REVERSED)
#define SW3_CAPTURED				((bitstream.bits & SW_MASK) == SW3_REVERSED)
#define SW4_CAPTURED				((bitstream.bits & SW_MASK) == SW4_REVERSED)

#define ADJACENT_SWS_LOST			((bitstream.counter - last_adjacent_sws_stamp) > BITRATE_BPS)

#define ADJACENT_SW12_CAPTURED		0
#define ADJACENT_SW23_CAPTURED		1
#define ADJACENT_SW34_CAPTURED		2
#define ADJACENT_SW41_CAPTURED		3

#define SUCCESS						TRUE
#define FAIL						FALSE

#define STAMPS_NUMBER				100

typedef struct sw_stamps_t {
	uint32_t buf[STAMPS_NUMBER];
	uint16_t idx;
} sw_stamps_t;

typedef enum sw_tracking_state_t {
	IDLE,
	SW12,
	SW23,
	SW34,
	SW41
} sw_tracking_state_t;

static sw_tracking_state_t sw_tracking_state = IDLE;
static sw_stamps_t sw_stamps = {{0}, 0};
static uint32_t last_adjacent_sws_stamp = 0;
static sync_flags_t sync_flags = 0;

static inline void clear_stamps(void) {
	sw_stamps.idx = 0;
}


static uint32_t make_stamp(void) {
	sw_stamps.buf[sw_stamps.idx++] = bitstream.counter;
	return bitstream.counter;
}


static boolean find_adjacent_sws(void) {
	uint16_t i = 0;

	for(; i < sw_stamps.idx; i++) {

		if((bitstream.counter - sw_stamps.buf[i]) == BITRATE_BPS) {
			return SUCCESS;
		}

	}
	return FAIL;
}


static void set_sync_flag(uint8_t sf_pos) {
	sync_flags |= (1 << sf_pos);
}


inline void clear_sync_flags(void) {
	sync_flags &= ~SYNC_FLAGS_MASK;
}


sync_flags_t sw_tracking(void) {
	switch(sw_tracking_state) {
	case IDLE:
		if(SW1_CAPTURED) {
			make_stamp();
			sw_tracking_state = SW12;
		}
		else if(SW2_CAPTURED) {
			make_stamp();
			sw_tracking_state = SW23;
		}
		else if(SW3_CAPTURED) {
			make_stamp();
			sw_tracking_state = SW34;
		}
		else if(SW4_CAPTURED) {
			make_stamp();
			sw_tracking_state = SW41;
		}
		break;
	case SW12:
		if(SW1_CAPTURED) {
			make_stamp();
		}
		else if(SW2_CAPTURED) {
			if(find_adjacent_sws() == SUCCESS) {
				clear_stamps();
				last_adjacent_sws_stamp = make_stamp();
				set_sync_flag(ADJACENT_SW12_CAPTURED);
				sw_tracking_state = SW23;
			}
		}
		break;
	case SW23:
		if(SW2_CAPTURED) {
			make_stamp();
		}
		else if(SW3_CAPTURED) {
			if(find_adjacent_sws() == SUCCESS) {
				clear_stamps();
				last_adjacent_sws_stamp = make_stamp();
				set_sync_flag(ADJACENT_SW23_CAPTURED);
				sw_tracking_state = SW34;
			}
		}
		break;
	case SW34:
		if(SW3_CAPTURED) {
			make_stamp();
		}
		else if(SW4_CAPTURED) {
			if(find_adjacent_sws() == SUCCESS) {
				clear_stamps();
				last_adjacent_sws_stamp = make_stamp();
				set_sync_flag(ADJACENT_SW34_CAPTURED);
				sw_tracking_state = SW41;
			}
		}
		break;
	case SW41:
		if(SW4_CAPTURED) {
			make_stamp();
		}
		else if(SW1_CAPTURED) {
			if(find_adjacent_sws() == SUCCESS) {
				clear_stamps();
				last_adjacent_sws_stamp = make_stamp();
				set_sync_flag(ADJACENT_SW41_CAPTURED);
				sw_tracking_state = SW12;
			}
		}
		break;
	}

	/* Sync error by lost of adjacent sync words */
	if(ADJACENT_SWS_LOST) {
		clear_sync_flags();
	}

	return sync_flags;
}

