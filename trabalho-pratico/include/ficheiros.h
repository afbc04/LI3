#ifndef FICHEIROS
#define FICHEIROS

#include "arraydin.h"

typedef struct ficheiro Ficheiro;

//Getters
int LinhasFicheiro(Ficheiro* fich);
void escreveLinha(Ficheiro* fich,const char* string);
char* lerLinha(Ficheiro* fich);

//Setters
Ficheiro* iniciaFicheiro(const char* nome,const char* modo);
void apagaFicheiro(Ficheiro* fich);
void rewindFicheiro(Ficheiro* fich);
int escreveFicheiro(const char* nome_ficheiro, ArrayDin* lista);

int comparaFich(char* nome1, char* nome2, int c);

#endif