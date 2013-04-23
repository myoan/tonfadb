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

int all_true(void* l, void* r) {
	return 1;
}

//int int_cmp(void* l, void* r) {
//	int* base = (int*)l;
//	int* id = (int*)r;
//	fprintf(stderr, "r(%d) == l(%d)\n", *base, *id);
//	fprintf(stderr, "result: %s\n", (*base < *id) ? "true" : "false");
//	return (*base <= *id) ? 1 : 0;
//}

bucket_id TNF_insert(TNF_Index* index, bucket_id id, char* data) {
	// 1. write Record, get Record addr
	size_t addr = Record_write(index->record, data, id);
	// 2. create RecordMap
	TNF_RecordAddrMap* map = createRecord(id, addr);
	// 3. insert BinTree
	Tree_add(index->tree, id, (void*)map);
	return id + 1;
}

void TNF_CmdUpdate(TNF_Index* index, char** line) {
}

void TNF_CmdDelete(TNF_Index* index, char** line) {
}

void TNF_CmdSelect(TNF_Index* index, char** line) {
	char* token, *svalue, *table_name;
	//int value;
	while ((token = getToken(line)) != '\0') {
	//	//fprintf(stderr, "insert: token: %s\n", token);
	//	if (!strncmp(token, "INTO", 1)) {
	//		table_name = getToken(line);
	//		assert(token != '\0');
	//		fprintf(stderr, "table: %s\n", table_name);
	//	}
	//	if (!strncmp(token, "VALUE", 1)) {
	//		svalue = getToken(line); // '('
	//		svalue = getToken(line);
	//		value = atoi(svalue);
	//		fprintf(stderr, "value: %d\n", value);
	//	}
	}
	int th = 2;
	TNF_NodeResult* list = Tree_search(index->tree, (void*)&th, all_true);
	int i;
	for (i = 0; i < list->size; i++) {
		TNF_RecordAddrMap* map = *(TNF_RecordAddrMap**)(list->data + (i * SIZEOF_VOIDPTR));
		fprintf(stderr, "id: %s\n", Record_read(index->record, map->addr));
	}
}

void TNF_CmdInsert(TNF_Index* index, char** line) {
	//fprintf(stderr, "cmd insert\n");
	char* token, *svalue, *table_name;
	int value;
	while ((token = getToken(line)) != '\0') {
		//fprintf(stderr, "insert: token: %s\n", token);
		if (!strncmp(token, "INTO", 1)) {
			table_name = getToken(line);
			assert(token != '\0');
			fprintf(stderr, "table: %s\n", table_name);
		}
		if (!strncmp(token, "VALUE", 1)) {
			svalue = getToken(line); // '('
			svalue = getToken(line);
			value = atoi(svalue);
			fprintf(stderr, "value: %d\n", value);
		}
	}
	index->cursor = TNF_insert(index, index->cursor, svalue);
}

void (*TNF_CmdLibs[])(TNF_Index*, char**) = {
	NULL, // INST_NULL
	TNF_CmdUpdate, // INST_UPDATE
	TNF_CmdDelete, // INST_DELETE
	TNF_CmdSelect, // INST_SELECT
	TNF_CmdInsert  // INST_INSERT
};

/*
int main(int argc, char** argv)
{
	TNF_Index* index = (TNF_Index*)TNF_malloc(sizeof(TNF_Index));
	TNF_Node* root = Tree_create();
	TNF_Record* record = TNF_CreateFile();
	index->tree = root;
	index->record = record;

	if (argc == 1){
		// interactive mode
	}
	else if (argc == 2){
		// file mode
		readFile(index, argv[1]);
	}
	else {
		fprintf(stderr, "ERROR: lispy need more arguments\n");
		exit(0);
	}
	return 0;
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
	}
	Tree_exit(root);
	return 0;
}
*/
