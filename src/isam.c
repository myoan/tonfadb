#include <tonfa.h>
#include <isam.h>

TNF_Root* Create_Root() {
	TNF_Root* root = (TNF_Root*)TNF_Malloc(sizeof(TNF_Root));
	return root;
}

TNF_Branch* Create_Branch() {
	TNF_Branch* br = (TNF_Branch*)TNF_Malloc(sizeof(TNF_Branch));
	//br->idx = idx;
	return br;
}

TNF_Leaf* Create_Leaf() {
	TNF_Leaf* leaf = (TNF_Leaf*)TNF_Malloc(sizeof(TNF_Leaf));
	//leaf->idx = idx;
	return leaf;
}

TNF_Leaf* SearchLeafFromIdx(TNF_Root* root) {
	return leaf;
}

TNF_Leaf* AddBranch(TNF_Root* root) {
	return leaf;
}

TNF_Leaf* AddLeaf(TNF_Root* root) {
	return leaf;
}

TNF_Leaf* DeleteBranch(TNF_Root* root, int idx) {
	return leaf;
}

TNF_Leaf* DeleteLeaf(TNF_Root* root, int idx) {
	return leaf;
}

TNF_Leaf* UpdateBranch(TNF_Root* root, int idx) {
	return leaf;
}

TNF_Leaf* UpdateLeaf(TNF_Root* root, int idx) {
	return leaf;
}


int main(int argc, char const* argv[])
{
	fprintf(stderr, "hello, tonfa\n");
	return 0;
}
