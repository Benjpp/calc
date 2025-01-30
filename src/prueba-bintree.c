#include "structures/bintree.h"
#include <stdio.h>

int main(int argc, char* argv[]){
    node* root = create_node();
    
    //Prueba del arbol binario con valores enteros
    node* hijo1 = create_node();    
    node* hijo2 = create_node();
    node* hijo3 = create_node();

    printf("Nodos hijo y raiz creados... \n");

    //Asignamos enteros a los hijos de prueba
    printf("%p \n\n",  hijo1->value);
    *((int*)(hijo1->value)) = 1;
    *((int*)(hijo2->value)) = 2;
    *((int*)(hijo3->value)) = 3;

    *((int*)(root->value)) = 0;
    printf("Valores asignados. Valor de root = %d... \n", *((int*)(root->value)));
    
    //creamos el arbol con root como nodo raiz
    bintree tree = create_tree(root);

    printf("Arbol creado");

    insert_right(tree.root, hijo2);
    insert_left(tree.root, hijo1);
    printf("Hijos a izquierda y derecha de root insertados... \n");
    
    insert_left(tree.root->left, hijo3);
    display_bintree(tree.root, 0);

    /*
    Arbol esperado de esta prueba
                root
                 /\
                /  \
               /    \
            hijo1   hijo2
            /
           /
          /
        hijo3

    */  


}