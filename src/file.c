#include <tonfa.h>

#ifdef __cplusplus
extern "C" {
#endif

TNF_libs Record_libs = {Record_init, Record_write, Record_read, Record_close};

int global_idx = 0;

TNF_Record* Record_init(TNF_Record* node, char* name, int fd, size_t table_size) {
	node->fd = fd;
	node->name = name;
	node->column_size = table_size;
	node->size = FILE_SIZE;
	node->head_idx = 0;
	node->tail_idx = node->head_idx + node->size / node->column_size;
	node->cur_idx = node->head_idx;
	//size_t data_idx = node->head_idx;
	//char* blank = (char*)TNF_Malloc(node->column_size);
	//while (data_idx < node->tail_idx) {
	//	write(fd, blank, node->column_size);
	//	data_idx++;
	//}
	//lseek(fd, -1 * node->size, SEEK_CUR);
	return node;
}

TNF_Record* Record_open(TNF_Record* node) {
	size_t data_idx = node->head_idx;
	char* blank = (char*)TNF_Malloc(node->column_size);
	while (data_idx < node->tail_idx) {
		write(node->fd, blank, node->column_size);
		data_idx++;
	}
	lseek(node->fd, -1 * node->size, SEEK_CUR);
	lseek(node->fd, -1 * node->size, SEEK_CUR);
	return node;
}

#define FILE_IDX(o, idx) (o->column_size * idx)

void Record_write(TNF_Record* node, char* data, size_t table_idx) {
	lseek(node->fd, FILE_IDX(node, table_idx), SEEK_CUR);
	write(node->fd, data, node->column_size);
}

char* Record_read(TNF_Record* node, size_t size) {
	return NULL;
}

void Record_close(TNF_Record* node) {
	close(node->fd);
	TNF_Free(node->name);
	TNF_Free(node);
}

TNF_Record* TNF_CreateFile() {
	size_t size = digit(global_idx) + FILE_SUFFIX_LEN + 1;
	char* filename = (char*)TNF_Malloc(size);
	snprintf(filename, size, "%d%s", global_idx, FILE_SUFFIX);
	fprintf(stderr, "create file: %s\n", filename);
	int fd = open(filename, O_RDWR|O_CREAT, S_IREAD|S_IWRITE);
	if (fd == -1) {
		printf("file open error\n");
		exit(EXIT_FAILURE);
	}
	TNF_Record* node = (TNF_Record*)TNF_Malloc(sizeof(TNF_Record));
	Record_init(node, filename, fd, COLUMN_SIZE);
	return node;
}

/*
int main(int argc, char const* argv[])
{
	TNF_Record* node = TNF_CreateFile();
	Record_open(node);
	Record_write(node, "You say good bye. I say hello.", 3);
	Record_close(node);
	node = TNF_CreateFile();
	Record_write(node, "hello, world.", 4);
	Record_close(node);
	node = TNF_CreateFile();
	Record_write(node, "i am the egg man.", 2);
	Record_close(node);
	node = TNF_CreateFile();
	Record_write(node, "love me do.", 6);
	Record_close(node);
	return 0;
}
*/

#ifdef __cplusplus
}
#endif

