#include<string>
#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;

class Node {
public:
	int value;
	int height;
	int balanced_factor;
	Node* left_child;
	Node* right_child;
	Node();
	Node(int num);
	void print();
};

class AVLtree {
public:
	Node* root = nullptr;
	void add_node(int num);
	void delete_node(int num);
	void pre_order_print();
	void post_order_print();
	void in_order_print();
private:
	void adjust_tree();
	Node* find_parent(int num, Node* ptr);
	void delete_leaf(int num, Node* parent);
	void calculate_height(Node* ptr);
	void calculate_balance_factor(Node* ptr);
	bool is_balanced(Node* ptr);
	bool is_balance_node(Node* ptr);
	Node* find_nearest_unbalanced_node(Node* ptr);
	void search_unbalanced_node(Node* ptr, vector<Node*>* list);
	void pre_order_rec(Node* ptr);
	void post_order_rec(Node* ptr);
	void in_order_rec(Node* ptr);
	void swap_value(Node* ptr1, Node* ptr2);
	void left_rotate(Node* ptr, Node* parent);
	void right_rotate(Node* ptr, Node* parent);
	void left_right_rotate(Node* ptr, Node* parent);
	void right_left_rotate(Node* ptr, Node* parent);
	void refresh_tree();
};

int main() {
	AVLtree tree;
	string s;

	while (cin >> s) {
		if (s[0] == 'A') {
			int num = stoi(s.substr(1));
			tree.add_node(num);
		}
		else if (s[0] == 'D') {
			int num = stoi(s.substr(1));
			tree.delete_node(num);
		}
		else if (s == "PRE") {
			tree.pre_order_print();
		}
		else if (s == "IN") {
			tree.in_order_print();
		}
		else if (s == "POST") {
			tree.post_order_print();
		}
	}
}

