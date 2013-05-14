#include <btree.h>

/* ----------------------------------------------------------------------------- */
/* [create] */

void initBucket(TNF_Node* node) {
	int i;
	for (i = 0; i < BUCKET_LENGTH; i++) {
		B(node, i) = (void*)-1;
	}
}

TNF_Node* createRoot() {
	TNF_Node* node = (TNF_Node*)TNF_malloc(sizeof(TNF_Node));
	node->type = ROOT;
	node->lsize = 0;
	node->bsize = 0;
	node->parent = NULL;
	//node->isOverLeaf = false;
	memset(node->data, '\0', SIZEOF_VOIDPTR * NODE_LENGTH);
	initBucket(node);
	//memset(node->bucket, -1, sizeof(bucket_id) * BINTREE_BUCKET_OVERSIZE);
	//memset(node->child, '\0', sizeof(TNF_Node*) * BINTREE_BUCKET_OVERSIZE);
	return node;
}

TNF_Node* createBranch(TNF_Node* parent) {
	TNF_Node* node = (TNF_Node*)TNF_malloc(sizeof(TNF_Node));
	node->type = BRANCH;
	node->lsize = 0;
	node->bsize = 0;
	node->parent = parent;
	memset(node->data, '\0', SIZEOF_VOIDPTR * NODE_LENGTH);
	initBucket(node);
	return node;
}

TNF_Node* createLBranch(TNF_Node* parent) {
	TNF_Node* node = (TNF_Node*)TNF_malloc(sizeof(TNF_Node));
	node->type = LEAF_BRANCH;
	node->lsize = 0;
	node->bsize = 0;
	node->parent = parent;
	memset(node->data, '\0', SIZEOF_VOIDPTR * NODE_LENGTH);
	initBucket(node);
	return node;
}

TNF_LeafNode* createLeaf(TNF_Node* node, bucket_id id) {
	TNF_LeafNode* leaf = (TNF_LeafNode*)TNF_malloc(sizeof(TNF_LeafNode));
	leaf->type = LEAF;
	leaf->parent = node;
	return leaf;
}

TNF_Node* Tree_create() {
	TNF_Node* ret = createRoot();
	//ret->isOverLeaf = true;
	return ret;
}

#define SWAP(T, a, b) {	\
		T _tmp = (T)a;		\
		a = (void*)b;		\
		b = (void*)_tmp;	\
	}

/* ----------------------------------------------------------------------------- */
/* [update] */

void node_datacpy(TNF_Node* to_node, size_t to_idx, TNF_Node* from_node, size_t from_idx, size_t size) {
	DBG_assert(size <= NODE_LENGTH);
	size_t base = size / 2;
	size_t mask = size % 2;
	memcpy(to_node->data + to_idx, from_node->data + from_idx, byte(size));
	to_node->bsize += base;
	to_node->lsize += base + mask;
	from_node->bsize -= base;
	from_node->lsize -= base + mask;
}

void SortIndex(TNF_Node* node) {
	//bubble sort
	//[TODO] implement better sort algorithm.
	size_t i, j, max_i = 0;
	for (i = BINTREE_BUCKET_MAXSIZE; i > 0; i--) {
		for (j = 0; j < i; j++) {
			if (B(node, max_i) < B(node, j)) {
				max_i = j;
			}
		}
		SWAP(size_t, B(node, i-1), B(node, max_i));
		max_i = 0;
	}
}

int BucketIdx(TNF_Node* node, bucket_id id) {
	//int i = 0;
	SortIndex(node);
	bucket_id bucket;
	FOREACH_BUCKET(node, bucket) {
		if (bucket > id) {
			return _bi;
		}
	}
	return _bi;
}

void InsertBucket(TNF_Node* node, bucket_id id, size_t idx) {
	size_t len = Nsize(node) - idx;
	memcpy(node->data + Bidx(idx+1), node->data + Bidx(idx), byte(len));
	setB(node, idx, id);
	memcpy(node->data + Bidx(idx+1), node->data + Bidx(idx), byte(len));
	memset(node->data + Bidx(idx), '\0', SIZEOF_VOIDPTR);
}

TNF_Node* SplitNode(TNF_Node* node, bucket_id id);

TNF_Node* Tree_add(TNF_Node* node, bucket_id id, void* data) {
	size_t idx = BucketIdx(node, id);
	void* child = L(node, idx);
	if (!(isLB(node) || (isRoot(node) && Lsize(node) < LEAF_LENGTH))) {
		return Tree_add((TNF_Node*)child, id, data);
	}
	else {
		TNF_LeafNode* leaf = L(node, idx);
		if (leaf == NULL) {
			leaf = (TNF_LeafNode*)createLeaf(node, id);
		}
		leaf->id = id;
		leaf->data = data;
		setB(node, idx, id);
		setL(node, idx, (void*)leaf);
		//connectNextNode();
		if (Bsize(node) > BUCKET_LENGTH) {
			fprintf(stderr, "impl splitnode\n");
			SplitNode(node->parent, CenterBucket(node));
		}
	}
	return node;
}

TNF_Node* SplitNode(TNF_Node* node, bucket_id id) {
	TNF_Node* parent = node->parent;
	size_t idx = BucketIdx(node, id);
	InsertBucket(node, id, idx);
	if (Bsize(parent) > BUCKET_LENGTH) {
		parent = SplitNode(parent, CenterBucket(node));
	}
	TNF_Node* (*create)();
	create = (isLB(node))? createLBranch : createBranch;
	TNF_Node* r = create();
	TNF_Node* l = create();
	setL(parent, idx, (void*)l);
	setL(parent, idx+1, (void*)r);
	r->parent = parent;
	l->parent = parent;
	node_datacpy(l, 0, node, 0, 4);
	node_datacpy(r, 0, node, 5, 7);
	free(node);
	return r;
}

TNF_NodeResult* Tree_search(TNF_Node* node, void* threshold, int (*compare)(void* l, void* r)) {
	return NULL;
}

void Tree_print(TNF_Node* node, void (*printLeaf)(TNF_LeafNode*)) {
	size_t len = node->bsize + node->lsize;
	int i;
	for (i = 0; i < len; i++) {
		if (node->data[i] == NULL) return;
		if (i % 2 == 0) {
			printLeaf((TNF_LeafNode*)node->data[i]);
		}
		else {
			fprintf(stderr, "[%lu]: \n", (size_t)node->data[i]);
		}
	}
}

