#include "dynamic_buf.h"

#define DBUF_IS_FULL		(dbuf->idx == (dbuf->size - 1))
#define CAPACITY_DELTA		10

void add_to_dbuf(dbuf_t *dbuf, uint32_t new_value) {
	if(DBUF_IS_FULL) {
		dbuf->buf = (uint32_t *)realloc(dbuf->buf, dbuf->size + CAPACITY_DELTA);
		dbuf->size += CAPACITY_DELTA;
	}
	dbuf->buf[dbuf->idx++] = new_value;
}


void clear_dbuf(dbuf_t *dbuf) {
	free(dbuf->buf);
	dbuf->idx = 0;
	dbuf->size = 0;
}