Node::Node()
{
	this->left_child = nullptr;
	this->right_child = nullptr;
}
Node::Node(int num)
{
	this->value = num;
	left_child = nullptr;
	right_child = nullptr;
}
void Node::print()
{
	cout << this->value << " ";
}
void AVLtree::add_node(int num)
{
	if (root == nullptr) {
		root = new Node(num);
	}
	else {
		Node* ptr = root;
		while (true) {
			if (num == ptr->value) {
				return;
			}

			if (num < ptr->value) {
				if (ptr->left_child == nullptr) {
					ptr->left_child = (new Node(num));
					break;
				}
				else {
					ptr = ptr->left_child;
				}
			}
			else {
				if (ptr->right_child == nullptr) {
					ptr->right_child = new Node(num);
					break;
				}
				else {
					ptr = ptr->right_child;
				}
			}
		}
	}
	refresh_tree();

	while (!is_balanced(root)) {
		adjust_tree();
		refresh_tree();
	}
}
/*
* delete a node for an avl tree
*/
void AVLtree::delete_node(int num)
{
	if (root == nullptr) {
		return;
	}
	//case that the tree is empty

	if (root->value == num) {
		if (root->height == 1) {
			root = nullptr;
			return;
		}
		//case that delete the only node
		else {
			if (root->left_child == nullptr || root->right_child == nullptr) {
				root = root->left_child == nullptr ? root->right_child : root->left_child;
			}
			//case that the node to be deleted only has one child tree
			else {
				Node* switched_node = root->left_child;
				Node* switched_parent = root;
				bool is_left_child = true;
				while (switched_node->right_child != nullptr) {
					switched_parent = switched_node;
					switched_node = switched_node->right_child;
					is_left_child = false;
				}
				swap_value(root, switched_node);
				//find the right most node in the left child tree and swap the value

				if (switched_node->height == 1) {
					if (!is_left_child) {
						switched_parent->right_child = nullptr;
					}
					else {
						switched_parent->left_child = nullptr;
					}
				}
				//if after swap the node to be deleted is a leave node
				else {
					if (!is_left_child) {
						switched_parent->right_child = switched_node->left_child;
					}
					else {
						switched_parent->left_child = switched_node->left_child;
					}
				}
				//if after swap the node to be deleted has a left child
				//delete the node
			}
		}
	}
	//case that delete root

	else {
		Node* found_parent = find_parent(num, root);
		if (found_parent == nullptr) {
			return;
		}
		//case of no such node to be deleted
		else {
			Node* found = num < found_parent->value ? found_parent->left_child : found_parent->right_child;
			if (found->height == 1) {
				if (num < found_parent->value) {
					found_parent->left_child = nullptr;
				}
				else {
					found_parent->right_child = nullptr;
				}
			}
			//case that it is a leave node
			else {
				if (found->left_child == nullptr || found->right_child == nullptr) {
					Node* child = found->left_child == nullptr ? found->right_child : found->left_child;
					if (num < found_parent->value) {
						found_parent->left_child = child;
					}
					else {
						found_parent->right_child = child;
					}
				}
				//case that it has one child tree
				else {
					Node* switched = found->left_child;
					Node* switched_parent = found;
					bool is_left_child = true;
					while (switched->right_child != nullptr) {
						switched_parent = switched;
						switched = switched->right_child;
						is_left_child = false;
					}
					swap_value(switched, found);
					//find the right most node in the left child tree and swap the value
					if (switched->height == 1) {
						if (!is_left_child) {
							switched_parent->right_child = nullptr;
						}
						else {
							switched_parent->left_child = nullptr;
						}
					}
					//if after swap the node to be deleted is a leave node
					else {
						if (!is_left_child) {
							switched_parent->right_child = switched->left_child;
						}
						else {
							switched_parent->left_child = switched->left_child;
						}
					}
					//if after swap the node to be deleted has a left child
				 //case that it has both child trees
				}
			}
		}
		//case that the node to be deleted is not root
	}

	refresh_tree();
	while (!is_balanced(root)) {
		adjust_tree();
		refresh_tree();
	}
}
/*
* delete a leave node by its value and parent node
*/
void AVLtree::delete_leaf(int num, Node* parent) {
	if (parent->left_child != nullptr && parent->left_child->value == num) {
		parent->left_child = nullptr;
	}
	else {
		parent->right_child = nullptr;
	}
}
/*
* this method find a node by a given value and returns its parent node
*/
Node* AVLtree::find_parent(int num, Node* ptr)
{
	if (ptr->value == num) {
		return nullptr;
	}
	else if (num < ptr->value) {
		if (ptr->left_child != nullptr) {
			if (ptr->left_child->value == num) {
				return ptr;
			}
			else {
				find_parent(num, ptr->left_child);
			}
		}
		else {
			return nullptr;
		}
	}
	else {
		if (ptr->right_child != nullptr) {
			if (ptr->right_child->value == num) {
				return ptr;
			}
			else {
				find_parent(num, ptr->right_child);
			}
		}
		else {
			return nullptr;
		}
	}
}
void AVLtree::pre_order_print()
{
	if (root == nullptr) {
		cout << "EMPTY" << endl;
	}
	else {
		pre_order_rec(root);
		cout << endl;
	}

}
void AVLtree::post_order_print()
{
	if (root == nullptr) {
		cout << "EMPTY" << endl;
	}
	else {
		post_order_rec(root);
	}
	cout << endl;
}
void AVLtree::in_order_print()
{
	if (root == nullptr) {
		cout << "EMPTY" << endl;
	}
	else {
		in_order_rec(root);
	}
	cout << endl;
}
void AVLtree::pre_order_rec(Node* ptr)
{
	if (ptr == nullptr) {
		return;
	}
	else {
		ptr->print();
		if (ptr->left_child != nullptr) {
			pre_order_rec(ptr->left_child);
		}
		if (ptr->right_child != nullptr) {
			pre_order_rec(ptr->right_child);
		}
	}
}
void AVLtree::post_order_rec(Node* ptr) {
	if (ptr == nullptr) {
		return;
	}
	else {
		if (ptr->left_child != nullptr) {
			post_order_rec(ptr->left_child);
		}
		if (ptr->right_child != nullptr) {
			post_order_rec(ptr->right_child);
		}
		ptr->print();
	}
}
void AVLtree::in_order_rec(Node* ptr) {
	if (ptr == nullptr) {
		return;
	}
	else {
		if (ptr->left_child != nullptr) {
			in_order_rec(ptr->left_child);
		}
		ptr->print();
		if (ptr->right_child != nullptr) {
			in_order_rec(ptr->right_child);
		}
	}
}
/*
* calculate the height of each node recursively
*/
void AVLtree::calculate_height(Node* ptr) {
	if (ptr == nullptr) {
		return;
	}
	if (ptr->left_child == nullptr && ptr->right_child == nullptr) {
		ptr->height = 1;
		return;
	}

	int left_child_height = 0;
	if (ptr->left_child != nullptr) {
		calculate_height(ptr->left_child);
		left_child_height = ptr->left_child->height;
	}
	int right_child_height = 0;
	if (ptr->right_child != nullptr) {
		calculate_height(ptr->right_child);
		right_child_height = ptr->right_child->height;
	}
	ptr->height = max(left_child_height, right_child_height) + 1;
}
/*
* calculate the balance factor recursively
*/
void AVLtree::calculate_balance_factor(Node* ptr) {
	int left_child_height = ptr->left_child == nullptr ? 0 : ptr->left_child->height;
	int right_child_height = ptr->right_child == nullptr ? 0 : ptr->right_child->height;
	ptr->balanced_factor = left_child_height - right_child_height;
	if (ptr->left_child != nullptr) {
		calculate_balance_factor(ptr->left_child);
	}
	if (ptr->right_child != nullptr) {
		calculate_balance_factor(ptr->right_child);
	}
}
/*
* adjust tree to be an avl tree
*/
void AVLtree::adjust_tree() {
	Node* unbalanced_node = find_nearest_unbalanced_node(root);
	Node* parent = find_parent(unbalanced_node->value, root);

	if (unbalanced_node->balanced_factor > 0) {
		if (unbalanced_node->left_child->balanced_factor >= 0) {
			right_rotate(unbalanced_node, parent);
		}
		else {
			left_right_rotate(unbalanced_node, parent);
		}
	}
	else {
		if (unbalanced_node->right_child->balanced_factor <= 0) {
			left_rotate(unbalanced_node, parent);
		}
		else {
			right_left_rotate(unbalanced_node, parent);
		}
	}
}
/*
* check if the tree is a balanced tree
*/
bool AVLtree::is_balanced(Node* ptr) {
	if (ptr == nullptr) {
		return true;
	}
	if (ptr->left_child == nullptr && ptr->right_child == nullptr) {
		return is_balance_node(ptr);
	}
	else if (ptr->left_child == nullptr && ptr->right_child != nullptr) {
		return is_balance_node(ptr) && is_balanced(ptr->right_child);
	}
	else if (ptr->left_child != nullptr && ptr->right_child == nullptr) {
		return is_balance_node(ptr) && is_balanced(ptr->left_child);
	}
	else {
		return is_balanced(ptr->right_child) && is_balanced(ptr->left_child) && is_balance_node(ptr);
	}
}
/*
* swap the value of two node
*/
void AVLtree::swap_value(Node* ptr1, Node* ptr2) {
	int temp = ptr1->value;
	ptr1->value = ptr2->value;
	ptr2->value = temp;
}
/*
* find the lowest unbalanced node
*/
Node* AVLtree::find_nearest_unbalanced_node(Node* ptr) {
	vector<Node*> unbalanced_nodes;
	search_unbalanced_node(ptr, &unbalanced_nodes);
	//cout << "number of unbalaned nodes" << unbalanced_nodes.size() << endl;
	Node* nearest_unbalanced_node = unbalanced_nodes[0];
	for (Node* node : unbalanced_nodes) {
		if (node->height < nearest_unbalanced_node->height) {
			nearest_unbalanced_node = node;
		}
	}
	return nearest_unbalanced_node;
}
/*
* search the unbalanced node recursively and return a vector 
*/
void AVLtree::search_unbalanced_node(Node* ptr, vector<Node*>* list) {
	if (!(is_balance_node(ptr))) {
		list->push_back(ptr);
	}
	if (ptr->left_child != nullptr) {
		search_unbalanced_node(ptr->left_child, list);
	}
	if (ptr->right_child != nullptr) {
		search_unbalanced_node(ptr->right_child, list);
	}
}
/*
* judge if the node is balanced
*/
bool AVLtree::is_balance_node(Node* ptr) {
	if (ptr->balanced_factor<2 && ptr->balanced_factor>-2) {
		return true;
	}
	else {
		return false;
	}
}
/*
* left rotation
* @param Node* ptr is the unbalanced node, Node* parent is the parent node of the former node
*/
void AVLtree::left_rotate(Node* ptr, Node* parent) {
	Node* v = ptr;
	Node* x = v->right_child;
	Node* w = x->right_child;

	Node* a = v->left_child;
	Node* b = x->left_child;

	if (parent == nullptr) {
		root = x;
	}
	else {
		if (v->value < parent->value) {
			parent->left_child = x;
		}
		else {
			parent->right_child = x;
		}
	}

	x->left_child = v;
	x->right_child = w;
	v->left_child = a;
	v->right_child = b;
}
/*
* left right rotation for avltree
* @param Node* ptr is the unbalanced node, Node* parent is the parent node of the former node
*/
void AVLtree::left_right_rotate(Node* ptr, Node* parent) {
	Node* v = ptr;
	Node* x = v->left_child;
	Node* w = x->right_child;

	Node* a = x->left_child;
	Node* b = w->left_child;
	Node* c = w->right_child;
	Node* d = v->right_child;

	if (parent == nullptr) {
		root = w;
	}
	else {
		if (v->value < parent->value) {
			parent->left_child = w;
		}
		else {
			parent->right_child = w;
		}
	}

	w->left_child = x;
	w->right_child = v;
	v->left_child = c;
	v->right_child = d;
	x->left_child = a;
	x->right_child = b;
}
/*
* right right rotation for avltree
* @param Node* ptr is the unbalanced node, Node* parent is the parent node of the former node
*/
void AVLtree::right_rotate(Node* ptr, Node* parent) {
	Node* v = ptr;
	Node* x = v->left_child;
	Node* w = x->left_child;

	Node* a = v->right_child;
	Node* b = x->right_child;

	if (parent == nullptr) {
		root = x;
	}
	else {
		if (v->value < parent->value) {
			parent->left_child = x;
		}
		else {
			parent->right_child = x;
		}
	}
	x->left_child = w;
	x->right_child = v;
	v->left_child = b;
	v->right_child = a;
}
/*
* right_left right rotation for avltree
* @param Node* ptr is the unbalanced node, Node* parent is the parent node of the former node
*/
void AVLtree::right_left_rotate(Node* ptr, Node* parent) {
	Node* v = ptr;
	Node* x = v->right_child;
	Node* w = x->left_child;

	Node* a = v->left_child;
	Node* b = w->left_child;
	Node* c = w->right_child;
	Node* d = x->right_child;

	if (parent == nullptr) {
		root = w;
	}
	else {
		if (v->value < parent->value) {
			parent->left_child = w;
		}
		else {
			parent->right_child = w;
		}
	}

	w->left_child = v;
	w->right_child = x;
	v->left_child = a;
	v->right_child = b;
	x->left_child = c;
	x->right_child = d;
}
/*
* recalculate the height and balance factor of each node
*/
void AVLtree::refresh_tree() {
	if (root == nullptr) {
		return;
	}
	else {
		calculate_height(root);
		calculate_balance_factor(root);
	}
}