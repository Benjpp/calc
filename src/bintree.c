#include "interpreter.h"
#include "structures/bintree.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


node* create_node(){   
    node* n = malloc(sizeof(node));
    if(n == NULL ){
        printf("Error allocating space for node. Please retry");
        return NULL;        
    } 

    //I switch in between theese to test the tree or the parser in itself. Testing tree = malloc int. Testing parser = malloc(token)
    //I find it easier to test just using integers if im working on the bintree
    n->right = NULL;
    n->left = NULL;
    n->value = NULL;
    //n->value = malloc(sizeof(int));
    return n;
}

//Auxliary function, given the node passed as a parameter, returns 1 if it is a leaf, 0 otherwise
int is_leaf(node* n){
    return ((n->left == NULL) && (n->right == NULL)) ? 1 : 0;
}

//Insert a node and allocate the space for it before using its insert functions!!
//A node can be inserted to the right or left if the father doesn already have a son to that side and if the inserted node is a leaf
void insert_left(node* n, node* son){
    assert((n->left == NULL) &&  (is_leaf(son) == 1) && "Inserting left node: either already has left son, or the node appointed is not a leaf");
    n->left = son;
}

void insert_right(node* n, node* son){
    assert((n->right == NULL) &&  (is_leaf(son) == 1 ) && "Inserting right node: either already has right son, or the node appointed is not a leaf");
    n->right = son;
}

bintree create_tree(node* n){
    bintree* new_tree = malloc(sizeof(bintree));
    if(new_tree == NULL){
        printf("Error allocating space for new tree");
        //Consider creating a function that manages theese errors
        return;
    }
    new_tree->root = n;
    return *new_tree;
}

// Function to print the binary tree, shows the left son, then the right one afterwards
//The conversion from void to int pointer is made for testing the bintree
//Any other case convert to token and use the corresponding values for the tree
void display_bintree(node* root, int level){
    if (root == NULL){
        /*This parte is just usefull for debugging, good for checking if the nodes are set to null when they should
        for (int i = 0; i < level; i++){
            printf(i == level - 1 ? "|-" : "  ");
        }
        printf("X\n");*/
        return;
    }

    for (int i = 0; i < level; i++){
        printf(i == level - 1 ? "|-" : "  ");
    }
    if(root->value != NULL){
        print_token(*((token*)(root->value)));
    }else{
        printf("NULL\n");
    }
    //The first son printed is the left one
    display_bintree(root->left, level + 1);
    display_bintree(root->right, level + 1);
}
