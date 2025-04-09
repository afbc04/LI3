#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "users.h"
#include "reservas.h"
#include <string.h>
#include <ctype.h>

#include "arraydin.h"
#include "hashtable.h"
#include "datas.h"

#include "ficheiros.h"

typedef struct pre_dados_reservations 
{
    char* idRserv;
    char* user_id;
    char* hotel_id;
    char* hotel_name;
    char* hotel_stars;
    char* city_tax;
    char* address;
    char* begin_date;
    char* end_date;
    char* price_per_night;
    char* includes_breakfast;
    char* room_details;
    char* rating;
    char* comment;
} Pre_Reservas;

typedef struct dados_reservas {
    char* id; //Id da reserva
    char* hotel_id; //id do hotel da reserva
    char* begin_date; //data do inicio da reserva
    char* end_date; //data do fim da reserva
    short price_per_night; //preço por noite da reserva
    bool includes_breakfast; //indice se inclui pequeno-almoço
    char* user_id; //id do user que efetuou a reserva
    char city_tax; //Taxas da cidade (usado char pois os valores são muito pequenos)
    char number_of_nights; //Numero de noites da reserva (uma reserva não dura mais que 200 dias, então podemos usar char)
    char* hotel_name; //nome do hotel
    char hotel_stars; //estrelas do hotel
    char rating; //avaliação da reserva
} Reservations;

typedef struct dados_hotel {
    char* hotel_name; //Nome do hotel
    char* hotel_id; //Id do hotel
    char hotel_stars; //estrelas do hotel
    ArrayDin* reservations; //lista das reservas do hotel
    bool reservas_ordenadas; //indica se as reservas do hotel estão ordenadas
    double rating;
    bool rating_calculada;
} Hotel;

//Struct com todas as reservas e hoteis guardados
typedef struct reservas {
    HashTable reservas; //Hash Table das Reservas
    HashTable hoteis; //Hash Table dos Hoteis
    char resultado; //Indica o resultado da leitura
    int lidos; //Indica quantos dados foram lidos
    int succ; //Indica quantos dados foram incorporados
    int fail; //Indica quantos dados foram rejeitados
} Reservas_Dados;


double calculaPrecoReserva(char noites, char city_tax, short price_per_night);

// ##################################################################
//                      GETTERS
// ##################################################################

//Função que indica qual indice procurar. Mais conhecida por Hash Function
int hashReservas(const char*s, int Max) {

    int N=strlen(s);
    char* id=malloc(N-3);
    for (int i=0;i<N-3;i++) id[i]=s[i+4];

    id[N-4]='\0';

    int A=atoi(id);
    free(id);
    return (A%Max);
}

//Função que indica se uma reserva existe
int getReservaExiste(Reservas_Dados* HT, const char* s) {
    void* reserva=procura_HashTable(s,HT->reservas,&hashReservas);
    if (reserva) return 1;
    else return 0;
}

//Obtem Id da Reserva
char* getReservaID(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return strdup(reserva->id);
}

//Obtem Hotel Id da Reserva
char* getReservaHotelID(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return strdup(reserva->hotel_id);
}

//Obtem Begin Date da Reserva
char* getReservaBeginDate(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return strdup(reserva->begin_date);
}

//Obtem End Date da Reserva
char* getReservaEndDate(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return strdup(reserva->end_date);
}

//Obtem Preço por Noite da Reserva
double getReservaPricePerNight(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return (double) reserva->price_per_night;
}

//Obtem Inclui Pequeno-Almoço da Reserva
char* getReservaIncludesBreakfast(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    if (reserva->includes_breakfast==false) return strdup("False");
    else return strdup("True");
}

//Obtem User Id da Reserva
char* getReservaUserID(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return strdup(reserva->user_id);
}

//Obtem City Tax da Reserva
int getReservaCityTax(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return (int) reserva->city_tax;
}

//Obtem Numero de Noites da Reserva
int getReservaNights(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return (int) reserva->number_of_nights;
}

//Obtem Hotel Name da Reserva
char* getReservaHotelName(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return strdup(reserva->hotel_name);
}

//Obtem Hotel Stars da Reserva
int getReservaHotelStars(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return (int) reserva->hotel_stars;
}

//Obtem Rating da Reserva
int getReservaRating(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return (int) reserva->rating;
}

