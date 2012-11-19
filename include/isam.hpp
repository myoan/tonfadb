#include <iostream>

using namespace std;

class Node {
public:
	Node(int idx);
	~Node();
	void insert(int idx);
	void remove(int idx);
	void show(int depth);
	int getIdx();
	Node* getLeft();
	Node* getRight();
	void deleteChild();
private:
	Node* left;
	Node* right;
	int idx;
	int value;
};
