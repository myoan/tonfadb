#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <util.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BPTREE_H_
#define BPTREE_H_

/* ---------------------------------------------------------------------- */

#define bucket_id size_t
#define BIN_BUCKET_MAXSIZE 4
#define BIN_BUCKET_OVERSIZE (BIN_BUCKET_MAXSIZE + 1)
#define BIN_CENTERING_KEY (BIN_BUCKET_MAXSIZE / 2)
#define DATA_CENTERING_KEY_BYTE (BIN_CENTERING_KEY * sizeof(void*))
#define BUCKET_CENTERING_KEY_BYTE (BIN_CENTERING_KEY * sizeof(bucket_id))

#define ROOT        1  // 00001
#define BRANCH      2  // 00010
#define LEAF        4  // 00100
#define BRANCH_LEAF 8  // 01000
#define NODE_TYPE   15 // 11111

typedef struct _TNF_Node         TNF_Node;
typedef struct _TNF_Record       TNF_Record;
typedef struct _TNF_RecordHeader TNF_RecordHeader;

// TODO: padding
struct _TNF_Node {
	size_t type;
	size_t size;
	TNF_Node* parent;
	bucket_id bucket[BIN_BUCKET_OVERSIZE]; // enable bucket[:-1] + extra space for split node
	void** data;
} _TNF_Node;

struct _TNF_RecordHeader {
	bucket_id base;
	size_t size;
	size_t cursor;
} _TNF_RecordHeader;

struct _TNF_Record {
	bucket_id pad;
	bucket_id id;
	size_t data_idx;
} _TNF_Record;
/* ---------------------------------------------------------------------- */

#endif /* BPTREE_H_ */

#ifdef __cplusplus
}
#endif
