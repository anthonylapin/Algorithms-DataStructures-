#ifndef H_Dictionary
#define H_Dictionary

#include <iostream>
#include <iomanip>
using namespace std;

int max(int a, int b) //  function two get maximum of two ints
{
	return (a > b) ? a : b;
}

template <typename Key, typename Info>
class Dictionary
{
protected:
	struct Node
	{
		Key key;
		Info info;
		Node *left;
		Node *right;
		int height;
	};

	Node *root;

public:
	Dictionary() : root(nullptr) {}
	Dictionary(const Dictionary &);
	const Dictionary &operator=(const Dictionary &);

	void insert(const Key &key, const Info &info);
	void print() const;
	void inorderTraversal() const
	{
		inorder(root);
		cout << endl;
	}
	void postorderTraversal() const
	{
		postorder(root);
		cout << endl;
	}
	void preorderTraversal() const
	{
		preorder(root);
		cout << endl;
	}
	void setInfo(const Key &key, const Info &info)
	{
		setInfo(root, key, info);
	}

	void changeKey(const Key &pastkey, const Key &newKey)
	{
		Node *n = getNode(root, pastkey);
		if (n == NULL)
		{
			return;
		}
		Info i = n->info;
		root = deleteNode(root, pastkey);
		root = insert(root, newKey, i);
	}

	int treeHeight()
	{
		return getHeight(root);
	}

	int getFullCounts()
	{
		return getFullCounts(root);
	}
	void remove(const Key &key);
	bool search(const Key &key) const;
	bool empty() const
	{
		return (root == NULL);
	}

	int averageKey(Key k)
	{
		Node *node = getNode(root);
		if (node == NULL)
		{
			return 0;
		}
	}

	void clear();
	~Dictionary()
	{
		destroyNode(root);
	}

private:
	//  function to get the height of the tree
	int height(Node *N)
	{
		if (N == NULL)
			return 0;
		return N->height;
	}

	int getFullCounts(Node *r)
	{
		if (r == NULL)
		{
			return 0;
		}

		int res = 0;
		if (r->left && r->right)
		{
			res++;
		}

		res += (getFullCounts(r->left) + getFullCounts(r->right));
		return res;
	}

	Node *newNode(Key key, Info info)
	{
		Node *node = new Node();
		node->key = key;
		node->info = info;
		node->left = NULL;
		node->right = NULL;
		node->height = 1; // new node is initially
						  // added at leaf
		return (node);
	}

	void setInfo(Node *node, const Key &key, const Info &newInfo)
	{
		if (node == NULL)
		{
			return;
		}
		else if (node->key == key)
		{
			node->info = newInfo;
		}
		else if (node->key > key)
		{
			setInfo(node->left, key, newInfo);
		}
		else
		{
			setInfo(node->right, key, newInfo);
		}
	}

	Node *getNode(Node *r, const Key &key)
	{
		if (r->key == key)
		{
			return r;
		}
		else if (r->key > key)
		{
			return getNode(r->left, key);
		}
		else
		{
			return getNode(r->right, key);
		}
		return NULL;
	}

	// function to right rotate subtree rooted with y
	Node *rightRotate(Node *y)
	{
		Node *x = y->left;
		Node *t = x->right;

		// Performing rotation
		x->right = y;
		y->left = t;

		// Updating heights
		y->height = max(height(y->left),
						height(y->right)) +
					1;
		x->height = max(height(x->left),
						height(x->right)) +
					1;

		// Returning new root
		return x;
	}

	// function to left rotate subtree rooted with x
	Node *leftRotate(Node *x)
	{
		Node *y = x->right;
		Node *T2 = y->left;

		// Performing rotation
		y->left = x;
		x->right = T2;

		// Updating heights
		x->height = max(height(x->left),
						height(x->right)) +
					1;
		y->height = max(height(y->left),
						height(y->right)) +
					1;

		// Returning new root
		return y;
	}

	// Getting Balance factor of node
	int getBalance(Node *node)
	{
		if (node == NULL)
			return 0;
		return height(node->left) - height(node->right);
	}

	Node *insert(Node *node, const Key &key, const Info &info) // returns the new root of the subtree.
	{
		if (node == NULL)
			return (newNode(key, info));

		if (key < node->key)
			node->left = insert(node->left, key, info);
		else if (key > node->key)
			node->right = insert(node->right, key, info);
		else
			return node;

		node->height = 1 + max(height(node->left),
							   height(node->right));

		int balance = getBalance(node);

		// Left Left Case
		if (balance > 1 && key < node->left->key)
			return rightRotate(node);

		// Right Right Case
		if (balance < -1 && key > node->right->key)
			return leftRotate(node);

		// Left Right Case
		if (balance > 1 && key > node->left->key)
		{
			node->left = leftRotate(node->left);
			return rightRotate(node);
		}

		// Right Left Case
		if (balance < -1 && key < node->right->key)
		{
			node->right = rightRotate(node->right);
			return leftRotate(node);
		}

		return node;
	}