//Obtem Total Price da Reserva
double getReservaTotalPrice(Reservas_Dados* HT, const char* s) {
    Reservations* reserva= getHTItem(s,HT->reservas,&hashReservas);
    return calculaPrecoReserva(reserva->number_of_nights,reserva->city_tax,reserva->price_per_night);
}

//Obtem o preço de noites considerando uma data
int getReservaNightPriceDates(Reservas_Dados* dados, const char* s, const char* inicio, const char* fim) {
    Reservations* reserva= getHTItem(s,dados->reservas,&hashReservas);
    int noites=getNoitesDatas(reserva->begin_date,reserva->end_date,inicio,fim);
    return (noites*(reserva->price_per_night));
}

//Obtem Array dinamico de todas as reservas
ArrayDin* getReservaLista(Reservas_Dados* HT) {

    return HashTable_to_AD(HT->reservas);
}


//Função que indica qual indice procurar. Mais conhecida por Hash Function
int hashHotel(const char*s, int Max) {
    int N=strlen(s);
    char* id=malloc(N-2);
    for (int i=0;i<N-2;i++) id[i]=s[i+3];

    id[N-3]='\0';

    int A=atoi(id);
    free(id);
    return (A%Max);
}

//Função que indica se um hotel existe
int getHotelExiste(Reservas_Dados* HT, const char* s) {
    void* hotel=procura_HashTable(s,HT->hoteis,&hashHotel);
    if (hotel) return 1;
    else return 0;
}

//Obtem Nome do Hotel
char* getHotelName(Reservas_Dados* HT, const char* s) {
    Hotel* hotel= getHTItem(s,HT->hoteis,&hashHotel);
    return strdup(hotel->hotel_name);
}

//Obtem Id do Hotel
char* getHotelId(Reservas_Dados* HT, const char* s) {
    Hotel* hotel= getHTItem(s,HT->hoteis,&hashHotel);
    return strdup(hotel->hotel_id);
}

//Obtem Stars do Hotel
int getHotelStars(Reservas_Dados* HT, const char* s) {
    Hotel* hotel= getHTItem(s,HT->hoteis,&hashHotel);
    return (int) hotel->hotel_stars;
}

void apagaDoubleGHR(void* a_) {
    double* a=a_;
    free(a);
}

//Obtem o Rating de um hotel
double getHotelRating(Reservas_Dados* HT, const char* s) {

    if (getHotelExiste(HT,s)==0) return -1;

    Hotel* hotel= getHTItem(s,HT->hoteis,&hashHotel);

    if (hotel->rating_calculada==true) return hotel->rating; //Dá o rating
    else { //O rating nao está calculado

    hotel->rating=hotel->rating/getADN(hotel->reservations);
    hotel->rating_calculada=true;
    return hotel->rating;

    }

}

//Obtem Reservas do Hotel
ArrayDin* getHotelReservas(Reservas_Dados* HT, const char* s) {
    Hotel* hotel= getHTItem(s,HT->hoteis,&hashHotel);
    
    ArrayDin* reservas=inicia_ArrayDin(1);

    int N=getADN(hotel->reservations);

    for (int i=0;i<N;i++) {
        inserir_ArrayDin(strdup(getADItem(hotel->reservations,i)),reservas);
    }

    return reservas;
}


//Obter o resultado do Reserva Dados
int getReservaDadosResultado(Reservas_Dados* lista) {
    return lista->resultado;
}

//Obter os dados lidos do Reserva Dados
int getReservaDadosLidos(Reservas_Dados* lista) {
    return lista->lidos;
}

//Obter os dados incorporados do Reserva Dados
int getReservaDadosSucc(Reservas_Dados* lista) {
    return lista->succ;
}

//Obter os dados rejeitados do Reserva Dados
int getReservaDadosFail(Reservas_Dados* lista) {
    return lista->fail;
}

// ##################################################################
//                      SETTERS
// ##################################################################

//Função que insere uma reserva a um hotel
void insereReservaHoteis(const char* hotel_, const char* id, Reservas_Dados* HTH) {

    if(getHotelExiste(HTH,hotel_)==0) return; //Hotel não existe

    Hotel* hotel=getHTItem(hotel_,HTH->hoteis,&hashHotel);
    char* reserva_id=strdup(id);
    inserir_ArrayDin(reserva_id,hotel->reservations);
}

