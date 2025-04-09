#ifndef VOOS_FUNC
#define VOOS_FUNC

#include "arraydin.h"
#include "hashtable.h"
#include <stdbool.h>

#include "users.h"

typedef struct voos Voos_Dados;

//Getters
int getVooExiste(Voos_Dados* HT, const char* s);
char* getVooAirline(Voos_Dados* HT, const char* s);
char* getVooPlaneModel(Voos_Dados* HT, const char* s);
char* getVooOrigin(Voos_Dados* HT, const char* s);
char* getVooDestination(Voos_Dados* HT, const char* s);
char* getVooDeparture(Voos_Dados* HT, const char* s);
char* getVooArrival(Voos_Dados* HT, const char* s);
int getVooNPassengers(Voos_Dados* HT, const char* s);
int getVooMaxPassengers(Voos_Dados* HT, const char* s);
int getVooDelay(Voos_Dados* HT, const char* s);
char* getVooId(Voos_Dados* HT, const char* s);
int getVooToAero(Voos_Dados* dados, const char* voo_, const char* aero);
int getVooEntreDatas(Voos_Dados* HT, const char* s, const char* inicio, const char* fim);
ArrayDin* getVooLista(Voos_Dados* HT);

int getAeroExiste(Voos_Dados* HT, const char* s);
char* getAeroNome(Voos_Dados* HT, const char* s);
int getAeroMediana(Voos_Dados* HT, const char* s);
ArrayDin* getAeroLista(Voos_Dados* HT);
int getAeroPassageirosAno(Voos_Dados* dados,const char* aero_, short ano);
ArrayDin* getAeroVoos(Voos_Dados* HT, const char* s);

int getVooDadosResultado(Voos_Dados* lista);
int getVooDadosLidos(Voos_Dados* lista);
int getVooDadosSucc(Voos_Dados* lista);
int getVooDadosFail(Voos_Dados* lista);
int getPassengersDadosResultado(Voos_Dados* lista);
int getPassengersDadosLidos(Voos_Dados* lista);
int getPassengersDadosSucc(Voos_Dados* lista);
int getPassengersDadosFail(Voos_Dados* lista);

//Setters
void ordenaVoosAeroporto(const char* aero_, Voos_Dados* Aero);
void calcularMedianasAeroportos(Voos_Dados* Voo);
ArrayDin* ordenaVoos(ArrayDin* lista,Voos_Dados* Voo);

//Funções
void preenchePassageirosUnicos(User_Dados* Users, Voos_Dados* Voo, const char* nome);
Voos_Dados* iniciaVoos(const char* caminho_);
void iniciaPassageiros(const char* caminho_,Voos_Dados* Voo,User_Dados* Users);
void insereVoosAeroportos(Voos_Dados* Aero);
void apagaListaVoosAero(Voos_Dados* dados);

//Auxiliares
void printVoo(void* a);
void printAero(void* a);

#endif