/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if(tree == NULL){
        return false;
    }else {
        // if the key was found, update the value
        if (tree->key == key) {
            *value = tree->value;
            return true;
            // if the key is greater, search in right subtree
        } else if (tree->key < key) {
            return bst_search(tree->right, key, value);
            // if the key is smaller, search in left subtree
        }else {
            return bst_search(tree->left, key, value);
        }
    }
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if((*tree) == NULL){
        // create the new node
        bst_node_t* newNodePtr = (bst_node_t*) malloc(sizeof(bst_node_t));
        if(newNodePtr == NULL){
            return;
        }
        // set values to the node
        newNodePtr->key = key;
        newNodePtr->value = value;
        newNodePtr->left = NULL;
        newNodePtr->right = NULL;
       (*tree) = newNodePtr;
        return;
        // If the key is smaller than the current node's key, insert in the left subtree
    }else if ((*tree)->key > key){
        bst_insert(&(*tree)->left, key, value);
        // If the key is greater than the current node's key, insert in the right subtree
    }else if((*tree)->key < key){
        bst_insert(&(*tree)->right, key, value);
        // if the key already exists, update the value
    }else{
        (*tree)->value = value;
    }
}



/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if((*tree)->right != NULL){
        bst_replace_by_rightmost(target,&(*tree)->right);
    }else{
        target->key = (*tree)->key;
        target->value = (*tree)->value;
        bst_node_t *tmp = (*tree);
        (*tree) = (*tree)->left;
        free(tmp);
    }
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if((*tree) == NULL){
        return;
    } else{
        // search the node which should be deleted
        if((*tree)->key < key){
            bst_delete(&(*tree)->right, key);
        } else if((*tree)->key > key){
            bst_delete(&(*tree)->left, key);
        } else{
            // node was found
            //node has no left child
            if((*tree)->left == NULL){
                bst_node_t* temp = (*tree);
                (*tree) = (*tree)->right;
                free(temp);
                //node has no right child
            } else if((*tree)->right == NULL){
                bst_node_t* temp = (*tree);
                (*tree) = (*tree)->left;
                free(temp);
                // node has no left and right children
            } else if((*tree)->left == NULL && (*tree)->right == NULL){
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

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    if((*tree) == NULL){
        return;
    } else{
        bst_dispose(&(*tree)->left);
        bst_dispose(&(*tree)->right);
        free(*tree);
        (*tree) = NULL;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    if(tree != NULL){
        bst_add_node_to_items(tree,items);
        bst_preorder(tree->left, items);
        bst_preorder(tree->right, items);
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    if(tree != NULL){
        bst_inorder(tree->left, items);
        bst_add_node_to_items(tree,items);
        bst_inorder(tree->right, items);
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    if(tree != NULL){
        bst_postorder(tree->left, items);
        bst_postorder(tree->right, items);
        bst_add_node_to_items(tree,items);
    }
}
