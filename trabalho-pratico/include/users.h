#ifndef USERS
#define USERS

#include "arraydin.h"
#include "hashtable.h"

typedef struct users User_Dados;

//Getters
int UserAtivo(User_Dados* HT, const char* s);
char* getUserId(User_Dados* HT, const char* s);
char* getUserName(User_Dados* HT, const char* s);
char* getUserSex(User_Dados* HT, const char* s);
int getUserAge(User_Dados* HT, const char* s);
char* getUserCountryCode(User_Dados* HT, const char* s);
char* getUserPassport(User_Dados* HT, const char* s);
char* getUserStatus(User_Dados* HT, const char* s);
char* getUserCreation(User_Dados* HT, const char* s);
double getUserTotalSpent(User_Dados* HT, const char* s);
int getUserExiste(User_Dados* HT, const char* s);
int getUserNumeroReservas(User_Dados* HT, const char* s);
int getUserNumeroVoos(User_Dados* HT, const char* s);
ArrayDin* getUserReservas(User_Dados* HT, const char* s);
ArrayDin* getUserVoos(User_Dados* HT, const char* s);
ArrayDin* getUserLista(User_Dados* HT);
int getUsersUniquePassengers(User_Dados* dados);

int getUserDadosResultado(User_Dados* lista);
int getUserDadosLidos(User_Dados* lista);
int getUserDadosSucc(User_Dados* lista);
int getUserDadosFail(User_Dados* lista);

//Setters
void insereReservaUsers(const char* user_, const char* id, User_Dados* HTU);
void insereVooUsers(const char* user_, const char* id, User_Dados* HTU);
void addTotalSpentUser(const char* user_,User_Dados* HTU, double valor);
void printUsers(User_Dados* User, short modo);
void resetUsersUniquePassengers(User_Dados* dados);
int setUserUniquePassenger(User_Dados* dados, const char* nome);


//Funções
User_Dados* iniciaUsers(const char* caminho_);
void apagaDadosUsers(User_Dados* lista);

#endif