#include <bptree.h>

void printNode(TNF_Node* node);
int insertBucket(TNF_Node* node, bucket_id id);

#define RECORD_MAX_SIZE 4

TNF_Node* Tree_createNode() {
	TNF_Node* node = (TNF_Node*)TNF_malloc(sizeof(TNF_Node));
	node->type = LEAF;
	node->size = 0;
	node->data = (void*)TNF_malloc(sizeof(void*) * (BIN_BUCKET_MAXSIZE + 1));
	memset(node->bucket, -1, sizeof(bucket_id) * BIN_BUCKET_OVERSIZE);
	return node;
}

TNF_Node* create_newRoot() { // depth is increment
	TNF_Node* node = (TNF_Node*)TNF_malloc(sizeof(TNF_Node));
	node->type = ROOT;
	node->size = 0;
	node->data = (void**)TNF_malloc(sizeof(void*) * (BIN_BUCKET_MAXSIZE + 1));
	memset(node->bucket, -1, sizeof(bucket_id) * BIN_BUCKET_OVERSIZE);
	return node;
}

#define INDENT_TO(__indent) {              \
	int __i;                               \
	for (__i = 0; __i < __indent; __i++) { \
		fprintf(stderr, "\t");             \
	}                                      \
}

#define isLeaf(node) (node->type & LEAF)
#define isNode(node)   (node->type & (BRANCH | ROOT))
#define _record(list, idx) ((TNF_Record*)(list + idx))
#define record(list, idx) (_record(list, idx + 1))
#define recordHeader(rcd) ((TNF_RecordHeader*)_record(rcd, 0))

void printRecord(TNF_Record* list, int indent) {
	TNF_RecordHeader* h = (TNF_RecordHeader*)list;
	size_t size = h->cursor, i;
	for (i = 0; i < size; i++) {
		TNF_Record* rcd = record(list, i);
		INDENT_TO(indent+1);
		fprintf(stderr, "(%lu, %lu)\n", rcd->id, rcd->data_idx);
	}
}

void printNode(TNF_Node* node) {
	static int indent = 0;
	if (node->data[0] == NULL) return;
	if (isLeaf(node)) {
		printRecord((TNF_Record*)node->data[0], indent+1);
	}
	else if (isNode(node)) {
		indent++;
		printNode(node->data[0]);
		indent--;
	}
	size_t i, data_i = 0;
	for (i = 0; i < node->size; i++) {
		data_i++;
		INDENT_TO(indent);
		fprintf(stderr, "bucket: [%lu]\n", node->bucket[i]);
		if (node->data[data_i] == NULL) continue;
		if (isLeaf(node)) {
			printRecord((TNF_Record*)node->data[data_i], indent+1);
		}
		else if (isNode(node)) {
			indent++;
			printNode(node->data[data_i]);
			indent--;
		}
	}
}

TNF_Node* create_child(TNF_Node* node, bucket_id id) {
	TNF_Node* parent = Tree_createNode();
	node->parent = parent;
	return node;
}

void updateRoot(TNF_Node* from, TNF_Node* to) {
	memcpy(to, from, sizeof(TNF_Node));
	to->data = from->data;
}

TNF_Node* splitNode(TNF_Node* node, bucket_id id) {
	TNF_Node* parent;
	if (node->parent == NULL) {
		parent = create_newRoot();
		node->parent = parent;
		node->type = LEAF;
	}
	else {
		parent = node->parent;
	}
	if (parent->size == BIN_BUCKET_MAXSIZE) {
		bucket_id center_key = parent->bucket[BIN_CENTERING_KEY];
		splitNode(parent, center_key);
	}
	int fw_idx = insertBucket(parent, id);
	TNF_Node* fw_node = Tree_createNode();
	TNF_Node* bw_node = Tree_createNode();
	fw_node->parent = parent;
	fw_node->size = BIN_CENTERING_KEY;
	bw_node->parent = parent;
	bw_node->size = BIN_CENTERING_KEY;
	parent->data[fw_idx] = (void*)fw_node;
	parent->data[fw_idx + 1] = (void*)bw_node;
	memcpy(fw_node->bucket, node->bucket, BUCKET_CENTERING_KEY_BYTE);
	memcpy(bw_node->bucket, node->bucket + BIN_CENTERING_KEY + 1, BUCKET_CENTERING_KEY_BYTE);
	memcpy(fw_node->data, node->data, DATA_CENTERING_KEY_BYTE);
	memcpy(bw_node->data, node->data + BIN_CENTERING_KEY, DATA_CENTERING_KEY_BYTE);
	updateRoot(parent, node);
	return bw_node;
}

