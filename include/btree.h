#include <util.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------------------- */

#ifndef __BTREE_H_
#define __BTREE_H_

typedef struct _TNF_Node            TNF_Node;
typedef struct _TNF_LeafNode        TNF_LeafNode;
typedef struct _TNF_NodeResult      TNF_NodeResult;

/* ----------------------------------------------------------------------------- */
/* TonfaDB: TNF_Node struct */

#define INDENT_TO(__indent) {              \
	int __i;                               \
	for (__i = 0; __i < __indent; __i++) { \
		fprintf(stderr, "  ");             \
	}                                      \
}

#define isLeaf(node) (node->type & LEAF)
#define isOverLeaf(node) node->isOverLeaf
#define isNode(node)   (node->type & (BRANCH | ROOT))
#define child(node, idx) ((TNF_Node*)(node->child[idx]))
#define leaf(node, idx) ((TNF_LeafNode*)(node->leaf[idx]))
#define leafdata(leaf, idx) (leaf->data + ((idx) * SIZEOF_VOIDPTR_BYTE))

#define _BYTE 8
#define SIZEOF_VOIDPTR (sizeof(void*))
#define SIZEOF_VOIDPTR_BYTE (SIZEOF_VOIDPTR * _BYTE)

#define bucket_id size_t
#define BINTREE_BUCKET_MAXSIZE 4
#define BINTREE_LEAF_DATASIZE 4
#define BINTREE_BUCKET_OVERSIZE (BINTREE_BUCKET_MAXSIZE + 1)
#define BINTREE_CENTERING_KEY (BINTREE_BUCKET_MAXSIZE / 2 + 1)
#define DATA_CENTERING_KEY_BYTE (BINTREE_CENTERING_KEY * sizeof(void*))
#define BUCKET_CENTERING_KEY_BYTE (BINTREE_CENTERING_KEY * sizeof(bucket_id))

#define ROOT        1  // 00001
#define BRANCH      2  // 00010
#define LEAF        4  // 00100
#define BRANCH_LEAF 8  // 01000
#define NODE_TYPE   15 // 11111

// TODO: padding
struct _TNF_Node {
	size_t type;
	size_t size;
	size_t bsize;
	bool isOverLeaf;
	TNF_Node* parent;
	bucket_id bucket[BINTREE_BUCKET_OVERSIZE]; // enable bucket[:-1] + extra space for split node
	union {
		TNF_Node** child;
		TNF_LeafNode** leaf;
	};
} _TNF_Node;

struct _TNF_LeafNode {
	size_t type;
	size_t size; // <-- ?
	TNF_Node* parent;
	//bucket_id bucket;
	void* data;
} _TNF_LeafNode;

struct _TNF_NodeResult {
	size_t size;
	void** data;
} _TNF_NodeResult;

#endif /* __BTREE_H_ */

/* ----------------------------------------------------------------------------- */
/* prototype */

void Tree_print(TNF_Node* node, void(*printLeaf)(TNF_LeafNode*));
TNF_Node* Tree_create();
void Tree_start(char* dbname, char* tblname);
TNF_Node* Tree_add(TNF_Node* node, bucket_id id, void* data);
void* Tree_get(TNF_Node* node, bucket_id id);
TNF_NodeResult* Tree_search(TNF_Node* node, void* threshold, int (*compare)(void* l, void* r));
void Tree_save(TNF_Node* node);
void Tree_exit(TNF_Node* node);

#ifdef __cplusplus
}
#endif
