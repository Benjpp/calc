#ifndef BINTREE_H
#define BINTREE_H

typedef struct node{
    struct node* right;
    struct node* left;
    void* value;
}node;

typedef struct{
    node* root;
}bintree;


/**
 * @brief: Allocates space for a new node in the tree
 *
 * @return: A pointer to the new node
 */
node* create_node();

/**
 * @brief: Appends a son to the right of a node
 * 
 * @param n: node to which append son to the right
 * 
 * @param right: node to be appended
 */
void insert_right(node* n, node* right);

/**
 * @brief: Appends a son to the right of a node
 * 
 * @param n: node to which append son to the right
 * 
 * @param left: node to be appended
 */
void insert_left(node* n, node* left);

/**
 * @brief: creates a new binary tree with only a root node
 * 
 * @param root: root node of the new binary tree
 *
 * @return: the newly created bintree 
 */
bintree create_tree(node* root);

/**
 * @brief: Prints the contents of the binary tree. Recursive function
 * 
 * @param tree: root node of the tree to print
 * 
 * @param level: level on which the recursive call is on, called with 0 initially
 */
void display_bintree(node* root, int level);

#endif //BINTREE_H
