#ifndef ARRAYDIN
#define ARRAYDIN

//Função para array dinâmico

typedef struct array_din ArrayDin;

//Getters
int getADMax(const ArrayDin* lista);
int getADN(const ArrayDin* lista);
void* getADItem(const ArrayDin* lista, int i);

//Setters
void inserir_ArrayDin(void* dado, ArrayDin* lista);
void printArrayDin(const ArrayDin* lista,void(*print)(void*));
ArrayDin* inicia_ArrayDin(int N);
void apaga_ArrayDin(ArrayDin* lista,void(*apagaF)(void*));
void apaga_ADStrings(ArrayDin* lista);
void quickSort(ArrayDin* lista, int ini, int fim, short(*compara)(void*,void*));

//void remove_ArrayDin(int ind, ArrayDin* lista,void(*apagaF)(void*));
//void comprimir_ArrayDin(ArrayDin* lista);

#endif