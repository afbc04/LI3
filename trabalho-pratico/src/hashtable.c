#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"
#include "arraydin.h"

typedef struct hashtable_item {
    void* dado;
    char* id;
    struct hashtable_item* prox;
} HT_Item;

typedef struct hashtable {
    int N;
    int max;
    HT_Item** lista;
} *HashTable;

// ##################################################################
//                      GETTERS
// ##################################################################

//Obtem o número de elementos na HashTable
int getHTN (HashTable t) {
    return t->N;
}

//Obtem a capacidade da HashTable
int getHTMax (HashTable t) {
    return t->max;
}

//Obtem o pior caso da hash table
int piorCasoHashTable(HashTable t) {
    int max=0;

    int N=t->max;

    for (int i=0;i<N;i++) {
        int num=0;
        HT_Item* aux=t->lista[i];
        while (aux) {
            num++;
            aux=aux->prox;
        }
        if (num>max) max=num;
    }

    return max;
}

//Obtem o Item da HashTable
void* getHTItem(const char* s, HashTable lista, int (*hashF)(const char*, int)) {
    HT_Item* a=procura_HashTable(s,lista,hashF);
    if (a) return a->dado;
    else return NULL;
}

// ##################################################################
//                      SETTERS
// ##################################################################

//Função que verifica se o número é primo
short NumeroPrimo(int n) {
    if (n < 2) return 0; //Se for menor que 2, não é primo
    
    //Verifica se n é divisivel por todos os numeros de 2 a raiz quadrada de n (Crivo de Eratóstenes)
    for (int i=2; i*i <= n;i++) 
        if (n%i==0) return 0; //é divisivel, logo não é primo
    
    return 1; //Não sendo encontrado um divisor, n é primo
}

//Função que obtendo um número encontra um número primo acima de 2n
int encontraNumeroPrimo(int n) {

    //Procura um numero primo entre n e n+200
    for (int i=0;i<200;i++) {
        if (NumeroPrimo(n+i)==1) return n+i; //encontrou, logo devolve esse número
    }

    return n; //Não encontrou, entao devolve o n
}

//Função que inicia a Hash Table
HashTable inicia_HashTable(int N_) {

    int N=encontraNumeroPrimo(N_);

    HashTable novo=malloc(sizeof(struct hashtable));
    novo->max=N;
    novo->N=0;
    novo->lista=malloc(sizeof(struct hashtable_item*)*N);

    for (int i=0;i<N;i++) {
        novo->lista[i]=NULL; //Inicializa tudo a NULL
    }

    return novo;
}

//Função que insere um voo na Hash Table dos Voos
void insere_HashTable(const char*s, HashTable lista,void* dado,int (*hashF)(const char*, int)) {
    //hashF -> hashFunction
    int i=hashF(s,lista->max)%(lista->max); //calcula o indice pela Função Hash

    HT_Item* ant=NULL;
    HT_Item* aux=lista->lista[i]; //Procura nos nodos do indice
    while(aux && strcmp(aux->id,s)!=0) { //a string é diferente, então avança
        ant=aux;
        aux=aux->prox;
    }

    if (aux) return; //Nodo existe, logo ele já está inserido
    else {
        aux=malloc(sizeof(struct hashtable_item));
        aux->id=strdup(s);
        aux->dado=dado;
        //aux->prox=lista->lista[i];
        //lista->lista[i]=aux;
        aux->prox=NULL;
        if (ant) ant->prox=aux; //Se o nodo anterior existe, então insere-o
        else lista->lista[i]=aux; //Se não existe, insere aux na lista

        lista->N++;
    }
}

//Função que procura o indice do voo no array dinamico usando a Hash Table e o id do voo
HT_Item* procura_HashTable(const char*s, HashTable lista,int (*hashF)(const char*, int)) {
    //hashF -> hashFunction

    int i=hashF(s,lista->max)%(lista->max); //calcula o indice pela Função Hash

    HT_Item* aux=lista->lista[i];
    while (aux) {
        if (strcmp(aux->id,s)==0) return aux;
        aux=aux->prox;
    }
    return NULL; //Nao encontrou
}

