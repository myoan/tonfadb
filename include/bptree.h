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

#define ROOT        0 // 0000
#define BRANCH      1 // 0001
#define LEAF        2 // 0010
#define BRANCH_LEAF 4 // 0100
#define NODE_TYPE   7 // 0111

typedef struct _TNF_BinNode   TNF_BinNode;
typedef struct _TNF_Record    TNF_Record;

// TODO: padding
struct _TNF_BinNode {
	size_t type;
	size_t size;
	TNF_BinNode* parent;
	//size_t depth;
	bucket_id bucket[BIN_BUCKET_OVERSIZE]; // enable bucket[:-1] + extra space for split node
	void** data;
} _TNF_BinNode;

struct _TNF_Record {
	size_t dummy;
} _TNF_Record;
/* ---------------------------------------------------------------------- */

#endif /* BPTREE_H_ */

#ifdef __cplusplus
}
#endif