int insertBucket(TNF_Node* node, bucket_id id) {
	bucket_id* buckets = node->bucket;
	int i;
	for (i = 0; i < BIN_BUCKET_MAXSIZE; i++) {
		if (buckets[i] > id) {
			memcpy(buckets + i+1, buckets + i, sizeof(bucket_id) * (node->size - i));
			buckets[i] = id;
			node->size++;
			if (node->size > BIN_BUCKET_MAXSIZE) {
				return -1;
			}
			return i;
		}
	}
	return -1;
}

TNF_Record* create_record(size_t id, size_t addr) {
	TNF_Record* ret = (TNF_Record*)malloc(sizeof(TNF_Record));
	ret->id = id;
	ret->data_idx = addr;
	return ret;
}

int getIndex(TNF_Node* node, bucket_id id) {
	bucket_id* buckets = node->bucket;
	int i;
	for (i = 0; i < BIN_BUCKET_MAXSIZE; i++) {
		if (buckets[i] > id) {
			return i;
		}
	}
	return i;
}

int insertBucket_withIdx(TNF_Node* node, int idx, bucket_id id) {
	memcpy(node->bucket + idx+1, node->bucket + idx, sizeof(bucket_id) * (node->size - idx));
	node->bucket[idx] = id;
	node->size++;
	if (node->size > BIN_BUCKET_MAXSIZE) {
		return -1;
	}
	return idx;
}

TNF_Node* getRoot(TNF_Node* node) {
	if (node->parent != NULL) {
		return getRoot(node->parent);
	}
	return node;
}

TNF_Record* create_recordList() {
	TNF_Record* list = (TNF_Record*)TNF_malloc(sizeof(TNF_Record) * (RECORD_MAX_SIZE + 1));
	TNF_RecordHeader* h = (TNF_RecordHeader*)list;
	h->size = RECORD_MAX_SIZE;
	h->cursor = 0;
	return list;
}

void write_record(TNF_Record* list, size_t idx, size_t data_addr) {
	size_t base = recordHeader(list)->base;
	TNF_Record* rcd = record(list, idx - base);
	rcd->id = idx;
	rcd->data_idx = data_addr;
	TNF_RecordHeader* h = recordHeader(list);
	h->cursor++;
}

TNF_Record* getRecord(TNF_Node* node, bucket_id id) {
	size_t i;
	for (i = 0; i < node->size; i++) {
		//fprintf(stderr, "base(%lu): %lu, idx: %lu\n", i+1, node->bucket[i], id);
		if (node->bucket[i] > id) {
			return (TNF_Record*)node->data[i];
		}
	}
	return NULL;
}

TNF_Record* addRecordList(TNF_Node* node, bucket_id id) {
	TNF_Record* list = create_recordList();
	node->data[node->size] = (void*)list;
	node->bucket[node->size] = id + RECORD_MAX_SIZE;
	node->size++;
	recordHeader(list)->base = id;
	return list;
}

TNF_Node* Tree_addNode(TNF_Node* node, bucket_id id, TNF_Record* record) {
	bucket_id* buckets = node->bucket;
	TNF_Node* ret;
	if (node->size > BIN_BUCKET_MAXSIZE) {
		if (node->bucket[node->size-1] == id) {
			goto filledBucket;
		}
	}
	int idx = getIndex(node, id);
	if (isNode(node)) {
		node = Tree_addNode((TNF_Node*)node->data[idx], id, record);
		ret = getRoot(node);
	}
	else if (isLeaf(node)) {
		TNF_Record* rcd = getRecord(node, id);
		if (rcd == NULL || recordHeader(rcd)->cursor == RECORD_MAX_SIZE) {
			rcd = addRecordList(node, id);
		}
		write_record(rcd, record->id, record->data_idx);
		ret = node;
	}
	return ret;
	// bucket is filled!
filledBucket:;
	asm("int3");
	idx = getIndex(node, id);
	insertBucket_withIdx(node, idx, id);
	bucket_id center_key = buckets[BIN_CENTERING_KEY];
	TNF_Node* cur = splitNode(node, center_key);
	cur->data[BIN_CENTERING_KEY] = record;
	TNF_Node* root = getRoot(node);
	return root;
}

int main(int argc, char const* argv[])
{
	TNF_Node* root = Tree_createNode();
	TNF_Record* rcd;
	size_t i;
	for (i = 0; i < 20; i++) {
		rcd = create_record(i, i);
		root = Tree_addNode(root, i, rcd);
	}
	printNode(root);
	//Tree_searchNode(10);
	return 0;
}

