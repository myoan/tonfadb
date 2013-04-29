#include <btree.h>

/* ----------------------------------------------------------------------------- */
TNF_Node* Tree_create() {
	TNF_Node* node = (TNF_Node*)TNF_malloc(sizeof(TNF_Node));
	node->type = BRANCH;
	node->size = 0;
	node->leaf = (void*)TNF_malloc(sizeof(TNF_LeafNode*) * BINTREE_BUCKET_OVERSIZE);
	fprintf(stderr, "leaf size: %lu\n", sizeof(TNF_LeafNode*) * BINTREE_BUCKET_OVERSIZE);
	memset(node->bucket, -1, sizeof(bucket_id) * BINTREE_BUCKET_OVERSIZE);
	return node;
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

static void sortIndex(bucket_id* bucket) {
	//bubble sort
	//[TODO] implement better sort algorithm.
	size_t i, j, max_i = 0;
	for (i = BINTREE_BUCKET_MAXSIZE; i > 0; i--) {
		for (j = 0; j < i; j++) {
			if (bucket[max_i] < bucket[j]) {
				max_i = j;
			}
		}
		SWAP(size_t, bucket[i-1], bucket[max_i]);
		max_i = 0;
	}
}

void printIndex(bucket_id* p) {
	int i;
	fprintf(stderr, "[");
	for (i = 0; i < BINTREE_BUCKET_MAXSIZE; i++) {
		fprintf(stderr, "%lu, ", p[i]);
	}
	fprintf(stderr, "]\n");
}

static int insertIndex(TNF_Node* node, bucket_id id) {
	bucket_id* buckets = node->bucket;
	int i = 0;
	sortIndex(buckets);
	for (i; i < BINTREE_BUCKET_MAXSIZE; i++) {
		if (buckets[i] > id) {
			// insert !!
			memcpy(buckets + (i+1), buckets + i, sizeof(bucket_id) * (BINTREE_BUCKET_MAXSIZE - i));
			buckets[i] = id;
			memcpy(node->child + (i+1), node->child + i, sizeof(TNF_Node*) * (BINTREE_BUCKET_MAXSIZE - i));
			memset(node->child + i, '\0', sizeof(TNF_Node*));
			printIndex(buckets);
			node->size++;
			return i;
		}
	}
	return i;
}

static TNF_Node* getRoot(TNF_Node* node) {
	if (node->parent != NULL) {
		return getRoot(node->parent);
	}
	return node;
}

static TNF_LeafNode* getLeaf(TNF_Node* node, bucket_id id) {
	size_t i;
	for (i = 0; i < node->size; i++) {
		fprintf(stderr, "bucket(%lu): %lu, idx: %lu\n", i, node->bucket[i], id);
		if (node->bucket[i] > id) {
			return leaf(node, i);
		}
	}
	return NULL;
}

static TNF_LeafNode* createLeaf(TNF_Node* node, bucket_id id) {
	fprintf(stderr, "create\n");
	TNF_LeafNode* leaf = (TNF_LeafNode*)TNF_malloc(sizeof(TNF_LeafNode));
	leaf->bucket = id;
	leaf->type = LEAF;
	leaf->size = 0;
	leaf->parent = node;
	//node->size++;
	return leaf;
}

static TNF_Node* splitNode(TNF_Node* node, bucket_id id) {
	todo("expand bintree node");
	asm("int3");
//	bucket_id* buckets = node->bucket;
//	int idx = insertIndex(node, id);
//	bucket_id center_key = buckets[BINTREE_CENTERING_KEY];
//	TNF_Node* cur = splitNode(node, center_key);
//	cur->data[BINTREE_CENTERING_KEY] = record;
//	TNF_Node* root = getRoot(node);
//	return root;
	return node;
}

TNF_Node* Tree_add(TNF_Node* node, bucket_id id, void* data) {
	TNF_Node* ret;
	int idx = insertIndex(node, id);
	if (node->size >= BINTREE_BUCKET_MAXSIZE) {
		splitNode(node, id);
	}
	TNF_Node* ch_node = child(node, idx);
	if (ch_node == NULL) {
		ch_node = (TNF_Node*)createLeaf(node, id);
	}
	if (isLeaf(ch_node)) {
		TNF_LeafNode* leaf = (TNF_LeafNode*)ch_node;
		leaf->data = data;
		leaf->bucket = id;
		node->leaf[idx] = leaf;
	}
	else if (isNode(ch_node)) {
		Tree_add(ch_node, id, data);
	}
	ret = getRoot(ch_node);
	return ret;
}

/*
TNF_Node* Tree_add__(TNF_Node* node, bucket_id id, void* data) {
	TNF_Node* ret;
	assert(node != NULL);
	if (node->size > BINTREE_BUCKET_MAXSIZE) {
		goto filledBucket;
	}
	int idx = getIndex(node, id);

	TNF_LeafNode* leaf = getLeaf(node, id);
	if (leaf == NULL) {
		leaf = createLeaf(node, id);
	}
	//todo("random bucket");
	leaf->data = data;
	leaf->bucket = id;
	node->leaf[idx] = leaf;
	ret = node;

	//if (isNode(node)) {
	//	node = Tree_add(child(node, idx), id, data);
	//	ret = getRoot(node);
	//}
	//else if (isLeaf(node)) {
	//	TNF_LeafNode* leaf = getLeaf(node, id);
	//	if (leaf == NULL) {
	//		leaf = createLeaf(node, id);
	//	}
	//	//todo("random bucket");
	//	leaf->data = data;
	//	leaf->bucket = id;
	//	//fprintf(stderr, "leaf: %p\n", getLeaf(node, id));
	//	//memcpy(getLeaf(node, id), leaf, sizeof(TNF_LeafNode*));
	//	//TNF_LeafNode* ptr = node->leaf + idx;
	//	node->leaf[idx] = leaf;
	//	node->size++;
	//	ret = node;
	//}
	return ret;
	// bucket is filled!
}
*/

/* ----------------------------------------------------------------------------- */
void* Tree_get(TNF_Node* node, bucket_id id) {
	return 0;
}

/* ----------------------------------------------------------------------------- */


TNF_NodeResult* Tree_search(TNF_Node* node, void* threshold, int (*compare)(void* l, void* r)) {
	size_t i;
	TNF_NodeResult* ret = (TNF_NodeResult*)TNF_malloc(sizeof(TNF_NodeResult));
	ret->size = 0;
	ret->data = (void*)TNF_malloc(SIZEOF_VOIDPTR * 32);
	if (isNode(node)) {
		for (i = 0; i < node->size; i++) {
			if (compare((void*)(&node->bucket[i]), threshold)) {
				Tree_search(child(node, i), threshold, compare);
			}
		}
	}
	else if (isLeaf(node)) {
		for (i = 0; i < node->size; i++) {
			if (child(node, i) == NULL) continue;
			//TNF_LeafNode* leaf = leaf(node, i);
			//if (compare((void*)(&leaf->base), threshold)) {
			//	todo("expand TNF_NodeResult");
			//	//if (ret->size == ret->maxsize) {
			//	//	expand(ret);
			//	//}
			//	for (j = 0; j < leaf->cursor; j++) {
			//		if (compare((void*)(&leaf->bucket[j]), threshold)) {
			//			memcpy(ret->data + (SIZEOF_VOIDPTR * ret->size), leafdata(leaf, j), SIZEOF_VOIDPTR);
			//			ret->size++;
			//		}
			//	}
			//}
		}
	}
	return ret;
}

/* ----------------------------------------------------------------------------- */
void Tree_save(TNF_Node* node) {
}

/* ----------------------------------------------------------------------------- */
void Tree_exit(TNF_Node* node) {
	TNF_free(node);
}

/* ----------------------------------------------------------------------------- */

void print(TNF_Node* node, int indent, void (*printLeaf)(TNF_LeafNode*)) {
	if (isNode(node)) {
		indent++;
		Tree_print(child(node, 0), printLeaf);
		indent--;
	}
	else if (isLeaf(node)) {
		TNF_LeafNode* leaf = (TNF_LeafNode*)node;
		INDENT_TO(indent+1);
		printLeaf(leaf);
	}
}

void Tree_print(TNF_Node* node, void (*printLeaf)(TNF_LeafNode*)) {

	static int indent = 0;
	size_t i, data_i = 1;
	print(node->child[0], indent+1, printLeaf);
	for (i = 0; data_i < node->size; i++, data_i++) {
		print(node->child[data_i], indent+1, printLeaf);
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
	fprintf(stderr, "[%lu]: %s\n", leaf->bucket, str);
}

#include <time.h>
int main(int argc, char const* argv[])
{
	bucket_id id = 0;
	TNF_Node* root = Tree_create();
	//Tree_start("dbname", "tblname"); // always working at another process
	size_t i;
	char* addr;
	size_t data;
	srand(time(NULL));
	for (i = 0; i < 4; i++) {
		data = rand() % 100;
		addr = (char*)malloc(sizeof(char) * 16);
		sprintf(addr, "hello%lu[%lu]", data, i);
		Tree_add(root, data, (void*)addr);
	}
	Tree_print(root, printTest);
	////size_t addr = Tree_get(root, bucket_id);
	//int th = 10;
	//TNF_NodeResult* list = Tree_search(root, (void*)&th, int_cmp);
	//for (i = 0; i < list->size; i++) {
	//	char** str = (char**)(list->data + (i * SIZEOF_VOIDPTR));
	//	fprintf(stderr, "ret: %s\n", *str);
	//}
	////Tree_save(root);
	Tree_exit(root);
	return 0;
}
