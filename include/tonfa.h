#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <util.h>
#include <btree.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------------------- */

#ifndef __TONFA_H_
#define __TONFA_H_ 1

typedef struct _TNF_Record TNF_Record;

typedef struct _TNF_Index           TNF_Index;
typedef struct _TNF_RecordAddrMap   TNF_RecordAddrMap;
typedef struct _TNF_RecordMapHeader TNF_RecordMapHeader;

/* ----------------------------------------------------------------------------- */
/* TonfaDB: TNF_Record struct */

#define bucket_id size_t
#define _KB 1024
#define _MB (_KB * _KB)
#define COLUMN_SIZE 32
#define FILE_SIZE   (4 * _KB)
#define FILE_SUFFIX ".tnf"
#define FILE_SUFFIX_LEN 4 /* .tnf */

typedef struct _TNF_RecordLibs {
	TNF_Record* (*init)(TNF_Record* node, char* name, FILE* fp, size_t size);
	void (*write)(TNF_Record* node, char* data, size_t size);
	char* (*read)(TNF_Record* node, size_t size);
	void (*close)(TNF_Record* node);
} TNF_RecordLibs;

typedef struct _TNF_RecordHeader {
	int id;
	TNF_RecordLibs* libs;
} TNF_RecordHeader;

struct _TNF_Record {
	TNF_RecordHeader* h;
	FILE* fp;
	char* name;
	size_t head_idx;
	size_t tail_idx;
	size_t cur_idx;
	size_t column_size;
	size_t size;
};


/* ----------------------------------------------------------------------------- */
/* TonfaDB: TNF_Node struct */

struct _TNF_RecordMapHeader {
	bucket_id base;
	size_t size;
	size_t cursor;
} _TNF_RecordMapHeader;

struct _TNF_RecordAddrMap {
	bucket_id pad;
	bucket_id id;
	size_t addr;
} _TNF_RecordAddrMap;

/* ----------------------------------------------------------------------------- */
/* TonfaDB: TNF_Index struct */

struct _TNF_Index {
	char* dbname;
	char* tblname;
	size_t cursor;
	TNF_Node* tree;
	TNF_Record* record;
};

/* ----------------------------------------------------------------------------- */
/* TonfaDB: TNF_Command libs */

typedef enum {
	INST_NULL = 0,
	INST_UPDATE,
	INST_DELETE,
	INST_SELECT,
	INST_INSERT,
} TNF_Inst_t;

void TNF_CmdUpdate(TNF_Index* index, char** line);
void TNF_CmdDelete(TNF_Index* index, char** line);
void TNF_CmdSelect(TNF_Index* index, char** line);
void TNF_CmdInsert(TNF_Index* index, char** line);

void (*TNF_CmdLibs[])(TNF_Index*, char**);
//void (*TNF_CmdLibs[])(char**) = {
//	NULL, // INST_NULL
//	TNF_CmdUpdate, // INST_UPDATE
//	TNF_CmdDelete, // INST_DELETE
//	TNF_CmdSelect, // INST_SELECT
//	TNF_CmdInsert  // INST_INSERT
//};

/* ============================================================================= */
/* prototype */
/* ----------------------------------------------------------------------------- */
/* utils */

//#define TNF_malloc(size) _TNF_malloc(size)
//#define TNF_free(p) _TNF_free(p)

void* _TNF_malloc(size_t size);
void _TNF_free(void* p);
size_t digit(int i);

/* ----------------------------------------------------------------------------- */
/* filenode methods */

TNF_Record* TNF_CreateFile();
TNF_Record* Record_init(TNF_Record* node, char* name, FILE* fp, size_t table_size);
size_t Record_write(TNF_Record* node, char* data, size_t table_idx);
char* Record_read(TNF_Record* node, size_t size);
void Record_close(TNF_Record* node);
void readFile(TNF_Index* index, char* str);
void parse(TNF_Index* index, char** line);
char* getToken(char** line);

#endif /* __TONFA_H_ */

/* ----------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
