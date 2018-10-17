/* Author: t.me/dmedser */

#ifndef DYNAMIC_BUF_H_
#define DYNAMIC_BUF_H_

#include <stdlib.h>
#include <stdint.h>

typedef struct dbuf_t {
	uint32_t *buf;
	uint32_t idx;
	uint32_t size;
} dbuf_t;

void add_to_dbuf(dbuf_t *dbuf, uint32_t new_value);
void clear_dbuf(dbuf_t *dbuf);

#endif /* DYNAMIC_BUF */
