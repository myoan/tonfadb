#include <btree.h>

/* ----------------------------------------------------------------------------- */
static TNF_Node* createRoot() {
	TNF_Node* node = (TNF_Node*)TNF_malloc(sizeof(TNF_Node));
	node->type = ROOT;
	node->size = 0;
	node->parent = NULL;
	node->isOverLeaf = false;
	node->child = (void*)TNF_malloc(sizeof(TNF_Node*) * BINTREE_BUCKET_OVERSIZE);
	fprintf(stderr, "leaf size: %lu\n", sizeof(TNF_Node*) * BINTREE_BUCKET_OVERSIZE);
	memset(node->bucket, -1, sizeof(bucket_id) * BINTREE_BUCKET_OVERSIZE);
	return node;
}

static TNF_Node* createBranch(TNF_Node* parent) {
	TNF_Node* node = (TNF_Node*)TNF_malloc(sizeof(TNF_Node));
	node->type = ROOT;
	node->size = 0;
	node->parent = parent;
	node->isOverLeaf = false;
	node->child = (void*)TNF_malloc(sizeof(TNF_Node*) * BINTREE_BUCKET_OVERSIZE);
	fprintf(stderr, "leaf size: %lu\n", sizeof(TNF_Node*) * BINTREE_BUCKET_OVERSIZE);
	memset(node->bucket, -1, sizeof(bucket_id) * BINTREE_BUCKET_OVERSIZE);
	return node;
}

static TNF_LeafNode* createLeaf(TNF_Node* node, bucket_id id) {
	TNF_LeafNode* leaf = (TNF_LeafNode*)TNF_malloc(sizeof(TNF_LeafNode));
	leaf->type = LEAF;
	leaf->size = 0;
	leaf->parent = node;
	return leaf;
}

TNF_Node* Tree_create() {
	TNF_Node* ret = createBranch(NULL);
	ret->isOverLeaf = true;
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

void printBucket(bucket_id* p) {
	int i;
	fprintf(stderr, "[");
	for (i = 0; i < BINTREE_BUCKET_OVERSIZE; i++) {
		fprintf(stderr, "%lu, ", p[i]);
	}
	fprintf(stderr, "]\n");
}

static int insertBucket(TNF_Node* node, bucket_id id) {
	bucket_id* buckets = node->bucket;
	int i = 0;
	sortIndex(buckets);
	for (i = 0; i < BINTREE_BUCKET_OVERSIZE; i++) {
		if (buckets[i] > id) {
			memcpy(buckets + (i+1), buckets + i, sizeof(bucket_id) * (BINTREE_BUCKET_MAXSIZE - i));
			buckets[i] = id;
			memcpy(node->child + (i+1), node->child + i, sizeof(TNF_Node*) * (BINTREE_BUCKET_MAXSIZE - i));
			memset(node->child + i, '\0', sizeof(TNF_Node*));
			node->size++;
			return i;
		}
	}
	return i;
}

static int getBucket(TNF_Node* node, bucket_id id) {
	bucket_id* buckets = node->bucket;
	int i = 0;
	sortIndex(buckets);
	for (i = 0; i < BINTREE_BUCKET_OVERSIZE; i++) {
		if (buckets[i] > id) {
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

static TNF_Node** insertParentBucket(TNF_Node* node, bucket_id id, bool isOverLeaf) {
	int idx = insertBucket(node, id);
	TNF_Node* ch_node;
	int i = 0;
	for (i = 0; i < 2; i++) {
		ch_node = child(node, idx + i);
		if (ch_node == NULL) {
			node->child[idx + i] = (TNF_Node*)createBranch(node);
			node->child[idx + i]->isOverLeaf = isOverLeaf;
		}
	}
	return node->child + idx;
}

static TNF_Node* splitNode(TNF_Node* node, bucket_id id) {
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

	//if (parent->size >= BINTREE_BUCKET_OVERSIZE) {
	//	splitNode(node->parent, center_key);
	//}
	TNF_Node** nodes = insertParentBucket(parent, center_key, true);
	TNF_Node* fwdNode = nodes[0];
	TNF_Node* bwdNode = nodes[1];

	memcpy(fwdNode->bucket, buckets, sizeof(bucket_id) * (BINTREE_CENTERING_KEY - 1));
	memcpy(bwdNode->bucket, buckets + BINTREE_CENTERING_KEY, sizeof(bucket_id) * BINTREE_CENTERING_KEY - 1);

	memcpy(fwdNode->leaf, node->leaf, sizeof(TNF_LeafNode*) * BINTREE_CENTERING_KEY);
	memcpy(bwdNode->leaf, node->leaf + BINTREE_CENTERING_KEY, sizeof(TNF_LeafNode*) * (BINTREE_CENTERING_KEY - 1));

	fwdNode->size = (BINTREE_CENTERING_KEY);
	bwdNode->size = BINTREE_CENTERING_KEY - 1;
	TNF_free(node);
	return parent;
}

TNF_Node* Tree_add(TNF_Node* node, bucket_id id, void* data) {
	TNF_Node* ret;
	int idx;
	if (isOverLeaf(node)) {
		idx = insertBucket(node, id);
		TNF_Node* ch_node = child(node, idx);
		if (ch_node == NULL) {
			ch_node = (TNF_Node*)createLeaf(node, id);
		}
		TNF_LeafNode* leaf = (TNF_LeafNode*)ch_node;
		leaf->data = data;
		node->leaf[idx] = leaf;
		if (node->size >= BINTREE_BUCKET_OVERSIZE) {
			node = splitNode(node, id);
		}
	}
	else {
		idx = getBucket(node, id);
		Tree_add(node->child[idx], id, data);
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
	if (node == NULL) {
		return;
	}
	if (isNode(node)) {
		Tree_print(node, printLeaf);
	}
	else if (isLeaf(node)) {
		TNF_LeafNode* leaf = (TNF_LeafNode*)node;
		INDENT_TO(indent);
		printLeaf(leaf);
	}
}

void Tree_print(TNF_Node* node, void (*printLeaf)(TNF_LeafNode*)) {

	static int indent = 0;
	size_t i, data_i = 1;
	indent++;
	print(node->child[0], indent+1, printLeaf);
	for (i = 0; i < node->size; i++, data_i++) {
		INDENT_TO(indent);
		fprintf(stderr, "[%lu]: \n", node->bucket[i]);
		print(node->child[data_i], indent+1, printLeaf);
	}
	indent--;
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

#include <time.h>
int main(int argc, char const* argv[])
{
	TNF_Node* root = Tree_create();
	//Tree_start("dbname", "tblname"); // always working at another process
	size_t i;
	char* addr;
	size_t data;
	srand(time(NULL));
	for (i = 0; i < 8; i++) {
		data = rand() % 100;
		addr = (char*)malloc(sizeof(char) * 16);
		sprintf(addr, "hello%lu[%lu]", data, i);
		root = Tree_add(root, data, (void*)addr);
		Tree_print(root, printTest);
	}
	//asm("int3");
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