	Node *minValueNode(Node *node)
	{
		Node *current = node;

		// loop down to find the leftmost node
		while (current->left != NULL)
			current = current->left;

		return current;
	}

	Node *deleteNode(Node *root, int key)
	{

		//  standart bst delete method
		if (root == NULL)
			return root;

		if (key < root->key)
			root->left = deleteNode(root->left, key);

		else if (key > root->key)
			root->right = deleteNode(root->right, key);

		else
		{
			// node with only one child or no child
			if ((root->left == NULL) ||
				(root->right == NULL))
			{
				Node *temp = root->left ? root->left : root->right;

				// No child case
				if (temp == NULL)
				{
					temp = root;
					root = NULL;
				}
				else
					*root = *temp;

				delete temp;
			}
			else
			{

				Node *temp = minValueNode(root->right);

				root->key = temp->key;

				root->right = deleteNode(root->right,
										 temp->key);
			}
		}

		if (root == NULL)
			return root;

		root->height = 1 + max(height(root->left),
							   height(root->right));

		int balance = getBalance(root);

		// If this node becomes unbalanced,
		// then 4 cases of rotation

		// Left Left Case
		if (balance > 1 &&
			getBalance(root->left) >= 0)
			return rightRotate(root);

		// Left Right Case
		if (balance > 1 &&
			getBalance(root->left) < 0)
		{
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}

		// Right Right Case
		if (balance < -1 &&
			getBalance(root->right) <= 0)
			return leftRotate(root);

		// Right Left Case
		if (balance < -1 &&
			getBalance(root->right) > 0)
		{
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}

		return root;
	}

	void graph(Node *const &subtreeRoot, int indent) const // make a graph of avl tree.
	{
		if (subtreeRoot != NULL)
		{
			graph(subtreeRoot->right, indent + 8);
			cout << setw(indent) << " " << subtreeRoot->key << " ";
			graph(subtreeRoot->left, indent + 8);
		}
		else
			cout << setw(indent) << " " << endl;
	}

	Node *search(Node *const &r, const Key &k) const
	{
		if (r == NULL || r->key == k)
		{
			return r;
		}

		if (root->key < k)
		{
			return search(r->right, k);
		}

		return search(r->left, k);
	}

	int getHeight(Node *r)
	{
		if (r == NULL)
		{
			return 0;
		}

		int lh = getHeight(r->left);
		int rh = getHeight(r->right);
		return 1 + max(lh, rh);
	}

	void destroyNode(Node *&node)
	{
		if (node != NULL)
		{
			destroyNode(node->left);
			destroyNode(node->right);
			delete node;
			node = NULL;
		}
	}

	void copy(Node *&copied, Node *other)
	{
		if (!other)
		{
			copied = NULL;
		}
		else
		{
			copied = new Node;
			copied->key = other->key;
			copied->info = other->info;
			copied->height = other->height;
			copy(copied->left, other->left);
			copy(copied->right, other->right);
		}
	}

	void inorder(Node *const &node) const
	{
		if (node != NULL)
		{
			inorder(node->left);
			cout << "Key: " << node->key << " "
				 << "Info: " << node->info << "\t;";
			inorder(node->right);
		}
	}

	void postorder(Node *const &node) const
	{
		if (node != NULL)
		{
			postorder(node->left);
			postorder(node->right);
			cout << "Key: " << node->key << " "
				 << "Info: " << node->info << "\t;";
		}
	}

	void preorder(Node *const &node) const
	{
		if (node != NULL)
		{
			cout << "Key: " << node->key << " "
				 << "Info: " << node->info << "\t;";
			preorder(node->left);
			preorder(node->right);
		}
	}
};

template <typename Key, typename Info>
Dictionary<Key, Info>::Dictionary(const Dictionary<Key, Info> &otherD)
{
	if (otherD.root == NULL)
	{
		root = NULL;
	}
	else
	{
		copy(root, otherD.root);
	}
}

template <typename Key, typename Info>
const Dictionary<Key, Info> &Dictionary<Key, Info>::operator=(const Dictionary<Key, Info> &other)
{
	if (this != &other)
	{
		if (this->root != NULL)
		{
			destroyNode(root);
		}

		if (other.root == NULL)
		{
			root = NULL;
		}
		else
		{
			copy(root, other.root);
		}
	}
	return *this;
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::insert(const Key &key, const Info &info)
{
	root = insert(root, key, info);
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::print() const
{
	graph(root, 0);
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::remove(const Key &key)
{
	root = deleteNode(root, key);
}

template <typename Key, typename Info>
bool Dictionary<Key, Info>::search(const Key &key) const
{
	Node *searchElement = search(root, key);
	if (searchElement)
	{
		return true;
	}
	return false;
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::clear()
{
	destroyNode(root);
}

#endif