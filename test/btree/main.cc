#include <gtest/gtest.h>
#include "../../include/btree.h"

TNF_Node* createRoot();

TEST(NodeTest, InitializeRoot) {
	TNF_Node* node = createRoot();
	ASSERT_EQ(node->type, ROOT);
	ASSERT_EQ(node->lsize, 0);
	ASSERT_EQ(node->bsize, 0);
	//ASSERT_EQ(node->parent, NULL);
}

void* mockData(size_t data) {
	char* addr = (char*)malloc(sizeof(char) * 16);
	sprintf(addr, "hello%lu", data);
	return (void*)addr;
}

TEST(BTreeTest, AddNode_Simple) {
	TNF_Node* node = Tree_create();
	node = Tree_add(node, 1, mockData(1));
	ASSERT_EQ(node->type, ROOT);
	ASSERT_EQ((size_t)B(node, 0), 1);
	ASSERT_EQ(node->lsize, 0);
	ASSERT_EQ(node->bsize, 0);
	//ASSERT_EQ(node->parent, NULL);
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
