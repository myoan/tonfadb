#include <tonfa.h>

void printRecord(TNF_LeafNode* leaf, int indent) {
	TNF_RecordAddrMap* map;
	size_t i;
	for (i = 0; i < leaf->cursor; i++) {
		INDENT_TO(indent);
		map = *(TNF_RecordAddrMap**)leafdata(leaf, i);
		fprintf(stderr, "[%lu]: %lu\n", map->id, map->addr);
	}
}

TNF_RecordAddrMap* createRecord(bucket_id id, size_t addr) {
	TNF_RecordAddrMap* map = (TNF_RecordAddrMap*)TNF_malloc(sizeof(TNF_RecordAddrMap));
	map->id = id;
	map->addr = addr;
	return map;
}

int int_cmp(void* l, void* r) {
	int* base = (int*)l;
	int* id = (int*)r;
	fprintf(stderr, "r(%d) == l(%d)\n", *base, *id);
	fprintf(stderr, "result: %s\n", (*base < *id) ? "true" : "false");
	return (*base <= *id) ? 1 : 0;
}

bucket_id TNF_insert(TNF_Index* index, bucket_id id, char* data) {
	// 1. write Record, get Record addr
	size_t addr = Record_write(index->record, data, id);
	// 2. create RecordMap
	TNF_RecordAddrMap* map = createRecord(id, addr);
	// 3. insert BinTree
	Tree_add(index->tree, id, (void*)map);
	return id + 1;
}

int main(int argc, char const* argv[])
{
	TNF_Index* index = (TNF_Index*)TNF_malloc(sizeof(TNF_Index));
	TNF_Node* root = Tree_create();
	TNF_Record* record = TNF_CreateFile();
	index->tree = root;
	index->record = record;
	bucket_id id = 0;

	id = TNF_insert(index, id, "hello, world1");
	id = TNF_insert(index, id, "hello, world2");
	id = TNF_insert(index, id, "hello, world3");
	id = TNF_insert(index, id, "hello, world4");
	id = TNF_insert(index, id, "hello, world5");

	Tree_print(index->tree, printRecord);
	int th = 2;
	size_t i;
	TNF_NodeResult* list = Tree_search(root, (void*)&th, int_cmp);
	for (i = 0; i < list->size; i++) {
		TNF_RecordAddrMap* map = *(TNF_RecordAddrMap**)(list->data + (i * SIZEOF_VOIDPTR));
		fprintf(stderr, "ret: %s\n", Record_read(index->record, map->addr));
		//fprintf(stderr, "ret: %s\n", *map);
	}
	//Tree_save(root);
	Tree_exit(root);
	return 0;
}

/*
int main(int argc, char const* argv[])
{
	TNF_Record* node = TNF_CreateFile();
	Record_write(node, "hello, world1.", 1);
	Record_write(node, "hello, world2.", 5);
	Record_write(node, "hello, world4.", 4);

	fprintf(stderr, "ret: %s\n", Record_read(node, 5));
	fprintf(stderr, "ret: %s\n", Record_read(node, 4));
	Record_close(node);
	return 0;
}
*/
