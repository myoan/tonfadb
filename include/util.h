//#include <tonfa.h>
#include <stdarg.h>

#ifndef TNF_UTIL_H_
#define TNF_UTIL_H_

#ifdef DEBUG

#define debug(fmt, ...) reportDBG(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define fixme(fmt, ...) reportDBG(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define todo(fmt, ...)  reportDBG(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define err(fmt, ...)   reportDBG(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)

#else

#define debug(fmt, ...)
#define fixme(fmt, ...)
#define todo(fmt, ...)
#define err(fmt, ...)

#endif /* DEBUG */

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
