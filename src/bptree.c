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

void printNode(TNF_BinNode* node) {
	size_t i;
	fprintf(stderr, "bucket: [");
	for (i = 0; i < node->size; i++) {
		fprintf(stderr, " %lu,", node->bucket[i]);
	}
	fprintf(stderr, "]\n");

	if (node->type & LEAF || node->type & BRANCH_LEAF) {
		fprintf(stderr, "record: [");
		for (i = 0; i < node->size + 1; i++) {
			if (node->data[i] != NULL) {
				fprintf(stderr, " %lu,", ((TNF_Record*)node->data[i])->dummy);
			}
		}
		fprintf(stderr, "]\n");
	}
}

TNF_BinNode* create_child(TNF_BinNode* node, bucket_id id) {
	TNF_BinNode* parent = create_node();
	node->parent = parent;
	return node;
}

void updateRoot(TNF_BinNode* from, TNF_BinNode* to) {
	TNF_free(from->data);
	memcpy(from, to, sizeof(TNF_BinNode));
	TNF_free(to);
}

TNF_BinNode* splitNode(TNF_BinNode* node, bucket_id id) {
	TNF_BinNode* parent;
	if (node->parent == NULL) {
		parent = create_newRoot();
		node->parent = parent;
		node->type = LEAF;
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
	memcpy(fw_node->bucket, node->bucket, sizeof(bucket_id) * BIN_CENTERING_KEY);
	memcpy(bw_node->bucket, node->bucket + BIN_CENTERING_KEY + 1, sizeof(bucket_id) * BIN_CENTERING_KEY);
	memcpy(fw_node->data, node->data, sizeof(void*) * BIN_CENTERING_KEY);
	memcpy(bw_node->data, node->data + BIN_CENTERING_KEY, sizeof(void*) * BIN_CENTERING_KEY);
	updateRoot(node, parent);
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

TNF_BinNode* addData(TNF_BinNode* node, bucket_id id, TNF_Record* record) {
	bucket_id* buckets = node->bucket;
	int idx = insertBucket(node, id);
	if (idx >= 0) {
		memcpy(node->data + idx+1, node->data + idx, sizeof(void*) * (node->size - idx));
		node->data[idx] = record;
		return node;
	}
	// bucket is filled!
	bucket_id center_key = buckets[BIN_CENTERING_KEY];
	//fprintf(stderr, "center key: %lu\n", center_key);
	TNF_BinNode* cur = splitNode(node, center_key);
	cur->data[BIN_CENTERING_KEY] = record;
	return node;
}

int main(int argc, char const* argv[])
{
	TNF_BinNode* root = create_newRoot();
	TNF_Record* rcd = create_record(0);
	root = addData(root, 10, rcd);
	rcd = create_record(1);
	root = addData(root, 12, rcd);
	rcd = create_record(2);
	root = addData(root, 8, rcd);
	rcd = create_record(3);
	root = addData(root, 90, rcd);
	rcd = create_record(4);
	root = addData(root, 20, rcd);
	printNode((TNF_BinNode*)root->data[1]);
	return 0;
}
