#include <isam.hpp>

Node::Node(int idx) {
	memset(this, '\0', sizeof(Node));
	this->idx = idx;
};

Node::~Node() {
	delete this;
};

void Node::insert(int idx) {
	if (idx < this->idx) {
		if (this->left == NULL) {
			this->left = new Node(idx);
		}
		else {
			this->left->insert(idx);
		}
	}
	else if (idx > this->idx) {
		if (this->right == NULL) {
			this->right = new Node(idx);
		}
		else {
			this->right->insert(idx);
		}
	}
	else {
		// TODO
	}
};

void Node::remove(int idx) {
	if (idx < this->idx) {
		cout << "parent idx: " << this->idx << " remove left" << endl;
		this->left->remove(idx);
	}
	else if (idx > this->idx) {
		cout << "parent idx: " << this->idx << " remove right" << endl;
		this->right->remove(idx);
	}
	else {
		delete this;
	}
};

void Node::show(int depth) {
	if (this->left != NULL) {
		this->left->show(depth + 1);
	}
	for (int i = 0; i < depth; i++) {
		cout << "  ";
	}
	cout << this->idx << endl;
	if (this->right != NULL) {
		this->right->show(depth + 1);
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
	node->insert(5);
	node->insert(2);
	node->insert(4);
	node->insert(0);
	node->deleteChild();
	node->show(0);
	node->remove(4);
	return 0;
}
