#include <bptree.h>

void printNode(TNF_BinNode* node);
int insertBucket(TNF_BinNode* node, bucket_id id);

TNF_BinNode* create_node() {
	TNF_BinNode* node = (TNF_BinNode*)TNF_malloc(sizeof(TNF_BinNode));
	node->type = LEAF;
	node->size = 0;
	node->data = (void*)TNF_malloc(sizeof(void*) * (BIN_BUCKET_MAXSIZE + 1));
	memset(node->bucket, -1, sizeof(bucket_id) * BIN_BUCKET_OVERSIZE);
	return node;
}

TNF_BinNode* create_newRoot() { // depth is increment
	TNF_BinNode* node = (TNF_BinNode*)TNF_malloc(sizeof(TNF_BinNode));
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

#define isRecord(node) (node->type & LEAF)
#define isNode(node)   (node->type & (BRANCH | ROOT))

void printNode(TNF_BinNode* node) {
	static int indent = 0;
	if (node->data[0] == NULL) return;
	if (isRecord(node)) {
		INDENT_TO(indent);
		fprintf(stderr, "r(%d): [%lu]\n", 0, ((TNF_Record*)node->data[0])->dummy);
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
		fprintf(stderr, "b(%lu): [%lu]\n", i, node->bucket[i]);
		if (node->data[data_i] == NULL) continue;
		if (isRecord(node)) {
			INDENT_TO(indent);
			fprintf(stderr, "r(%lu): [%lu]\n", data_i, ((TNF_Record*)node->data[data_i])->dummy);
		}
		else if (isNode(node)) {
			indent++;
			printNode(node->data[data_i]);
			indent--;
		}
	}
}

TNF_BinNode* create_child(TNF_BinNode* node, bucket_id id) {
	TNF_BinNode* parent = create_node();
	node->parent = parent;
	return node;
}

void updateRoot(TNF_BinNode* from, TNF_BinNode* to) {
	//TNF_free(from->data);
	//memcpy(to->data, from->data, sizeof(void*) * from->size);
	memcpy(to, from, sizeof(TNF_BinNode));
	to->data = from->data;
	//from = to;
	//TNF_free(from);
}

TNF_BinNode* splitNode(TNF_BinNode* node, bucket_id id) {
	TNF_BinNode* parent;
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
	TNF_BinNode* fw_node = create_node();
	TNF_BinNode* bw_node = create_node();
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

int insertBucket(TNF_BinNode* node, bucket_id id) {
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

TNF_Record* create_record(size_t data) {
	TNF_Record* ret = (TNF_Record*)malloc(sizeof(TNF_Record));
	ret->dummy = data;
	return ret;
}

int getIndex(TNF_BinNode* node, bucket_id id) {
	bucket_id* buckets = node->bucket;
	int i;
	for (i = 0; i < BIN_BUCKET_MAXSIZE; i++) {
		if (buckets[i] > id) {
			return i;
		}
	}
	return i;
}

int insertBucket_withIdx(TNF_BinNode* node, int idx, bucket_id id) {
	memcpy(node->bucket + idx+1, node->bucket + idx, sizeof(bucket_id) * (node->size - idx));
	node->bucket[idx] = id;
	node->size++;
	if (node->size > BIN_BUCKET_MAXSIZE) {
		return -1;
	}
	return idx;
}

TNF_BinNode* getRoot(TNF_BinNode* node) {
	if (node->parent != NULL) {
		return getRoot(node->parent);
	}
	return node;
}

TNF_BinNode* addData(TNF_BinNode* node, bucket_id id, TNF_Record* record) {
	fprintf(stderr, "add (%lu, %lu)\n", id, record->dummy);
	bucket_id* buckets = node->bucket;
	TNF_BinNode* ret;
	if (node->size == BIN_BUCKET_MAXSIZE) {
		goto filledBucket;
	}
	int idx = getIndex(node, id);
	if (isNode(node)) {
		node = addData((TNF_BinNode*)node->data[idx], id, record);
		ret = getRoot(node);
	}
	else if (isRecord(node)) {
		insertBucket_withIdx(node, idx, id);
		memcpy(node->data + idx+1, node->data + idx, sizeof(void*) * (node->size - idx));
		node->data[idx] = record;
		//TNF_Record* rcd = node->data[idx];
		//Record_open(rcd, rcd->filename, rcd->fd, COLUMN_SIZE);
		//Record_write(rcd, "You say good bye. I say hello.");
		//Record_close(rcd);
		ret = node;
	}
	return ret;
	// bucket is filled!
filledBucket:;
	idx = getIndex(node, id);
	insertBucket_withIdx(node, idx, id);
	bucket_id center_key = buckets[BIN_CENTERING_KEY];
	TNF_BinNode* cur = splitNode(node, center_key);
	cur->data[BIN_CENTERING_KEY] = record;
	TNF_BinNode* root = getRoot(node);
	return root;
	//return node;
}

int main(int argc, char const* argv[])
{
	TNF_BinNode* root = create_node();
	TNF_Record* rcd = create_record(0);
	//TNF_Record* rcd = TNF_CreateFile();
	root = addData(root, 10, rcd);
	root = addData(root, 12, rcd);
	rcd = create_record(2);
	root = addData(root, 8, rcd);
	rcd = create_record(3);
	root = addData(root, 90, rcd);
	rcd = create_record(4);
	root = addData(root, 20, rcd);
	rcd = create_record(5);
	root = addData(root, 100, rcd);
	rcd = create_record(6);
	root = addData(root, 101, rcd);
	rcd = create_record(7);
	root = addData(root, 102, rcd);
	rcd = create_record(8);
	root = addData(root, 103, rcd);
	fprintf(stderr, "root(%p)\n", root);
	printNode(root);
	return 0;
}