//Função que ordena as reservas de um hotel
void ordenaReservasHotel(const char* hotel_, Reservas_Dados* HTH) {

    if(getHotelExiste(HTH,hotel_)==0) return; //Hotel não existe

    Hotel* hotel= getHTItem(hotel_,HTH->hoteis,&hashHotel);

    if (hotel->reservas_ordenadas==true) return;

    ArrayDin* novas_reservas=ordenaReservas(hotel->reservations,HTH);
    apaga_ADStrings(hotel->reservations);
    hotel->reservations=novas_reservas;
    hotel->reservas_ordenadas=true;
    return;
}


// ##################################################################
//                      FUNÇÕES
// ##################################################################

//Struct auxiliar para a ordenação de reservas
struct ordena_reservas {
    char* id;
    char* dia;
};

//Função que serve para comparar Reservas e util na ordenação de reservas
short ordenaReservasAux(void* a_, void* b_) {

    struct ordena_reservas* a=a_;
    struct ordena_reservas* b=b_;

    short res=(-1)*comparaDatas(a->dia,b->dia);
    if (res==0) res=strcmp(a->id,b->id);

    return res;
}

//Função auxiliar que apaga a lista de reservas
void ordenaReservaApaga(void* a_) {
    struct ordena_reservas* a=a_;
    free(a->id);
    free(a->dia);
    free(a);
    return;
}

//Função que ordena uma lista de reservas
ArrayDin* ordenaReservas(const ArrayDin* lista,Reservas_Dados* Reservas) {

    int N=getADN(lista);
    ArrayDin* aux=inicia_ArrayDin(N);
    for (int i=0;i<N;i++) {
        struct ordena_reservas* novo=malloc(sizeof(struct ordena_reservas));
        Reservations* reserva=getHTItem(getADItem(lista,i),Reservas->reservas,&hashReservas);
        novo->id=strdup(reserva->id);
        novo->dia=strdup(reserva->begin_date);
        inserir_ArrayDin(novo,aux);
    }
    quickSort(aux,0,N-1,&ordenaReservasAux);

    ArrayDin* lista2=inicia_ArrayDin(N);
    for (int i=0;i<N;i++) {
        struct ordena_reservas* reserva=getADItem(aux,i);
        char* reserva_id=strdup(reserva->id);
        inserir_ArrayDin(reserva_id,lista2);
    }

    apaga_ArrayDin(aux,&ordenaReservaApaga);
    return lista2;
}


