#include <btree.h>

/* ----------------------------------------------------------------------------- */
TNF_Node* Tree_create() {
	TNF_Node* node = (TNF_Node*)TNF_malloc(sizeof(TNF_Node));
	node->type = LEAF;
	node->size = 0;
	node->leaf = (void*)TNF_malloc(SIZEOF_VOIDPTR_BYTE * (BINTREE_BUCKET_MAXSIZE + 1));
	memset(node->bucket, -1, sizeof(bucket_id) * BINTREE_BUCKET_OVERSIZE);
	return node;
}

/* ----------------------------------------------------------------------------- */
void Tree_start(char* dbname, char* tblname) {
}

/* ----------------------------------------------------------------------------- */

static int getIndex(TNF_Node* node, bucket_id id) {
	bucket_id* buckets = node->bucket;
	int i;
	for (i = 0; i < BINTREE_BUCKET_MAXSIZE; i++) {
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

static TNF_LeafNode* getLeaf(TNF_Node* node, bucket_id id) {
	size_t i;
	for (i = 0; i < node->size; i++) {
		//fprintf(stderr, "bucket(%lu): %lu, idx: %lu\n", i, node->bucket[i], id);
		if (node->bucket[i] > id) {
			return leaf(node, i);
		}
	}
	return NULL;
}

static TNF_LeafNode* createLeaf(TNF_Node* node, bucket_id id) {
	TNF_LeafNode* leaf = (TNF_LeafNode*)TNF_malloc(sizeof(TNF_LeafNode));
	leaf->base = id;
	leaf->size = BINTREE_LEAF_DATASIZE;
	leaf->cursor = 0;
	leaf->parent = node;
	leaf->data = (void*)TNF_malloc(SIZEOF_VOIDPTR_BYTE * BINTREE_LEAF_DATASIZE);
	fprintf(stderr, "data range: %p - %p\n", leaf->data, leaf->data + SIZEOF_VOIDPTR_BYTE * BINTREE_LEAF_DATASIZE);
	node->bucket[node->size] = id + BINTREE_LEAF_DATASIZE;
	node->size++;
	return leaf;
}

TNF_Node* Tree_add(TNF_Node* node, bucket_id id, void* data) {
	TNF_Node* ret;
	if (node->size > BINTREE_BUCKET_MAXSIZE) {
		if (node->bucket[node->size-1] == id) {
			goto filledBucket;
		}
	}
	int idx = getIndex(node, id);
	if (isNode(node)) {
		node = Tree_add(child(node, idx), id, data);
		ret = getRoot(node);
	}
	else if (isLeaf(node)) {
		TNF_LeafNode* leaf = getLeaf(node, id);
		if (leaf == NULL || leaf->cursor == BINTREE_LEAF_DATASIZE) {
			leaf = createLeaf(node, id);
		}
		todo("random bucket");
		void* ptr = leafdata(leaf, id - leaf->base);
		//fprintf(stderr, "leafdata: %p\n", ptr);
		memcpy(ptr, &data, SIZEOF_VOIDPTR);
		leaf->bucket[id - leaf->base] = id;
		leaf->cursor++;
		//fprintf(stderr, "leaf: %p\n", getLeaf(node, id));
		memcpy(getLeaf(node, id), leaf, sizeof(TNF_LeafNode));
		ret = node;
	}
	return ret;
	// bucket is filled!
filledBucket:;
	todo("expand bintree node");
	asm("int3");
	//bucket_id* buckets = node->bucket;
	//asm("int3");
	//idx = getIndex(node, id);
	//insertBucket_withIdx(node, idx, id);
	//bucket_id center_key = buckets[BINTREE_CENTERING_KEY];
	//TNF_Node* cur = splitNode(node, center_key);
	//cur->data[BINTREE_CENTERING_KEY] = record;
	//TNF_Node* root = getRoot(node);
	//return root;
	return node;
}

/* ----------------------------------------------------------------------------- */
void* Tree_get(TNF_Node* node, bucket_id id) {
	return 0;
}

/* ----------------------------------------------------------------------------- */


TNF_NodeResult* Tree_search(TNF_Node* node, void* threshold, int (*compare)(void* l, void* r)) {
	size_t i, j;
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
			TNF_LeafNode* leaf = leaf(node, i);
			if (compare((void*)(&leaf->base), threshold)) {
				todo("expand TNF_NodeResult");
				//if (ret->size == ret->maxsize) {
				//	expand(ret);
				//}
				for (j = 0; j < leaf->cursor; j++) {
					if (compare((void*)(&leaf->bucket[j]), threshold)) {
						memcpy(ret->data + (SIZEOF_VOIDPTR * ret->size), leafdata(leaf, j), SIZEOF_VOIDPTR);
						ret->size++;
					}
				}
			}
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

void Tree_print(TNF_Node* node, void (*printLeaf)(TNF_LeafNode*, int)) {
	static int indent = 0;
	if (node->child == NULL) return;
	if (isLeaf(node)) {
		printLeaf(leaf(node, 0), indent+1);
	}
	else if (isNode(node)) {
		indent++;
		Tree_print(child(node, 0), printLeaf);
		indent--;
	}
	size_t i, data_i = 1;
	for (i = 0; data_i < node->size; i++, data_i++) {
		INDENT_TO(indent);
		fprintf(stderr, "bucket: [%lu]\n", node->bucket[i]);
		if (child(node, data_i) == NULL) continue;
		if (isLeaf(node)) {
			printLeaf(leaf(node, data_i), indent+1);
		}
		else if (isNode(node)) {
			indent++;
			Tree_print(child(node, data_i), printLeaf);
			indent--;
		}
	}
}

/* ============================================================================= */

/*
int int_cmp(void* l, void* r) {
	int* base = (int*)l;
	int* id = (int*)r;
	//fprintf(stderr, "r(%d) > l(%d)\n", *base, *id);
	return (*base > *id) ? 1 : 0;
}

void printTest(TNF_LeafNode* leaf, int indent) {
	char** str;
	size_t i;
	for (i = 0; i < leaf->cursor; i++) {
		INDENT_TO(indent);
		str = (char**)leafdata(leaf, i);
		fprintf(stderr, "[%lu]: %s(%p)\n", leaf->bucket[i], *str, str);
	}
}

int main(int argc, char const* argv[])
{
	bucket_id id = 0;
	char* addr = (char*)malloc(sizeof(char) * (strlen("hello") + 1));
	strncpy(addr, "hello", strlen("hello"));
	TNF_Node* root = Tree_create();
	//Tree_start("dbname", "tblname"); // always working at another process
	size_t i;
	for (i = 0; i < 20; i++) {
		Tree_add(root, id++, (void*)addr);
	}
	Tree_print(root, printTest);
	//size_t addr = Tree_get(root, bucket_id);
	int th = 10;
	TNF_NodeResult* list = Tree_search(root, (void*)&th, int_cmp);
	for (i = 0; i < list->size; i++) {
		char** str = (char**)(list->data + (i * SIZEOF_VOIDPTR));
		fprintf(stderr, "ret: %s\n", *str);
	}
	//Tree_save(root);
	Tree_exit(root);
	return 0;
}
*/
