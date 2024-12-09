/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
    int i = 0;
    while(i < HT_SIZE){
        (*table)[i] = NULL;
        i++;
    }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    ht_item_t* current =  (*table)[get_hash(key)];
    while(current != NULL){
        if(strcmp(current->key, key) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    ht_item_t* existing_item = ht_search(table, key);
    // if item does not exist
    if(existing_item  == NULL){
        // create a new item
        ht_item_t* new_item = malloc(sizeof(ht_item_t));
        // set the values to the new item
        new_item->next = (*table)[get_hash(key)];
        new_item->key = key;
        new_item->value = value;
        (*table)[get_hash(key)] = new_item;
        // if the item exists, update the value
    }else{
        existing_item->value = value;
    }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    ht_item_t *existing_item = ht_search(table, key);
    return (existing_item != NULL) ? &(existing_item->value) : NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    if(table == NULL || key == NULL){
        return;
    }
    ht_item_t *current = (*table)[get_hash(key)];
    ht_item_t *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                (*table)[get_hash(key)] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }

}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
    int i = 0;
    while (i < HT_SIZE){
        ht_item_t* current_item = (*table)[i];
        while (current_item != NULL){
            ht_item_t* next_item  = current_item->next;
            free(current_item);
            current_item = next_item;
        }
        (*table)[i] = NULL;
        i++;
    }
}
