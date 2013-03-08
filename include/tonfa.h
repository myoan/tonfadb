#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TONFA_H_
#define __TONFA_H_

#define _KB 1024
#define _MB (_KB * _KB)
#define COLUMN_SIZE 32
#define FILE_SIZE   (4 * _KB)
#define FILE_SUFFIX ".tnf"
#define FILE_SUFFIX_LEN 4 /* .tnf */

/* ----------------------------------------------------------------------------- */

typedef struct _TNF_filenode TNF_filenode;

typedef struct _TNF_libs {
	TNF_filenode* (*init)(TNF_filenode* node, char* name, int fd, size_t size);
	void (*write)(TNF_filenode* node, char* data, size_t size);
	char* (*read)(TNF_filenode* node, size_t size);
	void (*close)(TNF_filenode* node);
} TNF_libs;

typedef struct _TNF_objHeader {
	int id;
	TNF_libs* libs;
} TNF_objHeader;

struct _TNF_filenode {
	TNF_objHeader* h;
	int fd;
	char* name;
	size_t head_idx;
	size_t tail_idx;
	size_t cur_idx;
	size_t column_size;
	size_t size;
};

#define TNF_Malloc(size) _TNF_malloc(size)
#define TNF_Free(p) _TNF_free(p)

/* ============================================================================= */
/* prototype */
/* ----------------------------------------------------------------------------- */
/* utils */

void* _TNF_malloc(size_t size);
void _TNF_free(void* p);
size_t digit(int i);

/* ----------------------------------------------------------------------------- */
/* filenode methods */

TNF_filenode* filenode_init(TNF_filenode* node, char* name, int fd, size_t table_size);
void filenode_write(TNF_filenode* node, char* data, size_t table_idx);
char* filenode_read(TNF_filenode* node, size_t size);
void filenode_close(TNF_filenode* node);

/* ----------------------------------------------------------------------------- */

//TNF_libs filenode_libs = {filenode_init, filenode_write, filenode_read, filenode_close};

#ifdef __cplusplus
}
#endif

#endif /* __TONFA_H_ */
