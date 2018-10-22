/* Author: t.me/dmedser */

#include "sync.h"
#include "global_cfg.h"

#define SW1_REVERSED		0xE2400000
#define SW2_REVERSED		0x1DA00000
#define SW3_REVERSED		0xE2500000
#define SW4_REVERSED		0x1DB00000

#define SW_MASK				0xFFF00000

#define SW1_CAPTURED		((bit_stream & SW_MASK) == SW1_REVERSED)
#define SW2_CAPTURED		((bit_stream & SW_MASK) == SW2_REVERSED)
#define SW3_CAPTURED		((bit_stream & SW_MASK) == SW3_REVERSED)
#define SW4_CAPTURED		((bit_stream & SW_MASK) == SW4_REVERSED)

#define SUCCESS				TRUE
#define FAIL				FALSE

extern uint32_t bit_stream;
extern uint32_t bit_counter;

static sync_states sync_state = IDLE;
static buf_u32_t sw_stamps = {{0}, 0};

inline void clear_stamps(void) {
	sw_stamps.idx = 0;
}


inline void make_stamp(void) {
	sw_stamps.buf[sw_stamps.idx++] = bit_counter;
}


boolean find_adjacent_sws(void) {
	uint16_t i = 0;

	for(; i < sw_stamps.idx; i++) {

		if((bit_counter - sw_stamps.buf[i]) == BITRATE_BPS) {
			return SUCCESS;
		}

	}
	return FAIL;
}


void sync_process(void) {
	switch(sync_state) {
	case IDLE:
		if(SW1_CAPTURED) {
			make_stamp();
			sync_state = SW12;
		}
		else if(SW2_CAPTURED) {
			make_stamp();
			sync_state = SW23;
		}
		else if(SW3_CAPTURED) {
			make_stamp();
			sync_state = SW34;
		}
		else if(SW4_CAPTURED) {
			make_stamp();
			sync_state = SW41;
		}
		break;
	case SW12:
		if(SW1_CAPTURED) {
			make_stamp();
		}
		else if(SW2_CAPTURED) {
			if(find_adjacent_sws() == SUCCESS) {
				clear_stamps();
				make_stamp();
				sync_state = SW23;
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
				make_stamp();
				sync_state = SW34;
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
				make_stamp();
				sync_state = SW41;
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
				make_stamp();
				sync_state = SW12;
			}
		}
		break;
	}
}

