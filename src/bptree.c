#include <bptree.h>

//void node_insert(TNF_BinRoot* root, size_t id, void* node) {
//	if (root->isFilled()) {
//	}
//}
//
//bool sortInsert(size_t[] array, size_t id) {
//	return false;
//}
//
//void setBucket(TNF_BinNode* node, size_t bucket) {
//	int ret = sortInsert(node->bucket, bucket);
//}
//
int main(int argc, char const* argv[])
{
	TNF_size_tArray* a = create_size_tArray();
	push_size_tArray(a, 1);
	push_size_tArray(a, 2);
	push_size_tArray(a, 3);
	push_size_tArray(a, 4);
	push_size_tArray(a, 5);
	fprintf(stderr, "pop: %lu\n", pop_size_tArray(a));
	fprintf(stderr, "pop: %lu\n", pop_size_tArray(a));
	fprintf(stderr, "pop: %lu\n", pop_size_tArray(a));
	fprintf(stderr, "pop: %lu\n", pop_size_tArray(a));
	fprintf(stderr, "pop: %lu\n", pop_size_tArray(a));
	return 0;
}
