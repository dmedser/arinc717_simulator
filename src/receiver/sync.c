/* Author: t.me/dmedser */

#include "sync.h"
#include <Platform_types.h>

/* The synchronization process is as follows:
 * appearances of sync words are stored in
 * the buffer of stamps; in each state of
 * sync_process() pairs of adjacent sync
 * words (i.e. sync words with stamps, the
 * difference of which is equal SW_PERIOD)
 * are searched. On success, next adjacent
 * sync words are searched in the same way.
 */

#define SW1_ENCODED			0xFC660000
#define SW2_ENCODED			0x1FBD8000
#define SW3_ENCODED			0xFC66C000
#define SW4_ENCODED			0x1FBDE000

#define SW1_ENCODE_MSK		0xFFFF8000
#define SW2_ENCODE_MSK		0xFFFFC000
#define SW3_ENCODE_MSK		0xFFFFC000
#define SW4_ENCODE_MSK		0xFFFFE000

#define SW1_CAPTURED		((encoded_bit_stream & SW1_ENCODE_MSK) == SW1_ENCODED)
#define SW2_CAPTURED		((encoded_bit_stream & SW2_ENCODE_MSK) == SW2_ENCODED)
#define SW3_CAPTURED		((encoded_bit_stream & SW3_ENCODE_MSK) == SW3_ENCODED)
#define SW4_CAPTURED		((encoded_bit_stream & SW4_ENCODE_MSK) == SW4_ENCODED)

/* Values in ms */
#define SW_PERIOD			1000
#define DEVIATION			1
#define LOWER_BOUND			(SW_PERIOD - DEVIATION)
#define UPPER_BOUND			(SW_PERIOD + DEVIATION)

#define SUCCESS				TRUE
#define FAIL				FALSE

extern uint32_t cnt_1_ms;
extern uint32_t encoded_bit_stream;

static sync_states sync_state = IDLE;
static buf_t stamps = {{0}, 0};

boolean find_period(void) {
	uint16_t i = 0;

	for(; i < stamps.idx; i++) {

		uint32_t diff = cnt_1_ms - stamps.buf[i];

		if((LOWER_BOUND <= diff) && (diff <= UPPER_BOUND)) {
			return SUCCESS;
		}
	}
	return FAIL;
}


inline void make_stamp(void) {
	stamps.buf[stamps.idx++] = cnt_1_ms;
}


inline void clear_stamps(void) {
	stamps.idx = 0;
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
			if(find_period() == SUCCESS) {
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
			if(find_period() == SUCCESS) {
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
			if(find_period() == SUCCESS) {
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
			if(find_period() == SUCCESS) {
				clear_stamps();
				make_stamp();
				sync_state = SW12;
			}
		}
		break;
	}
}