//Validação das Reservas
int reservation_validation(Pre_Reservas reservation,User_Dados* Users){


    if (reservation.idRserv==NULL) return 0;
    if (reservation.user_id==NULL) return 0;
    if (reservation.hotel_id==NULL) return 0;
    if (reservation.hotel_name==NULL) return 0;
    if (reservation.hotel_stars==NULL) return 0;
    if (reservation.city_tax==NULL) return 0;
    if (reservation.address==NULL) return 0;
    if (reservation.begin_date==NULL) return 0;
    if (reservation.end_date==NULL) return 0;
    if (reservation.price_per_night==NULL) return 0;
    if (reservation.includes_breakfast==NULL) return 0;
    if (reservation.room_details==NULL) return 0;
    if (reservation.rating==NULL) return 0;
    if (reservation.comment==NULL) return 0;


    int existe_user=getUserExiste(Users,reservation.user_id);
    if (existe_user==0) return 0;

    //Verificação de datas
    if(dia_validation(reservation.begin_date)==0) return 0;
    if(dia_validation(reservation.end_date)==0) return 0;

    //begin_date tem que vir antes do end_date
    short comparaDiasRes=comparaDatas(reservation.begin_date,reservation.end_date);
    if (comparaDiasRes==1) return 0;

    //O número de estrelas de um hotel (hotel_stars) tem que ser um valor inteiro entre 1 e 5, inclusive. Exemplos de erros: 0,-3,1.4,...;
    //Verifica se é um valor inteiro
    for(int i=0; i<strlen(reservation.hotel_stars);i++){
        if(isdigit(reservation.hotel_stars[i])==0) return 0;
    }
    //Verifica se está entre 1 e 5
    int ht_int = atoi(reservation.hotel_stars);
    if(ht_int>5 || ht_int<1) return 0;

    //A percentagem de imposto da cidade de uma reserva (city_tax) tem que ser um valor inteiro maior ou igual a zero;
    //Verifica se é um valor inteiro
    for(int i=0; i<strlen(reservation.city_tax);i++){
        if(isdigit(reservation.city_tax[i])==0) return 0;
    }
    //Verifica se é maior ou igual a 0
    int ct_int = atoi(reservation.city_tax);
    if(ct_int<0) return 0;

    //O preço por noite de uma reserva (price_per_night) tem que ser um valor inteiro maior que 0
    //Verifica se é um valor inteiro
    for(int i=0; i<strlen(reservation.price_per_night);i++){
        if(isdigit(reservation.price_per_night[i])==0) return 0;
    }
    //Verifica se é maior que 0
    int ppn_int = atoi(reservation.price_per_night);
    if(ppn_int<=0) return 0;

    //includes_breafast de uma reserva pode ser (com diferentes combinações de maiúsculas e minúsculas): Para valores falsos, “f ”, “false”, “0”, e “” (string vazia); Para valores verdadeiros, “t”, “true”, e “1”.
    if(strcasecmp(reservation.includes_breakfast,"f")!=0 && strcasecmp(reservation.includes_breakfast,"false")!=0 && strcasecmp(reservation.includes_breakfast,"0")!=0 && strlen(reservation.includes_breakfast)>0 && strcasecmp(reservation.includes_breakfast,"t")!=0 && strcasecmp(reservation.includes_breakfast,"true")!=0 && strcasecmp(reservation.includes_breakfast,"1")!=0) return 0;

    //As classificações de uma reserva (rating) têm que ter um valor inteiro entre 1 e 5, inclusive.
    for(int i=0; i<strlen(reservation.rating);i++){
        if(isdigit(reservation.rating[i])==0 ) return 0;
    }
    //Verifica se está entre 1 e 5
    int rate_int = atoi(reservation.rating);
    if(rate_int>5 || rate_int<1) return 0;

    //Os seguintes restantes campos têm que ter tamanho superior a zero:
    //Reserva: id, user_id, hotel_id, hotel_name, address.
    if(strlen(reservation.idRserv)==0 || strlen(reservation.user_id)==0 || strlen(reservation.hotel_id)==0 || strlen(reservation.hotel_name)==0 || strlen(reservation.address)==0) return 0;


    return 1;
}

//Função que calcula o preço de uma reserva
double calculaPrecoReserva(char noites, char city_tax, short price_per_night) {
    // noites = numero de noites passadas
    // city_tax e price_per_night = parametros da reserva

    double  N=(noites)*price_per_night;
    double total=N+(N/100)*city_tax; //Calculo do preço

    return total;
}

//Função que indica a partir de uma string se o pequeno-almoço é incluido
bool processaIncludeBreakfast(const char* string) {

    if (strcasecmp(string,"FALSE")==0 || strcasecmp(string,"F")==0) return false;
    if (strcasecmp(string,"TRUE")==0 || strcasecmp(string,"T")==0) return true;
    return false;
}

//Função que cria uma reserva com a informação recolhida pelo parser
Reservations* PreReserva_para_Reserva(Pre_Reservas dados) {

    Reservations* nova_reserva=malloc(sizeof(struct dados_reservas));

    nova_reserva->id=strdup(dados.idRserv);
    nova_reserva->hotel_id=strdup(dados.hotel_id);
    nova_reserva->begin_date=strdup(dados.begin_date);
    nova_reserva->end_date=strdup(dados.end_date);

    nova_reserva->price_per_night=atoi(dados.price_per_night);

    nova_reserva->includes_breakfast=processaIncludeBreakfast(dados.includes_breakfast);

    nova_reserva->user_id=strdup(dados.user_id);
    nova_reserva->city_tax=atoi(dados.city_tax);

    nova_reserva->number_of_nights=calculaDiasDatas(dados.begin_date,dados.end_date);

    nova_reserva->hotel_name=strdup(dados.hotel_name);
    nova_reserva->hotel_stars=atoi(dados.hotel_stars);
    nova_reserva->rating=atoi(dados.rating);

    return nova_reserva;
}


//Função que cria um Hotel
Hotel* criaHotel(Reservations* dados) {

    Hotel* novo=malloc(sizeof(struct dados_hotel)); //criar nodo

    novo->hotel_id=strdup(dados->hotel_id);
    novo->hotel_name=strdup(dados->hotel_name);
    novo->hotel_stars=dados->hotel_stars;
    novo->reservations=inicia_ArrayDin(1);
    novo->reservas_ordenadas=false;
    novo->rating=0;
    novo->rating_calculada=false;

    return novo;
}

