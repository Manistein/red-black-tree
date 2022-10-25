#include "rbtree.h"

#define max(a, b) (a >= b ? a : b)

static RBTreeNode* get_uncle(RBTreeNode* n) {
	if (n->parent == NULL)
		return NULL;

	RBTreeNode* grandfather = n->parent->parent;
	if (grandfather == NULL) {
		return NULL;
	}

	if (n->parent == grandfather->left) {
		return grandfather->right;
	}
	else {
		return grandfather->left;
	}
}

static RBTreeNode* get_subling(RBTreeNode* n) {
	if (n->parent == NULL) {
		return NULL;
	}

	if (n == n->parent->left) {
		return n->parent->right;
	}
	else {
		return n->parent->left;
	}
}

static bool has_red_children(RBTreeNode* n) {
	if (n->left == NULL && n->right == NULL) {
		return false;
	}

	if (n->left && n->left->color == RBTN_RED) {
		return true;
	}

	if (n->right && n->right->color == RBTN_RED) {
		return true;
	}

	return false;
}

static void left_rotate(RBTree* tree, RBTreeNode* n) {
	assert(n->right != NULL);
	RBTreeNode* right_child = n->right;
	RBTreeNode* left_grandson = right_child->left;

	right_child->parent = n->parent;
	right_child->left = n;

	if (n->parent) {
		if (n == n->parent->left) {
			n->parent->left = right_child;
		}
		else {
			n->parent->right = right_child;
		}
	}
	else {
		tree->root = right_child;
	}

	n->parent = right_child;
	n->right = left_grandson;

	if (left_grandson) {
		left_grandson->parent = n;
	}
}

static void right_rotate(RBTree* tree, RBTreeNode* n) {
	assert(n->left != NULL);
	RBTreeNode* left_child = n->left;
	RBTreeNode* right_grandson = left_child->right;

	left_child->parent = n->parent;
	left_child->right = n;

	if (n->parent) {
		if (n == n->parent->left) {
			n->parent->left = left_child;
		}
		else {
			n->parent->right = left_child;
		}
	}
	else {
		tree->root = left_child;
	}

	n->parent = left_child;
	n->left = right_grandson;

	if (right_grandson) {
		right_grandson->parent = n;
	}
}

static RBTreeNode* search(RBTreeNode* parent, RBTreeNode* node, int val) {
	if (node == NULL)
		return parent;

	if (node->val == val) {
		return node;
	}

	RBTreeNode* ret = NULL;
	if (val <= node->val) {
		ret = search(node, node->left, val);
	}
	else {
		ret = search(node, node->right, val);
	}

	return ret;
}

static void swap_color(RBTreeNode* n1, RBTreeNode* n2) {
	int temp_color = n1->color;
	n1->color = n2->color;
	n2->color = temp_color;
}

static void fix_red_red(RBTree* tree, RBTreeNode* n) {
	RBTreeNode* parent = n->parent;

	// n is root ? see case 1
	if (parent == NULL) {
		n->color = RBTN_BLACK;
		return;
	}

	RBTreeNode* grandfather = parent->parent;
	// parent is root? see case 2
	if (grandfather == NULL) { 
		parent->color = RBTN_BLACK;
		n->color = RBTN_RED;
		return;
	}

	// see case 3
	if (parent->color == RBTN_BLACK) {
		return;
	}

	// now node and it's parent are all red
	assert(grandfather->color == RBTN_BLACK);
	RBTreeNode* uncle = get_uncle(n);
	if (uncle) {
		if (uncle->color == RBTN_RED) {
			parent->color = uncle->color = RBTN_BLACK;
			grandfather->color = RBTN_RED;

			fix_red_red(tree, grandfather);
		}
		else {
			if (parent == grandfather->left) {
				if (n == parent->right) {
					left_rotate(tree, parent);
					right_rotate(tree, grandfather);
					swap_color(n, grandfather);
				}
				else {
					right_rotate(tree, grandfather);
					swap_color(parent, grandfather);
				}
			}
			else {
				if (n == parent->left) {
					right_rotate(tree, parent);
					left_rotate(tree, grandfather);
					swap_color(n, grandfather);
				}
				else {
					left_rotate(tree, grandfather);
					swap_color(parent, grandfather);
				}
			}
		}
	}
	else {
		if (parent == grandfather->left) {
			if (n == parent->right) {
				left_rotate(tree, parent);
				right_rotate(tree, grandfather);
				swap_color(n, grandfather);
			}
			else {
				right_rotate(tree, grandfather);
				swap_color(parent, grandfather);
			}
		}
		else {
			if (n == parent->right) {
				left_rotate(tree, grandfather);
				swap_color(parent, grandfather);
			}
			else {
				right_rotate(tree, parent);
				left_rotate(tree, grandfather);
				swap_color(n, grandfather);
			}
		}
	}
}

