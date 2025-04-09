#ifndef RESERVAS_FUNC
#define RESERVAS_FUNC

#include "hashtable.h"
#include "arraydin.h"

#include "users.h"

typedef struct reservas Reservas_Dados;

//Getters
int getReservaExiste(Reservas_Dados* HT, const char* s);
char* getReservaID(Reservas_Dados* HT, const char* s);
char* getReservaHotelID(Reservas_Dados* HT, const char* s);
char* getReservaBeginDate(Reservas_Dados* HT, const char* s);
char* getReservaEndDate(Reservas_Dados* HT, const char* s);
double getReservaPricePerNight(Reservas_Dados* HT, const char* s);
char* getReservaIncludesBreakfast(Reservas_Dados* HT, const char* s);
char* getReservaUserID(Reservas_Dados* HT, const char* s);
int getReservaCityTax(Reservas_Dados* HT, const char* s);
int getReservaNights(Reservas_Dados* HT, const char* s);
char* getReservaHotelName(Reservas_Dados* HT, const char* s);
int getReservaHotelStars(Reservas_Dados* HT, const char* s);
int getReservaRating(Reservas_Dados* HT, const char* s);
double getReservaTotalPrice(Reservas_Dados* HT, const char* s);
int getReservaNightPriceDates(Reservas_Dados* dados, const char* s, const char* inicio, const char* fim);
ArrayDin* getReservaLista(Reservas_Dados* HT);

int getHotelExiste(Reservas_Dados* HT, const char* s);
char* getHotelName(Reservas_Dados* HT, const char* s);
char* getHotelId(Reservas_Dados* HT, const char* s);
int getHotelStars(Reservas_Dados* HT, const char* s);
double getHotelRating(Reservas_Dados* HT, const char* s);
ArrayDin* getHotelReservas(Reservas_Dados* HT, const char* s);

int getReservaDadosResultado(Reservas_Dados* lista);
int getReservaDadosLidos(Reservas_Dados* lista);
int getReservaDadosSucc(Reservas_Dados* lista);
int getReservaDadosFail(Reservas_Dados* lista);

//Setters
void insereReservaHoteis(const char* hotel_, const char* id, Reservas_Dados* HTH);
void ordenaReservasHotel(const char* hotel_, Reservas_Dados* HTH);
ArrayDin* ordenaReservas(const ArrayDin* lista,Reservas_Dados* Reservas);

//Funções
Reservas_Dados* iniciaReservas(const char* caminho_,User_Dados* Users);
void apagaListaReservasHoteis(Reservas_Dados* dados);

//Auxiliares
void printReservas(void* a);
void printHotel(void* a);

#endif