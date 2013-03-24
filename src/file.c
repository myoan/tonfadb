#include <tonfa.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

//TNF_libs Record_libs = {Record_init, Record_write, Record_read, Record_close};

int global_idx = 0;

TNF_Record* Record_init(TNF_Record* node, char* name, FILE* fp, size_t table_size) {
	node->fp = fp;
	node->name = name;
	node->column_size = table_size;
	node->size = FILE_SIZE;
	node->head_idx = 0;
	node->tail_idx = node->head_idx + node->size / node->column_size;
	node->cur_idx = node->head_idx;
	//size_t data_idx = node->head_idx;
	//char* blank = (char*)TNF_malloc(node->column_size);
	//while (data_idx < node->tail_idx) {
	//	write(fd, blank, node->column_size);
	//	data_idx++;
	//}
	//lseek(fd, -1 * node->size, SEEK_CUR);
	return node;
}

TNF_Record* Record_open(TNF_Record* node) {
	assert(node != NULL);
	fseek(node->fp, 0L, SEEK_SET);
	return node;
}

#define FILE_IDX(o, idx) (o->column_size * (idx))

size_t Record_write(TNF_Record* node, char* data, size_t table_idx) {
	fseek(node->fp, FILE_IDX(node, table_idx), SEEK_SET);
	fwrite(data, sizeof(char), node->column_size, node->fp);
	return table_idx;
}

char* Record_read(TNF_Record* node, size_t table_idx) {
	char* ret = (char*)TNF_malloc(sizeof(char) * node->column_size);
	fseek(node->fp, FILE_IDX(node, table_idx), SEEK_SET);
	fread(ret, sizeof(char), node->column_size, node->fp);
	return ret;
}

void Record_close(TNF_Record* node) {
	fclose(node->fp);
	TNF_free(node->name);
	TNF_free(node);
}

TNF_Record* TNF_CreateFile() {
	size_t size = digit(global_idx) + FILE_SUFFIX_LEN + 1;
	char* filename = (char*)TNF_malloc(size);
	snprintf(filename, size, "%d%s", global_idx, FILE_SUFFIX);
	fprintf(stderr, "create file: %s\n", filename);
	FILE* fp = fopen(filename, "wb+");
	if (fp == NULL) {
		printf("file open error\n");
		exit(EXIT_FAILURE);
	}
	TNF_Record* node = (TNF_Record*)TNF_malloc(sizeof(TNF_Record));
	Record_init(node, filename, fp, COLUMN_SIZE);
	return node;
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

#ifdef __cplusplus
}
#endif