//Aumenta o rating de um hotel
void addRatingHotel(const char* hotel_,Reservas_Dados* HTH, double valor) {

    Hotel* hotel=getHTItem(hotel_,HTH->hoteis,&hashHotel);
    hotel->rating+=valor;
    return;
}

//Função que coleta todos os dados das reservas, processa-os e preenche a hash table de reservas e hoteis
Reservas_Dados* iniciaReservas(const char* caminho_,User_Dados* Users) {
//   Result:  0 -> nao há reservas  |  1 -> ha reservas da pasta caminho   |  2 -> ha reservas da pasta alternativa

    Reservas_Dados* dados=malloc(sizeof(Reservas_Dados));
    dados->lidos=0;
    dados->fail=0;
    dados->succ=0;

    if (!caminho_) {//Caminho inválido
        dados->resultado=0;
        dados->reservas=inicia_HashTable(1);
        dados->hoteis=inicia_HashTable(1);
        return dados;
    }

    dados->resultado=1;

    // ################ Parser ##############

    char caminho[1000]; //cria caminho
    sprintf(caminho,"%s/reservations.csv",caminho_);

    Ficheiro* ficheiro=iniciaFicheiro(caminho,"r");
    Ficheiro* erros=iniciaFicheiro("Resultados/reservations_errors.csv","w+");
    
    if (!erros) {
        apagaFicheiro(ficheiro);
        dados->resultado=0;
        dados->reservas=inicia_HashTable(1);
        dados->hoteis=inicia_HashTable(1);
        return dados;
    }

    if (ficheiro==NULL) {
        ficheiro=iniciaFicheiro("reservations.csv","r");  //Nao encontrou, procura na pasta default
        if (ficheiro) dados->resultado=2;
    }
    
    if (ficheiro==NULL) { //Dados nao encontrados
        apagaFicheiro(erros);
        dados->resultado=0;
        dados->reservas=inicia_HashTable(1);
        dados->hoteis=inicia_HashTable(1);
        return dados;
    }

    // ################ Incorporação das Reservas ####################


    int N_ficheiro=LinhasFicheiro(ficheiro);

    dados->reservas=inicia_HashTable(N_ficheiro-1); //inicia a hash table de reservas
    dados->hoteis=inicia_HashTable((N_ficheiro-1)/100); //inicia a hash table de hoteis

    if (Users) {

    for (int i=0;i<N_ficheiro;i++) {

        int j=0; //Numero do campo
        char* string=lerLinha(ficheiro); //Obtem a string
        string[strlen(string)-1]='\0'; //tira o '\n' da string

        char* string_copia=strdup(string); //para o ficheiro de erros

    if (i==0) {  //mete a primeira linha nos reservations errors
        
        escreveLinha(erros,string_copia);
        free(string);

        }

    if (i>0) {

        //Criação da Pre-Reserva
        Pre_Reservas pre_reserva;

        char* token=NULL;
        char* string_aux=string;

        while(string) {          
              token=strsep(&string, ";");

                switch (j) {
                case 0: pre_reserva.idRserv=strdup(token);
                break;

                case 1: pre_reserva.user_id=strdup(token);
                break;

                case 2: pre_reserva.hotel_id=strdup(token);
                break;

                case 3: pre_reserva.hotel_name=strdup(token);
                break;

                case 4: pre_reserva.hotel_stars=strdup(token);
                break;

                case 5: pre_reserva.city_tax=strdup(token);
                break;

                case 6: pre_reserva.address=strdup(token);
                break;

                case 7: pre_reserva.begin_date=strdup(token);
                break;

                case 8: pre_reserva.end_date=strdup(token);
                break;

                case 9: pre_reserva.price_per_night=strdup(token);
                break;

                case 10: pre_reserva.includes_breakfast=strdup(token);
                break;

                case 11: pre_reserva.room_details=strdup(token);
                break;

                case 12: pre_reserva.rating=strdup(token);
                break;

                case 13: pre_reserva.comment=strdup(token);
                break;

                default:
                    break;
                }

              j++;
          }
/*
    printf("Pre-Reserva:\n");
    printf("Id: %s\n",pre_reserva.idRserv);
    printf("User Id: %s\n",pre_reserva.user_id);
    printf("Hotel Id: %s\n",pre_reserva.hotel_id);
    printf("Hotel Name: %s\n",pre_reserva.hotel_name);
    printf("Hotel Stars: %s\n",pre_reserva.hotel_stars);
    printf("City Tax: %s\n",pre_reserva.city_tax);
    printf("Adress: %s\n",pre_reserva.address);
    printf("Begin Date: %s\n",pre_reserva.begin_date);
    printf("End Date: %s\n",pre_reserva.end_date);
    printf("Price per night: %s\n",pre_reserva.price_per_night);
    printf("Includes Breakfast: %s\n",pre_reserva.includes_breakfast);
    printf("Room Details: %s\n",pre_reserva.room_details);
    printf("Rating: %s\n",pre_reserva.rating);
    printf("Comment: %s\n",pre_reserva.comment);*/

        dados->lidos++;

        if (reservation_validation(pre_reserva,Users)==1) { //Dado é valido, Deve ser incorporado
        
            Reservations* reserva=PreReserva_para_Reserva(pre_reserva);
            insere_HashTable(reserva->id,dados->reservas,reserva,&hashReservas);

            int hotel_existe=getHotelExiste(dados,reserva->hotel_id);
            if (hotel_existe==0) insere_HashTable(reserva->hotel_id,dados->hoteis,criaHotel(reserva),&hashHotel);
        
            insereReservaHoteis(reserva->hotel_id,reserva->id,dados);
            addRatingHotel(reserva->hotel_id,dados,reserva->rating);

            insereReservaUsers(reserva->user_id,reserva->id,Users);
            addTotalSpentUser(reserva->user_id,Users,calculaPrecoReserva(reserva->number_of_nights,reserva->city_tax,reserva->price_per_night));

            dados->succ++;
        }
        else { //Dado é invalido, deve ser excluido
        escreveLinha(erros,string_copia);
        dados->fail++;
        }

    free(string_aux);

    free(pre_reserva.idRserv);
    free(pre_reserva.user_id);
    free(pre_reserva.hotel_id);
    free(pre_reserva.hotel_name);
    free(pre_reserva.hotel_stars);
    free(pre_reserva.city_tax);
    free(pre_reserva.address);
    free(pre_reserva.begin_date);
    free(pre_reserva.end_date);
    free(pre_reserva.price_per_night);
    free(pre_reserva.includes_breakfast);
    free(pre_reserva.room_details);
    free(pre_reserva.rating); 
    free(pre_reserva.comment);

    }

    free(string_copia);

    }
    }

    // ############ fim ###########

    //printHashTable(dados->reservas,&printReservas);
    //printHashTable(dados->hoteis,&printHotel);

    apagaFicheiro(ficheiro);
    apagaFicheiro(erros);

    return dados;
}

