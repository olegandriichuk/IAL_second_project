/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree) {
    (*tree) = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

   if(tree == NULL){
       return false;
   } else{
        while (tree != NULL){
            // if the key was found, update the value
            if(tree->key == key){
                *value = tree->value;
                return true;
                // if the key is greater, search in right subtree
            } else if(tree->key < key){
                tree = tree->right;
                // if the key is smaller, search in left subtree
            } else{
                tree = tree->left;
            }
        }
   }
   return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value)
{
    if (*tree == NULL){
        // create the new node
        bst_node_t* new_node = malloc(sizeof(bst_node_t));
        if (new_node == NULL){
            return;
        }
        // set values to the node
        new_node->key = key;
        new_node->value = value;
        new_node->left = NULL;
        new_node->right = NULL;
        (*tree) = new_node;
        return;
    }

    bst_node_t *current = *tree;

    while (current != NULL){
        // if the key already exists, update the value
        if (current->key == key){
            current->value = value;
            return;
        }
        else if (current->key > key){
            if (current->left == NULL){
                bst_node_t *new = malloc(sizeof(bst_node_t));
                new->key = key;
                new->value = value;
                new->left = NULL;
                new->right = NULL;
                current->left = new;
                return;
            }
            current = current->left;
        }

        else if (current->key < key){
            if (current->right == NULL){
                bst_node_t *new = malloc(sizeof(bst_node_t));
                new->key = key;
                new->value = value;
                new->left = NULL;
                new->right = NULL;
                current->right = new;
                return;
            }
            current = current->right;
        }
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if (*tree == NULL) {
        return;
    }
    bst_node_t* tmp;
    while ((*tree)->right != NULL) {
        tree = &((*tree)->right);
    }
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    tmp = (*tree);
    (*tree) = (*tree)->left;
    free(tmp);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if ((*tree) == NULL){
        return;
    } else{
        while ((*tree) != NULL){
            // search the node which should be deleted
            if((*tree)->key > key){
                tree = &((*tree)->left);
            }
            else if((*tree)->key < key){
                tree = &((*tree)->right);
            }else{
                // node was found
                //node has no left child
                if((*tree)->left == NULL){
                    bst_node_t* temp = (*tree);
                    (*tree) = (*tree)->right;
                    free(temp);
                    //node has no right child
                }else if((*tree)->right == NULL){
                    bst_node_t* temp = (*tree);
                    (*tree) = (*tree)->left;
                    free(temp);
                    // node has no left and right children
                }else if((*tree)->left == NULL && (*tree)->right == NULL){
                    free(*tree);
                    (*tree) = NULL;
                }
                    // node has left and right children
                else{
                    bst_replace_by_rightmost((*tree), &(*tree)->left);
                }
            }
        }
    }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    if((*tree) == NULL){
        return;
    }
    // create and init the stack
    stack_bst_t* stack = (stack_bst_t*)malloc(sizeof(stack_bst_t));
    stack_bst_init(stack);
    stack_bst_push(stack, (*tree));
    while(!stack_bst_empty(stack) ){
        // pop a node from stack
        bst_node_t *current = stack_bst_pop(stack);
        if (current->right != NULL) {
            stack_bst_push(stack, current->right);
            current->right = NULL;
        }
        if (current->left != NULL) {
                stack_bst_push(stack, current->left);
                current->left = NULL;
        }
        free(current);
    }
    (*tree) = NULL;
    free(stack);
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
    while (tree != NULL){
        stack_bst_push(to_visit, tree);
        bst_add_node_to_items(tree, items);
        tree = tree->left;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    stack_bst_t stack;
    stack_bst_init(&stack);
    bst_leftmost_preorder(tree, &stack, items);
    while (!stack_bst_empty(&stack)){
        tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        bst_leftmost_preorder(tree->right, &stack, items);
    }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
   while (tree != NULL){
        stack_bst_push(to_visit, tree);
        tree = tree->left;
   }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    stack_bst_t stack;
    stack_bst_init(&stack);
    bst_leftmost_inorder(tree, &stack);
    while (!stack_bst_empty(&stack)){
        tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        bst_add_node_to_items(tree, items);
        bst_leftmost_inorder(tree->right, &stack);
    }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    while (tree != NULL){
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
   bool fromLeft;
    stack_bst_t stack;
    stack_bool_t bool_stack;
    stack_bst_init(&stack);
    stack_bool_init(&bool_stack);
    bst_leftmost_postorder(tree, &stack, &bool_stack);
    while(!stack_bst_empty(&stack)){
        tree = stack_bst_top(&stack);
        fromLeft = stack_bool_top(&bool_stack);
        stack_bool_pop(&bool_stack);
        if(fromLeft){
            stack_bool_push(&bool_stack, false);
            bst_leftmost_postorder(tree->right,&stack, &bool_stack);
        }else{
            stack_bst_pop(&stack);
            bst_add_node_to_items(tree, items);
        }
    }
}
