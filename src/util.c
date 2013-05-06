#include <tonfa.h>
#include <util.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/* [report] */

void reportDBG(const char *file, const char *func, int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap , fmt);
	fflush(stdout);
	fprintf(stderr, "DEBUG(%s:%d) ", func, line);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

void reportFIXME(const char *file, const char *func, int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap , fmt);
	fflush(stdout);
	fprintf(stderr, "FIXME(%s:%d) ", func, line);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

void reportTODO(const char *file, const char *func, int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap , fmt);
	fflush(stdout);
	fprintf(stderr, "TODO(%s:%d) ", func, line);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

void reportERR(const char *file, const char *func, int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap , fmt);
	fflush(stdout);
	fprintf(stderr, "ERR(%s:%d) ", func, line);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

/* -------------------------------------------------------------------------- */
/* [array] */

TNF_size_tArray* create_size_tArray() {
	TNF_size_tArray* ret = (TNF_size_tArray*)malloc(sizeof(TNF_size_tArray));
	memset(ret, '\0', sizeof(TNF_size_tArray));
	size_t* buf = (size_t*)malloc(sizeof(size_t) * ARRAY_INIT_SIZE);
	memset(buf, '\0', sizeof(size_t) * ARRAY_INIT_SIZE);
	ret->idx = 0;
	ret->maxsize = ARRAY_INIT_SIZE;
	ret->buf = buf;
	return ret;
}

void expand_size_tArray(TNF_size_tArray* a) {
	size_t maxsize = a->maxsize * 2;
	size_t* tmp;
	DBG_debug("expand array[%p]: %lu -> %lu", a, a->maxsize, maxsize);
	if ((tmp = (size_t*)realloc(a->buf, maxsize)) == NULL) {
		DBG_fixme("EXCEPTION! could not allocate\n");
	}
	else {
		free(a->buf);
		a->buf = tmp;
		a->maxsize = maxsize;
	}
	return;
}

void push_size_tArray(TNF_size_tArray* a, size_t value) {
	if (a->idx >= a->maxsize) {
		expand_size_tArray(a);
	}
	*(a->buf + a->idx) = value;
	a->idx++;
}

size_t pop_size_tArray(TNF_size_tArray* a) {
	if (a->idx < 1) {
		return -1;
	}
	size_t ret = *a->buf + (a->idx - 1);
	a->idx--;
	return ret;
}

/* -------------------------------------------------------------------------- */
/* [allocate] */

void* _TNF_malloc(size_t size) {
	DBG_debug("allocate: %lu", size);
	void* ret = (void*)malloc(size);
	memset(ret, '\0', size);
	return ret;
}

void _TNF_free(void* p) {
	DBG_debug("free: %p", p);
	free(p);
}

size_t digit(int i) {
	size_t ret = 0;
	while (i > 10) {
		ret++;
		i /= 10;
	}
	ret++;
	return ret;
}

#ifdef __cplusplus
}
#endif
