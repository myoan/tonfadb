#include <tonfa.h>

#ifdef __cplusplus
extern "C" {
#endif

TNF_libs filenode_libs = {filenode_init, filenode_write, filenode_read, filenode_close};

int global_idx = 0;

TNF_filenode* filenode_init(TNF_filenode* node, char* name, int fd, size_t table_size) {
	node->fd = fd;
	node->name = name;
	node->column_size = table_size;
	node->size = FILE_SIZE;
	node->head_idx = 0;
	node->tail_idx = node->head_idx + node->size / node->column_size;
	node->cur_idx = node->head_idx;
	size_t data_idx = node->head_idx;
	char* blank = (char*)TNF_Malloc(node->column_size);
	while (data_idx < node->tail_idx) {
		write(fd, blank, node->column_size);
		data_idx++;
	}
	lseek(fd, -1 * node->size, SEEK_CUR);
	return node;
}

#define FILE_IDX(o, idx) (o->column_size * idx)

void filenode_write(TNF_filenode* node, char* data, size_t table_idx) {
	lseek(node->fd, FILE_IDX(node, table_idx), SEEK_CUR);
	write(node->fd, data, node->column_size);
}

char* filenode_read(TNF_filenode* node, size_t size) {
	return NULL;
}

void filenode_close(TNF_filenode* node) {
	close(node->fd);
	TNF_Free(node->name);
	TNF_Free(node);
}

TNF_filenode* TNF_CreateFile() {
	size_t size = digit(global_idx) + FILE_SUFFIX_LEN + 1;
	char* filename = (char*)TNF_Malloc(size);
	snprintf(filename, size, "%d%s", global_idx, FILE_SUFFIX);
	fprintf(stderr, "create file: %s\n", filename);
	int fd = open(filename, O_RDWR|O_CREAT, S_IREAD|S_IWRITE);
	if (fd == -1) {
		printf("file open error\n");
		exit(EXIT_FAILURE);
	}
	TNF_filenode* node = (TNF_filenode*)TNF_Malloc(sizeof(TNF_filenode));
	filenode_init(node, filename, fd, COLUMN_SIZE);
	return node;
}
/*
int main(int argc, char const* argv[])
{
	TNF_filenode* node = TNF_CreateFile();
	filenode_write(node, "You say good bye. I say hello.", 3);
	filenode_close(node);
	return 0;
}
*/

#ifdef __cplusplus
}
#endif

