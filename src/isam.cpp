#include <isam.hpp>

Node::Node(int idx) {
	cout << "create node " << idx << endl;
	memset(this, '\0', sizeof(Node));
	this->idx = idx;
};

Node::~Node() {
	delete this;
};

void Node::insert(int idx) {
	if (idx < this->idx) {
		cout << "parent idx: " << this->idx << " insert left" << endl;
		this->left = new Node(idx);
	}
	else if (idx > this->idx) {
		cout << "parent idx: " << this->idx << " insert right" << endl;
		this->right = new Node(idx);
	}
	else {
		// TODO
	}
};

int Node::getIdx() {
	return this->idx;
};

Node* Node::getRight() {
	return this->left;
};

Node* Node::getLeft() {
	return this->right;
};

void Node::deleteChild() {
	if (this->left != NULL) {
		this->left->deleteChild();
	}
	else if (this->right != NULL) {
		this->right->deleteChild();
	}
};

int main(int argc, char const* argv[])
{
	Node* node = new Node(1);
	node->insert(2);
	node->insert(0);
	node->deleteChild();
	return 0;
}
