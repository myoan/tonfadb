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

typedef size_t bucket_id;

#define isLeaf(node) (node->type & LEAF)
#define isOverLeaf(node) node->isOverLeaf
#define isLB(node) (node->type & LEAF_BRANCH)
#define isRoot(node)   (node->type & ROOT)
#define isNode(node)   (node->type & (BRANCH | ROOT))

#define _BYTE 8
#define SIZEOF_VOIDPTR (sizeof(void*))
#define SIZEOF_VOIDPTR_BYTE (SIZEOF_VOIDPTR * _BYTE)
#define byte(size) (size * SIZEOF_VOIDPTR)

#define BINTREE_BUCKET_MAXSIZE 4
#define BINTREE_LEAF_DATASIZE 4
#define BINTREE_BUCKET_OVERSIZE (BINTREE_BUCKET_MAXSIZE + 1)
#define BINTREE_CENTERING_KEY (BINTREE_BUCKET_MAXSIZE / 2 + 1)
#define DATA_CENTERING_KEY_BYTE (BINTREE_CENTERING_KEY * sizeof(void*))
#define BUCKET_CENTERING_KEY_BYTE (BINTREE_CENTERING_KEY * sizeof(bucket_id))
#define CenterBucket(node) (bucket_id)node->data[Bidx(BINTREE_CENTERING_KEY)]

#define ROOT        1  // 00001
#define BRANCH      2  // 00010
#define LEAF        4  // 00100
#define LEAF_BRANCH 8  // 01000
#define NODE_TYPE   15 // 11111

#define BUCKET_LENGTH 5
#define LEAF_LENGTH   (BUCKET_LENGTH + 1)
#define NODE_LENGTH   (BUCKET_LENGTH + LEAF_LENGTH)

#define Bidx(idx)  (2 * (idx) + 1)
#define Lidx(idx)  (2 * (idx))
#define Bsize(node)  (node->bsize)
#define Lsize(node)  (node->lsize)
#define Nsize(node)  (node->bsize + node->lsize)
#define B(node, idx)  node->data[Bidx(idx)]
#define L(node, idx)  node->data[Lidx(idx)]
#define leafdata(leaf, idx) (leaf->data + (Lidx(idx) * SIZEOF_VOIDPTR_BYTE))

#define setB(node, idx, id) {\
		B(node,idx)  = (void*)id;	\
		node->bsize++;				\
	}

#define setL(node, idx, data) {\
		L(node,idx)  = (void*)data;	\
		node->lsize++;				\
	}

#define foreach(a, e) \
		size_t i; \
		for (i = 0; i < 8, e = a[i]; i++) \

#define FOREACH_BUCKET(node, bucket) \
		size_t _bi; \
		for (_bi = 0; _bi < Bsize(node), bucket = (size_t)B(node, _bi); _bi++)\

#define FOREACH_NODE(node, ch) \
		size_t _ni; \
		for (_ni = 0; _ni < Lsize(node), ch = (TNF_Node*)L(node, _ni); _ni++) \

#define FOREACH_LEAF(node, leaf) \
		size_t _li; \
		for (_li = 0; _li < Lsize(node), node = (TNF_LeafNode*)L(node, _li); _li++) \

#define LSHIFT_BUCKET(node, idx, n) { \
		size_t _len = Nsize(node) - idx; \
		memcpy(node->data + Bidx(idx) - (2 * n), node->data + Bidx(idx), SIZEOF_VOIDPTR * _len); \
		memset(node->data + Bidx(idx), '\0', SIZEOF_VOIDPTR * 2 * n); \
	}

#define RSHIFT_BUCKET(node, idx, n) { \
		size_t _len = Nsize(node) - idx; \
		memcpy(node->data + Bidx(idx) + (2 * n), node->data + Bidx(idx), SIZEOF_VOIDPTR * _len); \
		memset(node->data + Bidx(idx), '\0', SIZEOF_VOIDPTR * 2 * n); \
	}

// TODO: padding
struct _TNF_Node {
	size_t type;
	size_t bsize;
	size_t lsize;
	//bool isOverLeaf;
	TNF_Node* parent;
	void* data[NODE_LENGTH];
} _TNF_Node;

struct _TNF_LeafNode {
	size_t type;
	TNF_Node* parent;
	bucket_id id;
	void* data;
} _TNF_LeafNode;

struct _TNF_NodeResult {
	size_t size;
	void** data;
} _TNF_NodeResult;

#endif /* __BTREE_H_ */

/* ----------------------------------------------------------------------------- */
/* prototype */

extern void Tree_print(TNF_Node* node, void(*printLeaf)(TNF_LeafNode*));
extern TNF_Node* Tree_create();
extern void Tree_start(char* dbname, char* tblname);
extern TNF_Node* Tree_add(TNF_Node* node, bucket_id id, void* data);
extern void* Tree_get(TNF_Node* node, bucket_id id);
extern TNF_NodeResult* Tree_search(TNF_Node* node, void* threshold, int (*compare)(void* l, void* r));
extern void Tree_save(TNF_Node* node);
extern void Tree_exit(TNF_Node* node);

extern TNF_Node* createRoot();
extern TNF_Node* createBranch(TNF_Node* parent);
extern TNF_LeafNode* createLeaf(TNF_Node* node, bucket_id id);
extern void printBucket(bucket_id* p);
extern void InsertBucket(TNF_Node* node, bucket_id id, size_t idx);
extern int getBucket(TNF_Node* node, bucket_id id);
extern TNF_Node* getRoot(TNF_Node* node);
extern void node_datacpy(TNF_Node* to_node, size_t to_idx, TNF_Node* from_node, size_t from_idx, size_t size);

#ifdef __cplusplus
}
#endif
