#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <assert.h>


#ifndef TNF_UTIL_H_
#define TNF_UTIL_H_

#ifdef DEBUG

#define debug(fmt, ...) reportDBG(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define fixme(fmt, ...) reportFIXME(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define todo(fmt, ...)  reportTODO(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define err(fmt, ...)   reportERR(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)

#else

#define debug(fmt, ...)
#define fixme(fmt, ...)
#define todo(fmt, ...)
#define err(fmt, ...)

#endif /* DEBUG */

#define TNF_malloc(s) _TNF_malloc(s)
#define TNF_free(p)   _TNF_free(p)
/* ---------------------------------------------------------------------- */
/* [array] */

#define ARRAY_INIT_SIZE 4

struct _TNF_size_tArray {
	size_t idx;
	size_t maxsize;
	size_t* buf;
};

typedef struct _TNF_size_tArray TNF_size_tArray;
/* ---------------------------------------------------------------------- */
/* [prototype functions] */

void reportDBG(const char *file, const char *func, int line, const char *fmt, ...);
void reportFIXME(const char *file, const char *func, int line, const char *fmt, ...);
void reportTODO(const char *file, const char *func, int line, const char *fmt, ...);
void reportERR(const char *file, const char *func, int line, const char *fmt, ...);
TNF_size_tArray* create_size_tArray();
void expand_size_tArray(TNF_size_tArray* a);
void push_size_tArray(TNF_size_tArray* a, size_t value);
size_t pop_size_tArray(TNF_size_tArray* a);
void* _TNF_malloc(size_t size);
void _TNF_free(void* p);
size_t digit(int i);

#endif /* TNF_UTIL_H_ */
