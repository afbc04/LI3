#include <stdio.h>
#include <stdlib.h>
#include "arraydin.h"

typedef struct array_din {
    int N; //Numero de elementos
    int max; //Capacidade máxima do array
    void** lista; //Lista
} ArrayDin;

// ##################################################################
//                      GETTERS
// ##################################################################

//Obtem o tamanho máximo do array din
int getADMax(const ArrayDin* lista) {
    if (!lista) return 0;
    return lista->max;
}

//Obtem o número de elementos do array din
int getADN(const ArrayDin* lista) {
    if (!lista) return 0;
    return lista->N;
}

//Obter o item de índice i
void* getADItem(const ArrayDin* lista, int i) {
    if (i<0 || i>=lista->N) return NULL;
    else return lista->lista[i];
}

// ##################################################################
//                      SETTERS
// ##################################################################

//Inserção no fim do Array Dinamico
void inserir_ArrayDin(void* dado, ArrayDin* lista) {

    if (lista->N>=lista->max) { //Aumento do tamanho do array
    int novo_max=(lista->max)*2; //Aumenta para o dobro
    void** nova_lista=malloc(sizeof(void*)*novo_max);

    int N2=lista->N;
    for (int i=0;i<N2;i++) { //Copiar todos os elementos do array antigo para o novo
        nova_lista[i]=lista->lista[i];
    }
    free(lista->lista); //Dar free ao array antigo
    lista->lista=nova_lista; //Colocar o novo array
    lista->max=novo_max; //definir um novo máximo
    }

    //Inserção do user
    lista->lista[lista->N]=dado;
    lista->N++;
}

//Remove um elemento de índice i do array dinamico
void remove_ArrayDin(int ind, ArrayDin* lista,void(*apagaF)(void*)) {
    if (ind<0 || ind>=lista->N) return;
    if (lista->lista[ind]) apagaF(lista->lista[ind]);
    lista->lista[ind]=NULL;
    return;
}

void comprimir_ArrayDin(ArrayDin* lista) {
    //Função que faz com que a lista nao tenha nenhum espaço livre

    int N=lista->N;
    int N_novo=0;

    for (int i=0;i<N;i++) {
        if (lista->lista[i]!=NULL) N_novo++;
    }

    void** nova_lista=malloc(sizeof(void*)*N_novo);
    int j=0;
    for (int i=0;i<N;i++) { //Copiar todos os elementos do array antigo para o novo
        if (lista->lista[i]==NULL) continue;
        nova_lista[j]=lista->lista[i];
        j++;
    }
    free(lista->lista); //Dar free ao array antigo
    lista->lista=nova_lista; //Colocar o novo array
    lista->max=N_novo; //definir um novo máximo
    lista->N=N_novo;
}

//Função que imprime o array dinamico
void printArrayDin(const ArrayDin* lista,void(*print)(void*)) {
    int N=lista->N;
    for (int i=0;i<N;i++) {
        if (lista->lista[i]!=NULL) {
            printf("%d ",i);
            print(lista->lista[i]);
            printf("\n");
        }
        else printf("*\n");
    }
    return;
}

//Função que inicia um array dinamico
ArrayDin* inicia_ArrayDin(int N) {
    ArrayDin* novo=malloc(sizeof(ArrayDin));
    novo->max=N;
    novo->N=0;
    novo->lista=malloc(sizeof(void*)*N);
    return novo;
}

//Função que apaga o array dinamico
void apaga_ArrayDin(ArrayDin* lista,void(*apagaF)(void*)) {
    int N=lista->N;
    for (int i=0;i<N;i++) {
        if (lista->lista[i]!=NULL) apagaF(lista->lista[i]);
    }
    free(lista->lista);
    free(lista);
    return;
}

void apaga_ADString(void* a_) {
    char* a=a_;
    free(a);
}

//Apaga Array dinamicos de strings
void apaga_ADStrings(ArrayDin* lista) {
    apaga_ArrayDin(lista,&apaga_ADString);
}

//Função auxiliar para a quicksort do array dinamico de Users
int quickSort_aux(ArrayDin* lista, int ini, int fim, short(*compara)(void*,void*)) {
  
  void* pivo = lista->lista[fim]; //pivo (seleciona o elemento mais á direita)

  int i=ini-1; //aponta para o maior elemento

  for (int j = ini; j < fim; j++) {
    if (compara(lista->lista[j],pivo)<1) {
        //compara: troca se o elemento j for igual ou menor que o pivo
        i++;
      void* temp=lista->lista[i]; //Efetua a troca entre elementos
      lista->lista[i]=lista->lista[j];
      lista->lista[j]=temp;
    }
  }

    //Efetua a troca do pivo com o maior elemento
      void* temp=lista->lista[i+1];
      lista->lista[i+1]=lista->lista[fim];
      lista->lista[fim]=temp;
      
  return i+1;
}

//Função que ordena o array dinamico (quick sort)
void quickSort(ArrayDin* lista, int ini, int fim, short(*compara)(void*,void*)) {

  if (ini>=fim) return; //Nao ordenamos mais

    int pivo = quickSort_aux(lista,ini,fim,compara); //dá o pivo

        quickSort(lista,ini,pivo-1,compara); //Faz a parte da esquerda
        quickSort(lista,pivo+1,fim,compara); //Faz a parte da direita

    return;
}
