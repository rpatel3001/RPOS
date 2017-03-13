#include <util.h>
#include "BST.h"

// return if a node is a leaf
bool is_leaf(BST* tree) {
	return tree->left == 0 && tree->right == 0;
}

// perform the AA tree skew operation
BST* skew(BST* tree) {
	if (tree == 0) {
		return 0;
	} else if (tree->left == 0) {
		return tree;
	} else if (tree->level == tree->left->level) {
		BST* l = tree->left;
		tree->left = l->right;
		l->right = tree;
		return l;
	} else {
		return tree;
	}
}

// perform the AA tree split operation
BST* split(BST* tree) {
	if (tree == 0) {
		return 0;
	} else if (tree->right == 0 || tree->right->right == 0) {
		return tree;
	} else if (tree->level == tree->right->right->level) {
		BST* r = tree->right;
		tree->right = r->left;
		r->left = tree;
		r->level++;
		return r;
	} else {
		return tree;
	}
}

// return the node with the smallest value greater than or equal to val
BST* BST_smallest_larger(BST* tree, void* val) {
	if (tree == 0) {
		return 0;
	} else if (tree->lessThan(tree->value, val)) {
		return BST_smallest_larger(tree->right, val);
	} else {
		BST* bst = BST_smallest_larger(tree->left, val);
		return bst ? bst : tree;
	}
}

// return the node with the largest value less than or equal to val
BST* BST_largest_smaller(BST* tree, void* val) {
	if (tree == 0) {
		return 0;
	} else if (tree->lessThan(val, tree->value)) {
		return BST_largest_smaller(tree->left, val);
	} else {
		BST* bst = BST_largest_smaller(tree->right, val);
		return bst ? bst : tree;
	}
}

// fix the level of a node
BST* decrease_level(BST* tree) {
	uint32_t should_be = min(tree->left->level, tree->right->level) + 1;
	if (should_be < tree->level) {
		tree->level = should_be;
		if (should_be < tree->right->level) {
			tree->right->level = should_be;
		}
	}
	return tree;
}

// helper for BST_delete
BST* delete(BST* root, BST* tree, void* val) {
	if (tree == 0) {
		return 0;
	} else if (tree->lessThan(tree->value, val)) {
		tree->right = delete(root, tree->right, val);
	} else if (tree->lessThan(val, tree->value)) {
		tree->left = delete(root, tree->left, val);
	} else {
		if (is_leaf(tree)) {
			return 0;
		} else if (tree->left == 0) {
			BST* l = BST_smallest_larger(root, tree->value);
			tree->right = delete(root, tree->right, l->value);
			tree->value = l->value;
		} else {
			BST* l = BST_largest_smaller(root, tree->value);
			tree->left = delete(root, tree->left, l->value);
			tree->value = l->value;
		}
	}

	tree = decrease_level(tree);
	tree = skew(tree);
	tree->right = skew(tree->right);
	if (tree->right != 0) {
		tree->right->right = skew(tree->right->right);
	}
	tree = split(tree);
	tree->right = split(tree->right);
	return tree;
}

// delete the node with value == val
BST* BST_delete(BST* tree, void* val) {
	return delete(tree, tree, val);
}

// insert a new node with with value == val
BST* BST_insert(BST* tree, void* val) {
	if (tree->lessThan(val, tree->value)) {
		if (tree->left == 0) {
			BST bst = {.lessThan = tree->lessThan, .value = val};
			*tree->left = bst;
		} else {
			BST_insert(tree->left, val);
		}
	} else {
		if (tree->right == 0) {
			BST bst = {.lessThan = tree->lessThan, .value = val};
			*tree->right = bst;
		} else {
			BST_insert(tree->right, val);
		}
	}
	tree = skew(tree);
	tree = split(tree);
	return tree;
}