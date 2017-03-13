#ifndef _BST_H
#define _BST_H

#include <stdint.h>
#include <stdbool.h>

struct BST;
typedef struct BST BST;
struct BST {
	void* value;
	BST* left;
	BST* right;
	uint32_t level;
	bool (*lessThan)(void*, void*);
};

BST* BST_insert(BST* tree, void* val);
BST* BST_delete(BST* tree, void* val);
BST* BST_smallest_larger(BST* tree, void* val);
BST* BST_largest_smaller(BST* tree, void* val);

#endif