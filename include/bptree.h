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

#define BIN_NODE_NUM 5

typedef struct _TNF_BinNode TNF_BinNode;
typedef struct _TNF_BinBranch TNF_BinBranch;
typedef struct _TNF_BinLeaf TNF_BinLeaf;

struct _TNF_BinNode {
	size_t type;
	size_t depth;
	size_t bucket[BIN_NODE_NUM - 1];
	union {
		TNF_BinBranch* branch;
		TNF_BinLeaf*   leaf;
	};
} _TNF_BinNode;

struct _TNF_BinBranch {
	size_t bucket[BIN_NODE_NUM - 1];
	TNF_BinLeaf* leaf;
} _TNF_BinBranch;

struct _TNF_BinLeaf {
	size_t bucket[BIN_NODE_NUM - 1];
	size_t data[BIN_NODE_NUM];
} _TNF_BinLeaf;

/* ---------------------------------------------------------------------- */

#endif /* BPTREE_H_ */

#ifdef __cplusplus
}
#endif
