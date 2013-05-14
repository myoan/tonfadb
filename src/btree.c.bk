#include <btree.h>

/* ----------------------------------------------------------------------------- */
TNF_Node* createRoot() {
	TNF_Node* node = (TNF_Node*)TNF_malloc(sizeof(TNF_Node));
	node->type = ROOT;
	node->lsize = 0;
	node->bsize = 0;
	node->parent = NULL;
	node->isOverLeaf = false;
	memset(node->data, '\0', SIZEOF_VOIDPTR * NODE_LENGTH);
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

/* ----------------------------------------------------------------------------- */
void Tree_start(char* dbname, char* tblname) {
}

/* ----------------------------------------------------------------------------- */

#define SWAP(T, a, b) {	\
		T _tmp = a;		\
		a = b;			\
		b = _tmp;		\
	}

void sortIndex(TNF_Node* node) {
	//bubble sort
	//[TODO] implement better sort algorithm.
	size_t i, j, max_i = 0;
	for (i = BINTREE_BUCKET_MAXSIZE; i > 0; i--) {
		for (j = 0; j < i; j++) {
			if (B(node, max_i) < B(node, j)) {
				max_i = j;
			}
		}
		SWAP(bucket_id, B(node, i-1), B(node, max_i));
		max_i = 0;
	}
}

void printBucket(bucket_id* p) {
	int i;
	fprintf(stderr, "[");
	for (i = 0; i < BINTREE_BUCKET_OVERSIZE; i++) {
		fprintf(stderr, "%lu, ", p[i]);
	}
	fprintf(stderr, "]\n");
}

int insertBucket(TNF_Node* node, bucket_id id) {
	int i = 0;
	sortIndex(node);
	bucket_id bucket;
	FOREACH_BUCKET(node, bucket) {
		if (bucket > id) {
			size_t len = Nsize(node) - _bi;
			memcpy(node->data + Bidx(_bi+1), node->data + Bidx(_bi), SIZEOF_VOIDPTR * len);
			setB(node, i, id);
			memcpy(node->data + Bidx(_bi+1), node->data + Bidx(_bi), SIZEOF_VOIDPTR * len);
			memset(node->data + Bidx(_bi), '\0', SIZEOF_VOIDPTR);
			return i;
		}
	}
	return i;
}

int getBucket(TNF_Node* node, bucket_id id) {
	int i = 0;
	sortIndex(node);
	bucket_id bucket;
	FOREACH_BUCKET(node, bucket) {
		if (bucket > id) {
			return _bi;
		}
	}
	return _bi;
}

TNF_Node* getRoot(TNF_Node* node) {
	if (node->parent != NULL) {
		return getRoot(node->parent);
	}
	return node;
}

