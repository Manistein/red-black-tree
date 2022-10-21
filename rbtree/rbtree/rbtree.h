#ifndef _rbtree_h_
#define _rbtree_h_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum NodeColor {
	RBTN_BLACK = 0,
	RBTN_RED = 1,
	RBTN_COLOR_INVALID = 2,
};

typedef struct RBTreeNode {
	int val;
	int color;
	RBTreeNode* parent;
	RBTreeNode* left;
	RBTreeNode* right;
} RBTreeNode;

typedef struct RBTree {
	RBTreeNode* root;
} RBTree;

typedef struct RBTArray {
	RBTreeNode** arr;
	int size;
} RBTArray;

RBTree* rbtree_init();
RBTreeNode* rbtree_insert(RBTree* tree, int val);
RBTreeNode* rbtree_find(RBTree* tree, int val);
RBTreeNode* rbtree_remove(RBTree* tree, int val);
void rbtree_unint(RBTree* tree);
int rbtree_traverse(RBTreeNode* n, int parent_index, int level, RBTArray* array);

//void print_tree(RBTree* tree);

#endif