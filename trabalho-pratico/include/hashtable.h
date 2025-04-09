#ifndef HASHTABLE
#define HASHTABLE

#include "arraydin.h"

//Função para a HashTable

typedef struct hashtable_item HT_Item;
typedef struct hashtable *HashTable;

//Getters
int getHTN (HashTable t);
int getHTMax (HashTable t);
int piorCasoHashTable(HashTable t);
void* getHTItem(const char* s, HashTable lista, int (*hashF)(const char*, int)); 

//Setters
HashTable inicia_HashTable(int N_);
void insere_HashTable(const char*s, HashTable lista,void* dado,int (*hashF)(const char*, int));
HT_Item* procura_HashTable(const char*s, HashTable lista,int (*hashF)(const char*, int));
void remove_HashTable (const char*s, HashTable lista,int (*hashF)(const char*, int),void (*apagaNodo)(void*));
void printHashTable(HashTable lista,void (*print)(void*));
void apagaHashTable(HashTable lista,void (*apagaNodo)(void*));
void applyHashTable(HashTable lista,void (*apply)(void*,void*),void* a);
ArrayDin* HashTable_to_AD(HashTable lista);

#endif