/*
TNF_Node** insertParentBucket(TNF_Node* node, bucket_id id, bool isOverLeaf) {
	int idx = insertBucket(node, id);
	TNF_Node* ch_node;
	int i = 0;
	for (i = 0; i < 2; i++) {
		ch_node = child(node, idx + i);
		node->child[idx + i] = (TNF_Node*)createBranch(node);
		node->child[idx + i]->isOverLeaf = isOverLeaf;
	}
	return node->child + idx;
}

TNF_Node* splitNode(TNF_Node* node, bucket_id id) {
	todo("expand blanced-tree node");
	TNF_Node* parent = node->parent;
	if (parent == NULL) { // node is root
		parent = createRoot();
		node->type = BRANCH;
		node->parent = parent;
	}

	bucket_id* buckets = node->bucket;
	bucket_id center_key = buckets[BINTREE_CENTERING_KEY - 1];
	printBucket(buckets);

	if (parent->lsize >= BINTREE_BUCKET_OVERSIZE) {
		fprintf(stderr, "split parent!!\n");
		asm("int3");
		splitNode(node->parent, center_key);
	}
	TNF_Node** nodes = insertParentBucket(parent, center_key, true);
	TNF_Node* fwdNode = nodes[0];
	TNF_Node* bwdNode = nodes[1];
	fprintf(stderr, "node->bsize: %lu, size: %lu\n", node->bsize, node->lsize);

	memcpy(fwdNode->bucket, buckets, sizeof(bucket_id) * (BINTREE_CENTERING_KEY - 1));
	memcpy(bwdNode->bucket, buckets + BINTREE_CENTERING_KEY, sizeof(bucket_id) * (BINTREE_CENTERING_KEY - 1));

	memcpy(fwdNode->leaf, node->leaf, sizeof(TNF_LeafNode*) * BINTREE_CENTERING_KEY);
	memcpy(bwdNode->leaf, node->leaf + BINTREE_CENTERING_KEY, sizeof(TNF_LeafNode*) * (node->lsize - (BINTREE_CENTERING_KEY)));

	// TODO bwdNodeの一番最後のleafのデータがうまくコピーできてない
	// bucket: 5, leaf: 6とbucket: 5, leaf: 5の場合の対応付けがだめ?
	//fprintf(stderr, "node->data[%lu]: %s\n", node->lsize-2, (char*)leaf(node, node->lsize-2)->data);
	//fprintf(stderr, "node->data[%lu]: %s\n", node->lsize-1, (char*)leaf(node, node->lsize-1)->data);
	fwdNode->lsize = BINTREE_CENTERING_KEY;
	bwdNode->lsize = node->lsize - (BINTREE_CENTERING_KEY);
	fwdNode->bsize = BINTREE_CENTERING_KEY - 1;
	bwdNode->bsize = BINTREE_CENTERING_KEY - 1;
	//fprintf(stderr, "bwd->data[%lu]: %s\n", bwdNode->lsize-1, (char*)leaf(bwdNode, bwdNode->lsize-1)->data);
	TNF_free(node);
	return parent;
}
*/

TNF_Node* Tree_add(TNF_Node* node, bucket_id id, void* data) {
	TNF_Node* ret;
	int idx;
	if (isOverLeaf(node)) {
		idx = insertBucket(node, id);
		fprintf(stderr, "id: %lu\n", id);
		TNF_Node* ch_node = L(node, idx);
		if (ch_node == NULL) {
			ch_node = (TNF_Node*)createLeaf(node, id);
		}
		TNF_LeafNode* leaf = (TNF_LeafNode*)ch_node;
		leaf->data = data;
		setL(node, idx, leaf);
		//if (node->bsize >= BINTREE_BUCKET_OVERSIZE) {
		//	node = splitNode(node, id);
		//}
	}
	else {
		idx = getBucket(node, id);
		Tree_add((TNF_Node*)L(node, idx), id, data);
	}
	ret = getRoot(node);
	return ret;
}

/* ----------------------------------------------------------------------------- */
void* Tree_get(TNF_Node* node, bucket_id id) {
	return 0;
}

/* ----------------------------------------------------------------------------- */


TNF_NodeResult* Tree_search(TNF_Node* node, void* threshold, int (*compare)(void* l, void* r)) {
/*
	size_t i;
	TNF_NodeResult* ret = (TNF_NodeResult*)TNF_malloc(sizeof(TNF_NodeResult));
	ret->lsize = 0;
	ret->data = (void*)TNF_malloc(SIZEOF_VOIDPTR * 32);
	if (isNode(node)) {
		for (i = 0; i < node->lsize; i++) {
			if (compare((void*)(&node->bucket[i]), threshold)) {
				Tree_search(child(node, i), threshold, compare);
			}
		}
	}
	else if (isLeaf(node)) {
		for (i = 0; i < node->lsize; i++) {
			if (child(node, i) == NULL) continue;
			//TNF_LeafNode* leaf = leaf(node, i);
			//if (compare((void*)(&leaf->base), threshold)) {
			//	todo("expand TNF_NodeResult");
			//	//if (ret->lsize == ret->maxsize) {
			//	//	expand(ret);
			//	//}
			//	for (j = 0; j < leaf->cursor; j++) {
			//		if (compare((void*)(&leaf->bucket[j]), threshold)) {
			//			memcpy(ret->data + (SIZEOF_VOIDPTR * ret->lsize), leafdata(leaf, j), SIZEOF_VOIDPTR);
			//			ret->lsize++;
			//		}
			//	}
			//}
		}
	}
	return ret;
*/
	return NULL;
}

