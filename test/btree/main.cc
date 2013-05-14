#include <gtest/gtest.h>
#include "../../include/btree.h"

/* ----------------------------------------------------------------------------- */
/* [Node] */

TEST(NodeTest, InitializeRoot) {
	TNF_Node* node = createRoot();
	ASSERT_EQ(node->type, ROOT);
	ASSERT_EQ(node->lsize, 0);
	ASSERT_EQ(node->bsize, 0);
	//ASSERT_EQ(node->parent, NULL);
}

TEST(NodeTest, InitializeBranch) {
	TNF_Node* node = createBranch(createRoot());
	ASSERT_EQ(node->type, BRANCH);
	ASSERT_EQ(node->lsize, 0);
	ASSERT_EQ(node->bsize, 0);
	//ASSERT_EQ(node->parent, NULL);
}

TEST(NodeTest, InitializeLeaf) {
	TNF_LeafNode* leaf = createLeaf(createRoot(), 3);
	ASSERT_EQ(leaf->type, LEAF);
	//ASSERT_EQ(node->parent, NULL);
}

TEST(NodeTest, setBucketData) {
	TNF_Node* node = createBranch(createRoot());
	setB(node, 0, 1);
	setB(node, 1, 4);
	setB(node, 2, 19);
	setB(node, 3, 21);
	setB(node, 4, 3);

	//ASSERT_EQ(node->data[1], 1);
	ASSERT_EQ((size_t)node->data[1], 1);
	ASSERT_EQ((size_t)node->data[3], 4);
	ASSERT_EQ((size_t)node->data[5], 19);
	ASSERT_EQ((size_t)node->data[7], 21);
	ASSERT_EQ((size_t)node->data[9], 3);
}

TEST(NodeTest, ReadBucketData) {
	TNF_Node* node = createBranch(createRoot());
	setB(node, 0, 1);
	setB(node, 1, 4);
	setB(node, 2, 19);
	setB(node, 3, 21);
	setB(node, 4, 3);

	ASSERT_EQ((size_t)B(node, 0), 1);
	ASSERT_EQ((size_t)B(node, 1), 4);
	ASSERT_EQ((size_t)B(node, 2), 19);
	ASSERT_EQ((size_t)B(node, 3), 21);
	ASSERT_EQ((size_t)B(node, 4), 3);
	ASSERT_LE(Bsize(node), BUCKET_LENGTH);
	ASSERT_LE(Lsize(node), LEAF_LENGTH);
}

TEST(NodeTest, SlideNodeDataEven) {
	TNF_Node* node = createBranch(createRoot());
	/* [4, 1, 21, 3]*/
	setL(node, 0, 4);
	setB(node, 0, 1);
	setL(node, 1, 21);
	setB(node, 1, 3);

	ASSERT_EQ(Bsize(node), 2);
	ASSERT_EQ(Lsize(node), 2);
	ASSERT_EQ((size_t)L(node, 0), 4);
	ASSERT_EQ((size_t)B(node, 0), 1);
	ASSERT_EQ((size_t)L(node, 1), 21);
	ASSERT_EQ((size_t)B(node, 1), 3);

	node_datacpy(node, Bidx(1), node, Bidx(0), 3);

	ASSERT_EQ(Bsize(node), 2);
	ASSERT_EQ(Lsize(node), 2);

	/* [4, 1, 21, 1, 21, 3]*/
	ASSERT_EQ((size_t)L(node, 0), 4);
	ASSERT_EQ((size_t)B(node, 0), 1);
	ASSERT_EQ((size_t)L(node, 1), 21);

	ASSERT_EQ((size_t)B(node, 1), 1);
	ASSERT_EQ((size_t)L(node, 1), 21);
	ASSERT_EQ((size_t)B(node, 2), 3);
}

TEST(NodeTest, SlideNodeDataOdd) {
	TNF_Node* node = createBranch(createRoot());
	setL(node, 0, 4);
	setB(node, 0, 1);
	setL(node, 1, 21);

	ASSERT_EQ(Bsize(node), 1);
	ASSERT_EQ(Lsize(node), 2);

	node_datacpy(node, Bidx(0), node, Bidx(1), 2);

	ASSERT_EQ(Bsize(node), 1);
	ASSERT_EQ(Lsize(node), 2);
}

TEST(NodeTest, InsertBucket) {
	TNF_Node* node = createBranch(createRoot());
	setB(node, 0, 1);
	setB(node, 1, 10);
	setB(node, 2, 18);

	InsertBucket(node, 12, 2);

	ASSERT_EQ((size_t)B(node, 0), 1);
	ASSERT_EQ((size_t)B(node, 1), 10);
	ASSERT_EQ((size_t)B(node, 2), 12);
	ASSERT_EQ((size_t)B(node, 3), 18);
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
	ASSERT_EQ(node->lsize, 1);
	ASSERT_EQ(node->bsize, 1);
	//ASSERT_EQ(node->parent, NULL);
}

void printTest(TNF_LeafNode* leaf) {
	char* str;
	str = (char*)leaf->data;
	fprintf(stderr, "%s\n", str);
}

TEST(BTreeTest, AddNode_Nested) {
	TNF_Node* node = Tree_create();
	int i;
	for (i = 0; i < 6; i++) {
		node = Tree_add(node, i, mockData(i));
		//Tree_print(node, printTest);
	}
	ASSERT_EQ(node->type, ROOT);
	//ASSERT_EQ((size_t)B(node, 0), 1);
	//ASSERT_EQ(node->lsize, 1);
	//ASSERT_EQ(node->bsize, 1);
	//ASSERT_EQ(node->parent, NULL);
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
