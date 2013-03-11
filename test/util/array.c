#include <util.h>

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