/* ----------------------------------------------------------------------------- */
void Tree_save(TNF_Node* node) {
}

/* ----------------------------------------------------------------------------- */
void Tree_exit(TNF_Node* node) {
	TNF_free(node);
}

/* ----------------------------------------------------------------------------- */

//void print(TNF_Node* node, int indent, void (*printLeaf)(TNF_LeafNode*)) {
//	if (node == NULL) {
//		return;
//	}
//	if (isNode(node)) {
//		Tree_print(node, printLeaf);
//	}
//	else if (isLeaf(node)) {
//		TNF_LeafNode* leaf = (TNF_LeafNode*)node;
//		INDENT_TO(indent);
//		printLeaf(leaf);
//	}
//}
//
//void Tree_print(TNF_Node* node, void (*printLeaf)(TNF_LeafNode*)) {
//
//	static int indent = 0;
//	size_t bi /* bucket_idx */, li = 1 /* leaf_idx */;
//	indent++;
//	print(node->child[0], indent+1, printLeaf);
//	for (bi = 0; bi < node->lsize; bi++, li++) {
//		if (bi < node->bsize) {
//			INDENT_TO(indent);
//			fprintf(stderr, "[%lu]: \n", node->bucket[bi]);
//		}
//		print(node->child[li], indent+1, printLeaf);
//	}
//	indent--;
//}

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
/* ============================================================================= */

int int_cmp(void* l, void* r) {
	int* base = (int*)l;
	int* id = (int*)r;
	//fprintf(stderr, "r(%d) > l(%d)\n", *base, *id);
	return (*base > *id) ? 1 : 0;
}

void printTest(TNF_LeafNode* leaf) {
	char* str;
	str = (char*)leaf->data;
	fprintf(stderr, "%s\n", str);
}

TNF_Node* Add_mocData(TNF_Node* root, size_t data) {
	char* addr = (char*)malloc(sizeof(char) * 16);
	sprintf(addr, "hello%lu", data);
	root = Tree_add(root, data, (void*)addr);
	Tree_print(root, printTest);
	return root;
}

/*
int main(int argc, char const* argv[])
{
	TNF_Node* root = Tree_create();
	//Tree_start("dbname", "tblname"); // always working at another process
	size_t data;
	data = 20;
	root = Add_mocData(root, data);
	data = 7;
	root = Add_mocData(root, data);
	data = 32;
	root = Add_mocData(root, data);
	//data = 78;
	//root = Add_mocData(root, data);
	//data = 52;
	//root = Add_mocData(root, data);
	//data = 15;
	//root = Add_mocData(root, data);
	//data = 99;
	//root = Add_mocData(root, data);
	//data = 64;
	//root = Add_mocData(root, data);
	//data = 21;
	//root = Add_mocData(root, data);
	//data = 23;
	//root = Add_mocData(root, data);
	//data = 45;
	//root = Add_mocData(root, data);

	//asm("int3");
	////size_t addr = Tree_get(root, bucket_id);
	//int th = 10;
	//TNF_NodeResult* list = Tree_search(root, (void*)&th, int_cmp);
	//for (i = 0; i < list->lsize; i++) {
	//	char** str = (char**)(list->data + (i * SIZEOF_VOIDPTR));
	//	fprintf(stderr, "ret: %s\n", *str);
	//}
	////Tree_save(root);
	Tree_exit(root);
	return 0;
}
*/