static void fix_black(RBTree* tree, RBTreeNode* node) {
	if (node == NULL || node->color != RBTN_BLACK)
		return;

	RBTreeNode* parent = node->parent;
	RBTreeNode* uncle = get_uncle(node);
	RBTreeNode* subling = get_subling(node);

	// black node must has a subling
	if (subling == NULL) {
		return;
	}

	if (parent->color == RBTN_RED) {
		if (has_red_children(subling)) { // subling is black and has red children
			if (node == parent->left) {
				if (subling->right) {
					left_rotate(tree, parent);
					swap_color(subling, parent);
					subling->right->color = parent->color;
				}
				else {
					right_rotate(tree, subling);
					swap_color(subling, subling->left);
					left_rotate(tree, parent);
					swap_color(parent, subling->left);
					subling->color = parent->color;
				}
			}
			else {
				if (subling->left) {
					right_rotate(tree, parent);
					swap_color(subling, parent);
					subling->left->color = parent->color;
				}
				else {
					left_rotate(tree, subling);
					swap_color(subling, subling->right);
					right_rotate(tree, parent);
					swap_color(subling->right, parent);
					subling->color = parent->color;
				}
			}
		}
		else if (subling->left || subling->right) { // subling is black and has black children
			if (node == parent->left) {
				left_rotate(tree, parent);
			}
			else {
				right_rotate(tree, parent);
			}
		}
		else { // subling has no child
			parent->color = RBTN_BLACK;
			subling->color = RBTN_RED;
		}
	}
	else {
		if (subling->color == RBTN_BLACK) {
			if (has_red_children(subling)) {
				if (node == parent->left) {
					if (subling->right) {
						left_rotate(tree, parent);
						subling->right->color = RBTN_BLACK;
					}
					else {
						right_rotate(tree, subling);
						left_rotate(tree, parent);
						subling->left->color = RBTN_BLACK;
					}
				}
				else {
					if (subling->left) {
						right_rotate(tree, parent);
						subling->left->color = RBTN_BLACK;
					}
					else {
						left_rotate(tree, subling);
						right_rotate(tree, parent);
						subling->right->color = RBTN_BLACK;
					}
				}
			}
			else {
				subling->color = RBTN_RED;
				fix_black(tree, parent);
			}
		}
		else { // subling is red
			RBTreeNode* close_nephew = NULL;
			if (node == parent->left) {
				close_nephew = subling->left;
				left_rotate(tree, parent);
			}
			else {
				close_nephew = subling->right;
				right_rotate(tree, parent);
			}

			RBTreeNode* new_subling = get_subling(node);
			if (has_red_children(new_subling)) {
				fix_black(tree, node);
				swap_color(new_subling, subling);
			}
			else if (new_subling->left || new_subling->right) {
				swap_color(new_subling, subling);
			}
			else {
				swap_color(new_subling, subling);
			}
		}
	}
}

RBTree* rbtree_init() {
	RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
	if (tree)
		tree->root = NULL;
	return tree;
}

RBTreeNode* rbtree_insert(RBTree* tree, int val) {
	if (tree == NULL) {
		return NULL;
	}

	// node has been inserted?
	RBTreeNode* n = search(NULL, tree->root, val);
	if (n && n->val == val) {
		return n;
	}

	RBTreeNode* node = (RBTreeNode*)malloc(sizeof(RBTreeNode));
	node->parent = node->left = node->right = NULL;
	node->val = val;
	node->color = RBTN_RED;

	// tree is empty
	if (n == NULL) {
		node->color = RBTN_BLACK;
		tree->root = node;
		return tree->root;
	}
	else { // insert into n
		if (val <= n->val) { // node is left child of n
			assert(n->left == NULL);
			n->left = node;
			node->parent = n;
		}
		else { // node is right child of n
			assert(n->right == NULL);
			n->right = node;
			node->parent = n;
		}

		fix_red_red(tree, node);

		return node;
	}
}

RBTreeNode* rbtree_find(RBTree* tree, int val) {
	return search(NULL, tree->root, val);
}

