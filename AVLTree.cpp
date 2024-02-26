// avl tree (hands on)

#include <iostream>
using namespace std;

class Node {
public:
	int key;
	Node* left;
	Node* right;
	int height;

};

int max(int a, int b) {
	return (a > b) ? a : b;
}

int height(Node* N) {
	if (N == NULL)
		return 0;
	return N->height;
}

// node creation
Node* newNode(int key) {
	Node* node = new Node();
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return node;
}

// right rotation

Node* rightRotate(Node* y) {
	Node* x = y->left;
	Node* T2 = x->right;

	// perform rotation
	x->right = y;
	y->left = T2;

	// update heights
	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

	// return new root
	return x;
}

// left rotation

Node* leftRotate(Node* x) {
	Node* y = x->right;
	Node* T2 = y->left;

	// perform rotation
	y->left = x;
	x->right = T2;

	// update heights
	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;

	// return new root
	return y;
}

// get balance factor of node N
int getBalance(Node* N) {
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

// insert node

Node* insert(Node* node, int key) {
	// perform normal BST insertion
	if (node == NULL)
		return (newNode(key));

	if (key < node->key)
		node->left = insert(node->left, key);
	else if (key > node->key)
		node->right = insert(node->right, key);
	else // equal keys are not allowed in BST
		return node;

	// update height of this ancestor node
	node->height = 1 + max(height(node->left), height(node->right));

	// get the balance factor of this ancestor node to check whether this node became unbalanced
	int balance = getBalance(node);


	if (balance > 1) { // left left case
		if (key < node->left->key) { // left left case
			return rightRotate(node);
		}
		else if (key > node->left->key) { // left right case
			node->left = leftRotate(node->left);
			return rightRotate(node);
		}
	}
	return node;
}

// node with minimum value

Node* minValueNode(Node* node) {
	Node* current = node;

	// loop down to find the leftmost leaf
	while (current->left != NULL)
		current = current->left;

	return current;
}

// delete node

Node* deleteNode(Node* root, int key) {

	// perform standard BST delete
	if (root == NULL)
		return root;

	// if the key to be deleted is smaller than the root's key, then it lies in left subtree
	if (key < root->key)
		root->left = deleteNode(root->left, key);

	// if the key to be deleted is greater than the root's key, then it lies in right subtree
	else if (key > root->key)
		root->right = deleteNode(root->right, key);

	// if the key is same as root's key, then this is the node to be deleted
	else {
		// node with only one child or no child
		if ((root->left == NULL) || (root->right == NULL)) {
			Node* temp = root->left ? root->left : root->right;

			// no child case
			if (temp == NULL) {
				temp = root;
				root = NULL;
			}
			else // one child case
				*root = *temp; // copy the contents of the non-empty child
			free(temp);
		}
		else {
			// node with two children: get the inorder successor (smallest in the right subtree)
			Node* temp = minValueNode(root->right);

			// copy the inorder successor's data to this node
			root->key = temp->key;

			// delete the inorder successor
			root->right = deleteNode(root->right, temp->key);
		}
	}

	// if the tree had only one node then return
	if (root == NULL)
		return root;

	// update height of the current node
	root->height = 1 + max(height(root->left), height(root->right));

	// get the balance factor of this node (to check whether this node became unbalanced)
	int balance = getBalance(root);

	// if this node becomes unbalanced, then there are 4 cases

	// left left case
	if (balance > 1) {
		if (getBalance(root->left) >= 0) {
			return rightRotate(root);
		}
		else {
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}
	}

	// right right case
	if (balance < -1) {
		if (getBalance(root->right) <= 0) {
			return leftRotate(root);
		}
		else {
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}
	}

	return root;
}

// print tree

void printTree(Node* root, string indent, bool last) {
	if (root != NULL) {
		cout << indent;
		if (last) {
			cout << "R----";
			indent += "   ";
		}
		else {
			cout << "L----";
			indent += "|  ";
		}
		cout << root->key << endl;
		printTree(root->left, indent, false);
		printTree(root->right, indent, true);
	}
}

int main() {
	Node* root = NULL;

	root = insert(root, 33);
	root = insert(root, 13);
	root = insert(root, 53);
	root = insert(root, 9);
	root = insert(root, 21);
	root = insert(root, 61);
	root = insert(root, 8);
	root = insert(root, 11);

	printTree(root, "", true);

	root = deleteNode(root, 13);

	cout << "After deletion" << endl;
	printTree(root, "", true);

	return 0;
}