//Função que apaga uma reserva
void apagaReserva(void* reserva_) {
    Reservations* reserva=reserva_;

        free(reserva->id);
        free(reserva->hotel_id);
        free(reserva->begin_date);
        free(reserva->end_date);
        free(reserva->user_id);
        free(reserva->hotel_name);
        free(reserva);
}

//Função que apaga um Hotel
void apagaHotel(void* hotel_) {
    Hotel* hotel=hotel_;
    free(hotel->hotel_id);
    free(hotel->hotel_name);
    apaga_ADStrings(hotel->reservations);
    free(hotel);
}

//Função que apaga as Reservas e Hoteis
void apagaListaReservasHoteis(Reservas_Dados* dados) {

    if (!dados) return;

    apagaHashTable(dados->reservas,&apagaReserva);
    apagaHashTable(dados->hoteis,&apagaHotel);
    free(dados);
}

//Função que imprime uma reserva
void printReservas(void* a) {
    Reservations* reserva=a;
    printf("%s;%s;%s (%s >> %s)",reserva->id,reserva->user_id,reserva->hotel_id,reserva->begin_date,reserva->end_date);
}

//Função que imprime um Hotel
void printHotel(void* a) {
    Hotel* hotel=a;
    printf("%s;%s;%d (%d)",hotel->hotel_id,hotel->hotel_name,hotel->hotel_stars,getADN(hotel->reservations));
    /*
        printf("reservas: ");
    for (int i=0;i<getADN(hotel->reservations);i++) {
        char* b=getADItem(hotel->reservations,i);
        printf("%s ",b);
    }*/
}