static RBTreeNode* successor(RBTreeNode* node) {
	while (node) {
		if (node->left) {
			node = node->left;
		}
		else {
			break;
		}
	}

	return node;
}

static RBTreeNode* find_replace(RBTreeNode* node) {
	if (node->left && node->right) {
		return successor(node->right);
	}

	if (node->left == NULL && node->right == NULL)
		return NULL;

	if (node->left)
		return node->left;
	else
		return node->right;
}

bool rbtree_remove(RBTree* tree, int val) {
	RBTreeNode* node = tree->root;
	while (node) {
		if (val == node->val)
			break;

		if (val < node->val)
			node = node->left;
		else
			node = node->right;
	}

	if (node == NULL) {
		return false;
	}

	RBTreeNode* parent = node->parent;
	RBTreeNode* uncle = get_uncle(node);
	RBTreeNode* subling = get_subling(node);

	RBTreeNode* rnode = find_replace(node);
	if (rnode == NULL) { // node is leaf ?
		if (parent == NULL) {
			tree->root = NULL;
			free(node);

			return true;
		}

		if (node->color == RBTN_BLACK)  {
			assert(subling != NULL);
			fix_black(tree, node);
		}

		if (node == parent->left)
			parent->left = NULL;
		else
			parent->right = NULL;

		node->parent = NULL;
	}
	else {
		if (rnode->color == RBTN_RED) {
			node->val = rnode->val;
		}
		else {
			node->val = rnode->val;
			fix_black(tree, rnode);
		}

		if (rnode == node) {
			node->right = rnode->right;
			if (rnode->right) {
				rnode->right->parent = node;
			}
		}
		else {
			if (rnode == rnode->parent->left)
				rnode->parent->left = NULL;
			else
				rnode->parent->right = NULL;
		}

		rnode->parent = NULL;
		node = rnode;
	}

	free(node);
	return true;
}

static void resize_array(RBTArray* array, int new_size) {
	if (new_size <= 0) {
		if (array->size > 0) {
			free(array->arr);
			array->arr = NULL;
			array->size = 0;
		}
	}
	else {
		int old_size = array->size;
		array->arr = (RBTreeNode**)realloc(array->arr, ((size_t)new_size) * sizeof(RBTreeNode*));
		for (int i = old_size; i < new_size; i++) {
			array->arr[i] = NULL;
		}
		array->size = new_size;
	}
}

int rbtree_traverse(RBTreeNode* n, int parent_index, int level, RBTArray* array) {
	int max_level = level;

	if (n == NULL) {
		return level;
	}

	int index;
	if (n->parent == NULL) {
		index = 0;
	}
	else if (n == n->parent->left) {
		index = 2 * parent_index + 1;
	}
	else {
		index = 2 * parent_index + 2;
	}

	if (array) {
		if (index >= array->size) {
			int new_size = max(array->size * 2, 4);
			resize_array(array, new_size);
		}
		array->arr[index] = n;
	}

	if (n->left != NULL) {
		max_level = max(rbtree_traverse(n->left, index, level + 1, array), max_level);
	}

	if (n->right != NULL) {
		max_level = max(rbtree_traverse(n->right, index, level + 1, array), max_level);
	}

	return max_level;
}

//void print_tree(RBTree* tree) {
//	Array* array = (Array*)malloc(sizeof(Array));
//	if (array == NULL) {
//		return;
//	}
//	array->arr = NULL;
//	array->size = 0;
//
//	int index = 0;
//	int depth = traverse(tree->root, 0, 1, array);
//
//	for (int i = 0; i < array->size; i++) {
//		Node* n = array->arr[i];
//		if (n) {
//			printf("[%d|(%c:%d)]", i, s_color[n->color], n->val);
//		}
//		else {
//			printf("[%d|(null)]", i);
//		}
//	}
//	printf("\n");
//	
//	resize_array(array, 0);
//	free(array);
//}

void rbtree_unint(RBTree* tree) {
	RBTArray* array = (RBTArray*)malloc(sizeof(RBTArray));
	if (array == NULL) {
		abort();
	}
	array->arr = NULL;
	array->size = 0;

	rbtree_traverse(tree->root, 0, 1, array);

	for (int i = 0; i < array->size; i++) {
		RBTreeNode* n = array->arr[i];
		if (n) {
			free(n);
			array->arr[i] = NULL;
		}
	}

	tree->root = NULL;
	free(array);
}