//Remove um nodo da Hash Table
void remove_HashTable (const char*s, HashTable lista,int (*hashF)(const char*, int),void (*apagaNodo)(void*)) {
    //hashF -> hashFunction
    //apagaNodo -> apaga o dado de cada nodo

    int i=hashF(s,lista->max)%(lista->max); //calcula o indice pela Função Hash
    
    HT_Item* aux=lista->lista[i];

    if (!aux) return; //O nodo está vazio
    else { //O nodo tem algo

    HT_Item* inicio=aux;
    HT_Item* ant=NULL;

    while (aux) { //Procura o nodo
        if (strcmp(aux->id,s)==0) { //O nodo foi encontrado

            if (ant==NULL) inicio=aux->prox; //O nodo está no inicio da lista
            else ant->prox=aux->prox; //O nodo está no meio ou no fim

            free(aux->id);
            apagaNodo(aux->dado);
            free(aux);
            lista->N--;
            break;
            }

        ant=aux;
        aux=aux->prox;
        }

    lista->lista[i]=inicio;
    }
}

//Função que imprime a hash table
void printHashTable(HashTable lista,void (*print)(void*)) {
    //print -> função que imprime o dado de cada nodo

    if (lista==NULL) printf("Hash Table inexistente");
    else {

    printf("Hash Table com %d slots e com %d itens\n",lista->max,lista->N);

    //Ciclo para ver todos os espaços da Hash Table
    for (int i=0;i<lista->max;i++) { 
        printf("%d. ",i);
        if (lista->lista[i]==NULL) printf("---\n"); //Espaço vazio
        else { //Existe coisas, logo dá print a toda a lista ligada
            HT_Item* aux=lista->lista[i]; 
            while(aux) {
                printf("%s (",aux->id);
                print(aux->dado);
                printf(") -> ");
                aux=aux->prox;
            }
            printf("X\n"); //Indica que a lista ligada encontrou um NULL
        }
    }
    }
    return;
}

//Função que aplica uma transformação a todos os itens da hash table
void applyHashTable(HashTable lista,void (*apply)(void*,void*),void* dado) {
    //print -> função que imprime o dado de cada nodo

    if (lista==NULL) return;
    else {

    //Ciclo para ver todos os espaços da Hash Table
    for (int i=0;i<lista->max;i++) { 
        if (lista->lista[i]!=NULL) {
            HT_Item* aux=lista->lista[i]; 
            while(aux) {
                apply(aux->dado,dado);
                aux=aux->prox;
            }
        }
    }
    }
    return;
}

//Função transforma uma Hash Table num Array Dinamico
ArrayDin* HashTable_to_AD(HashTable lista) {
    //print -> função que imprime o dado de cada nodo

    ArrayDin* novo=inicia_ArrayDin(1);

    if (lista==NULL) return novo;
    else {

    //Ciclo para ver todos os espaços da Hash Table
    for (int i=0;i<lista->max;i++) { 
        if (lista->lista[i]!=NULL) {
            HT_Item* aux=lista->lista[i]; 
            while(aux) {
                inserir_ArrayDin(strdup(aux->id),novo);
                aux=aux->prox;
            }
        }
    }
    }
    return novo;
}

//Função que apaga a Hash Table
void apagaHashTable(HashTable lista,void (*apagaNodo)(void*)) {
    //apagaNodo -> apaga o dado de cada nodo

    if (!lista) return; //HashTable não existe

    int N=lista->max;

    for (int i=0;i<N;i++) {
        HT_Item* aux=lista->lista[i];
        while (aux) {
            free(aux->id);
            apagaNodo(aux->dado);
            HT_Item* aux2=aux;
            aux=aux->prox;
            free(aux2);
        }
    }
    free(lista->lista);
    free(lista);
    return;
}