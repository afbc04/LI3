#include "queries.h"
#include "arraydin.h"
#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "users.h"
#include "reservas.h"
#include "voos.h"
#include "datas.h"

#include <locale.h>

#include "ficheiros.h"

#define LinhaMax 10000

typedef struct query_aux {
    void* dado1;
    void* dado2;
    void* dado3;
    void* dado4;
    void* dado5;
    void* dado6;
    void* dado7;
} QueryAux;

void apagaString(void* a_) {
  char* a=a_;
  free(a);
}

short comparaInts(void* a_, void* b_) {

  int* a=a_;
  int* b=b_;

  if (*a > *b) return 1;
  if (*a < *b) return -1;
  return 0;
}

// ###############################################################################################
//                                QUERY  1
// ###############################################################################################

//Dá um resumo de users, reservas ou voos
void query1(ArrayDin* output, short tipo_output, char* id, User_Dados* Users, Reservas_Dados* Reservas, Voos_Dados* Voos) {
  // tipo_output ->  |  0 = sem flag "F"  |  1 = com flag "F"

  if (getVooExiste(Voos,id)==1) { //Imprime o Voo

      char linha[LinhaMax];

      char* voo_airline=getVooAirline(Voos,id);
      char* voo_planemodel=getVooPlaneModel(Voos,id);
      char* voo_origin=getVooOrigin(Voos,id);
      char* voo_destino=getVooDestination(Voos,id);
      char* voo_departure=getVooDeparture(Voos,id);
      char* voo_arrival=getVooArrival(Voos,id);


      if (tipo_output==0) sprintf(linha,"%s;%s;%s;%s;%s;%s;%d;%d",voo_airline,voo_planemodel,voo_origin,voo_destino,voo_departure,voo_arrival,getVooNPassengers(Voos,id),getVooDelay(Voos,id));
      if (tipo_output==1) sprintf(linha,"airline: %s\nplane_model: %s\norigin: %s\ndestination: %s\nschedule_departure_date: %s\nschedule_arrival_date: %s\npassengers: %d\ndelay: %d",voo_airline,voo_planemodel,voo_origin,voo_destino,voo_departure,voo_arrival,getVooNPassengers(Voos,id),getVooDelay(Voos,id));

      free(voo_airline);
      free(voo_planemodel);
      free(voo_origin);
      free(voo_destino);
      free(voo_departure);
      free(voo_arrival);

      inserir_ArrayDin(strdup(linha),output);

    }
    else if (getReservaExiste(Reservas,id)==1) { //Imprime a Reserva

      char linha[LinhaMax];

      char* reserva_hotel_id=getReservaHotelID(Reservas,id);
      char* reserva_hotel_name=getReservaHotelName(Reservas,id);
      char* reserva_begin=getReservaBeginDate(Reservas,id);
      char* reserva_end=getReservaEndDate(Reservas,id);
      char* reserva_breakfast=getReservaIncludesBreakfast(Reservas,id);

      if (tipo_output==0) sprintf(linha,"%s;%s;%d;%s;%s;%s;%d;%.3f",reserva_hotel_id,reserva_hotel_name,getReservaHotelStars(Reservas,id),reserva_begin,reserva_end,reserva_breakfast,getReservaNights(Reservas,id),getReservaTotalPrice(Reservas,id));
      if (tipo_output==1) sprintf(linha,"hotel_id: %s\nhotel_name: %s\nhotel_stars: %d\nbegin_date: %s\nend_date: %s\nincludes_breakfast: %s\nnights: %d\ntotal_price: %.3f",reserva_hotel_id,reserva_hotel_name,getReservaHotelStars(Reservas,id),reserva_begin,reserva_end,reserva_breakfast,getReservaNights(Reservas,id),getReservaTotalPrice(Reservas,id));

      free(reserva_hotel_id);
      free(reserva_hotel_name);
      free(reserva_begin);
      free(reserva_end);
      free(reserva_breakfast);

      inserir_ArrayDin(strdup(linha),output);

    }
    else if (getUserExiste(Users,id)==1 && UserAtivo(Users,id)==1) { //Imprime o User

      char linha[LinhaMax];

      char* user_name=getUserName(Users,id);
      char* user_sex=getUserSex(Users,id);
      char* user_cc=getUserCountryCode(Users,id);
      char* user_passport=getUserPassport(Users,id);

      if (tipo_output==0) sprintf(linha,"%s;%s;%d;%s;%s;%d;%d;%.03f",user_name,user_sex,getUserAge(Users,id),user_cc,user_passport,getUserNumeroVoos(Users,id),getUserNumeroReservas(Users,id),getUserTotalSpent(Users,id));
      if (tipo_output==1) sprintf(linha,"name: %s\nsex: %s\nage: %d\ncountry_code: %s\npassport: %s\nnumber_of_flights: %d\nnumber_of_reservations: %d\ntotal_spent: %.03f",user_name,user_sex,getUserAge(Users,id),user_cc,user_passport,getUserNumeroVoos(Users,id),getUserNumeroReservas(Users,id),getUserTotalSpent(Users,id));

      free(user_name);
      free(user_sex);
      free(user_cc);
      free(user_passport);

      inserir_ArrayDin(strdup(linha),output);

    }
    else { //Id não existe

    }

  return;
}

// ###############################################################################################
//                                QUERY  2
// ###############################################################################################

//Função auxiliar que compara as reservas/voos
short query2_aux(void* a_,void* b_) {

    QueryAux* a=a_;
    QueryAux* b=b_;

    int res=(-1)*comparaDatas(a->dado4,b->dado4);

    if (res==0) res=strcmp(a->dado1,b->dado1);

    return res;
}

void apagaQueryAux2(void* a_) {
  QueryAux* a=a_;
  char* n=a->dado2;
  char* c=a->dado1;
  bool* b=a->dado3;
  char* d=a->dado4;
  free(n);
  free(c);
  free(b);
  free(d);
  free(a);
}

// Função para a query 2
void query2(ArrayDin* output,char* user_id,char* arg2,short tipo_output,User_Dados* Users, Voos_Dados* Voos, Reservas_Dados* Reservas)   // 3 HOTEL STARS
{  
  // user_id -> argumento com o id do user
  // arg2 -> argumento opcional que pode ser reservas ou voos
  // tipo_output ->  |  0 = sem flag "F"  |  1 = com flag "F"

   if (getUserExiste(Users,user_id)==0 || UserAtivo(Users,user_id)==0) return;

   ArrayDin* lista=inicia_ArrayDin(50); //array din com os voos e reservas
  
  //modo_output:  0 = voos e reservas | 1 = só voos | 2 = só reservas
   short modo_output=0;
   if (strcmp(arg2,"flights")==0) modo_output=1;
   if (strcmp(arg2,"reservations")==0) modo_output=2;

  // i_voo e i_reservas corresponde a quantos reservas e voos existem no user

   int N_ItensUser; //Numero das reservas e voos

   if (modo_output!=1) { //Copia as reservas

   N_ItensUser=getUserNumeroReservas(Users,user_id);

   ArrayDin* AD_reservas=getUserReservas(Users,user_id);

   for (int i=0;i<N_ItensUser;i++) {
    char* reserva=getADItem(AD_reservas,i);

    QueryAux* aux=malloc(sizeof(QueryAux));
    //Dado1 -> id  |  Dado2 -> Data  | Dado3 -> reserva?

    char* reserva_id=getReservaID(Reservas,reserva);
    char* reserva_dia=getReservaBeginDate(Reservas,reserva);
    bool* e_reserva=malloc(sizeof(bool));
    *e_reserva=true;
    char* reserva_data=getReservaBeginDate(Reservas,reserva);


    aux->dado1=reserva_id;
    aux->dado2=reserva_dia;
    aux->dado3=e_reserva;
    aux->dado4=reserva_data;

    inserir_ArrayDin(aux,lista);

   }

   apaga_ADStrings(AD_reservas);

   }

   if (modo_output!=2) { //Copia os voos

   N_ItensUser=getUserNumeroVoos(Users,user_id);

   ArrayDin* AD_voos=getUserVoos(Users,user_id);

   for (int i=0;i<N_ItensUser;i++) {
    char* voo=getADItem(AD_voos,i);

    QueryAux* aux=malloc(sizeof(QueryAux));
    //Dado1 -> id  |  Dado2 -> Data  | Dado3 -> reserva?

    char* voo_id=getVooId(Voos,voo);
    char* voo_data_aux=getVooDeparture(Voos,voo);

    char voo_aux2[100];
    for (int j=0;j<10;j++) voo_aux2[j]=voo_data_aux[j];
    voo_aux2[10]='\0';

    char* voo_dia=strdup(voo_aux2);
    free(voo_data_aux);

    bool* e_reserva=malloc(sizeof(bool));
    *e_reserva=false;

    char* voo_data=getVooDeparture(Voos,voo);

    aux->dado1=voo_id;
    aux->dado2=voo_dia;
    aux->dado3=e_reserva;
    aux->dado4=voo_data;

    inserir_ArrayDin(aux,lista);

   }

   apaga_ADStrings(AD_voos);

   }

  quickSort(lista,0,getADN(lista)-1,&query2_aux);

  int N=getADN(lista);

  for (int i=0;i<N;i++) {

      QueryAux* query=getADItem(lista,i);

      char linha[LinhaMax];

      char* id=strdup(query->dado1);
      char* data=strdup(query->dado2);
      bool* reserva=query->dado3;

      if (tipo_output==0) { //Sem Flag
        
        if (modo_output!=0) sprintf(linha,"%s;%s",id,data);
        else {
          if (*reserva==true) sprintf(linha,"%s;%s;reservation",id,data);
          else sprintf(linha,"%s;%s;flight",id,data);       
        }

      }
      if (tipo_output==1) { //Com flag

        if (modo_output!=0) sprintf(linha,"id: %s\ndate: %s",id,data);
        else {
          if (*reserva==true) sprintf(linha,"id: %s\ndate: %s\ntype: reservation",id,data);
          else sprintf(linha,"id: %s\ndate: %s\ntype: flight",id,data);       
        }

      }

      free(id);
      free(data);

      inserir_ArrayDin(strdup(linha),output);

  }

  apaga_ArrayDin(lista,&apagaQueryAux2);

   return;
}

// ###############################################################################################
//                                QUERY  3
// ###############################################################################################

//tempo 135 seg

// Função da query 3 - Indicar o rating do Hotel
void query3(ArrayDin* output, short tipo_output, char* id, Reservas_Dados* Reservas)   // 3 HOTEL STARS
{  
  // query -> indica o id do Hotel
  // tipo_output ->  |  0 = sem flag "F"  |  1 = com flag "F"

  if (getHotelExiste(Reservas,id)==0) return;

  double rating=getHotelRating(Reservas,id);

  if (rating>=0) {

    char linha[LinhaMax];
    if (tipo_output==0) sprintf(linha,"%.03f",rating);
    if (tipo_output==1) sprintf(linha,"rating: %.03f",rating);
    inserir_ArrayDin(strdup(linha),output);

  }

   return;   
}

// ###############################################################################################
//                                QUERY  4
// ###############################################################################################

// Função da query 4
void query4(ArrayDin* output, short tipo_output, char* id, Reservas_Dados* Reservas)
{  
  // query -> indica o id do Hotel
  // tipo_output ->  |  0 = sem flag "F"  |  1 = com flag "F"

  if (getHotelExiste(Reservas,id)==1) {

    ordenaReservasHotel(id,Reservas);

    ArrayDin* reservas=getHotelReservas(Reservas,id);
    int N=getADN(reservas);

    for (int i=0;i<N;i++) {

      char linha[LinhaMax];
      char* r_id=getADItem(reservas,i);

      char* reserva_id=getReservaID(Reservas,r_id);
      char* reserva_begin=getReservaBeginDate(Reservas,r_id);
      char* reserva_end=getReservaEndDate(Reservas,r_id);
      char* reserva_user=getReservaUserID(Reservas,r_id);

    if (tipo_output==0) sprintf(linha,"%s;%s;%s;%s;%d;%.03f",reserva_id,reserva_begin,reserva_end,reserva_user,getReservaRating(Reservas,r_id),getReservaTotalPrice(Reservas,r_id));
    if (tipo_output==1) sprintf(linha,"id: %s\nbegin_date: %s\nend_date: %s\nuser_id: %s\nrating: %d\ntotal_price: %.3f",reserva_id,reserva_begin,reserva_end,reserva_user,getReservaRating(Reservas,r_id),getReservaTotalPrice(Reservas,r_id));

      free(reserva_id);
      free(reserva_begin);
      free(reserva_end);
      free(reserva_user);

    inserir_ArrayDin(strdup(linha),output);

    }

    apaga_ArrayDin(reservas,&apagaString);

  }
   
   return;   
}

// ###############################################################################################
//                                QUERY  5
// ###############################################################################################

//Função para a execução da query 5
short query5_aux(void* a_,void* b_) {

    QueryAux* a=a_;
    QueryAux* b=b_;

    int res = (-1) * comparaDatas(a->dado2, b->dado2);

    if (res==0) res = strcmp(a->dado1, b->dado1);

    return res;
}

void apagaQueryAux5(void* a_) {
  QueryAux* a=a_;
  char* b=a->dado2;
  char* c=a->dado1;
  free(b);
  free(c);
  free(a);
}

//Função para a execução da query 5
void query5(ArrayDin* output, short tipo_output, char* nome_, char* begin , char* end , Voos_Dados* Voos) {
  // tipo_output ->  |  0 = sem flag "F"  |  1 = com flag "F"

  if(comparaDatas(begin, end)==1) return;

  for(int i=0; i<strlen(nome_); i++){
    nome_[i] = toupper(nome_[i]);
  }

  if(getAeroExiste(Voos, nome_)==0) return;

  ArrayDin* voosforAero = getAeroVoos(Voos, nome_);

  ArrayDin* voosEntreDatas = inicia_ArrayDin(2);

  for(int j=0; j<getADN(voosforAero); j++){
    char* nome_voo = getADItem(voosforAero, j);

    if(getVooEntreDatas(Voos, nome_voo, begin, end)==0 || getVooEntreDatas(Voos, nome_voo, begin, end)==-1) continue;
    if(getVooToAero(Voos, nome_voo, nome_)==1 || getVooToAero(Voos, nome_voo, nome_)==2) continue;

    QueryAux* query_voo = malloc(sizeof(QueryAux));

    query_voo->dado1 = strdup(nome_voo);
    query_voo->dado2 = getVooDeparture(Voos, nome_voo);

    inserir_ArrayDin(query_voo, voosEntreDatas);
  }

  quickSort(voosEntreDatas,0,getADN(voosEntreDatas)-1,&query5_aux);

  apaga_ADStrings(voosforAero);

  for (int i=0; i<getADN(voosEntreDatas); i++) {

      char linha[LinhaMax];
      QueryAux* voo_item = getADItem(voosEntreDatas, i);
      char* voo_id = voo_item->dado1;
      char* scheduled_date = voo_item->dado2;
      char* destination = getVooDestination(Voos, voo_id);
      char* airline = getVooAirline(Voos, voo_id);
      char* plane_model = getVooPlaneModel(Voos, voo_id);


    if (tipo_output==0) sprintf(linha,"%s;%s;%s;%s;%s", voo_id, scheduled_date, destination, airline, plane_model);
    if (tipo_output==1) sprintf(linha,"id: %s\nschedule_departure_date: %s\ndestination: %s\nairline: %s\nplane_model: %s", voo_id, scheduled_date, destination, airline, plane_model);

    free(destination);
    free(airline);
    free(plane_model);

    inserir_ArrayDin(strdup(linha),output);

  }
    
    apaga_ArrayDin(voosEntreDatas, &apagaQueryAux5);

return;
}

// ###############################################################################################
//                                QUERY  6
// ###############################################################################################

//Função para a execução da query 6
short query6_aux(void* a_,void* b_) {

    QueryAux* a=a_;
    QueryAux* b=b_;

    int res=(-1)*comparaInts(a->dado2,b->dado2);
    if (res==0) res=strcmp(a->dado1,b->dado1);

    return res;
}

void apagaQueryAux6(void* a_) {
  QueryAux* a=a_;
  int* n=a->dado2;
  char* c=a->dado1;
  free(n);
  free(c);
  free(a);
}

//Função para a execução da query 6
void query6(ArrayDin* output, short tipo_output,char* ano_, char* N_, Voos_Dados* Voos) 
  // ano_ -> ano do argumento
  // N_ -> numero de aeroportos a verificar
  // tipo_output ->  |  0 = sem flag "F"  |  1 = com flag "F"
{
  int ano = atoi(ano_); // transforma o argumento ano em int;
  int n_aeros= atoi(N_);
  ArrayDin* AD_aeros = getAeroLista(Voos);
  int num_aeros = getADN(AD_aeros);
   
  ArrayDin* dados =  inicia_ArrayDin(num_aeros);

  for(int i=0;i<num_aeros;i++)
     {    
          QueryAux* aux = malloc(sizeof(QueryAux));
          char* aero_id = getADItem(AD_aeros,i); 
          int* n_passageiros = malloc(sizeof(int)); 
          *n_passageiros= getAeroPassageirosAno(Voos,aero_id,ano);

          aux->dado2=n_passageiros; // numero de passageiros de um dado aeroporto
          char* nameAero = strdup(aero_id); // nome do aeroporto
          aux->dado1 = nameAero; // guardar nome do aeroporto

          inserir_ArrayDin(aux,dados);
     }
     quickSort(dados,0,num_aeros-1,&query6_aux);
     
     int N = n_aeros; // numero de aeroportos pedidos
     if(N>num_aeros) N=num_aeros; 

      for (int i=0;i<N;i++) {

      char linha[LinhaMax];
      QueryAux* aux=getADItem(dados,i);

      char* aux_nome=aux->dado1;
      int* aux_npassageiros=aux->dado2;

    if (tipo_output==0) sprintf(linha,"%s;%d",aux_nome,*aux_npassageiros);
    if (tipo_output==1) sprintf(linha,"name: %s\npassengers: %d",aux_nome,*aux_npassageiros);
    inserir_ArrayDin(strdup(linha),output);
}
    apaga_ArrayDin(dados,&apagaQueryAux6);
    apaga_ADStrings(AD_aeros);

return;
}

// ###############################################################################################
//                                QUERY  7
// ###############################################################################################

//Função auxiliar que compara as medianas
short query7_aux(void* a_,void* b_) {

    QueryAux* a=a_;
    QueryAux* b=b_;

    int res=(-1)*comparaInts(a->dado2,b->dado2);
    if (res==0) res=strcmp(a->dado1,b->dado1);

    return res;
}

void apagaQueryAux7(void* a_) {
  QueryAux* a=a_;
  int* n=a->dado2;
  char* c=a->dado1;
  free(n);
  free(c);
  free(a);
}

//Função para a execução da query 7
void query7(ArrayDin* output, short tipo_output, char* N_, Voos_Dados* Voos) {
  // N_ -> quantos aeroportos com maior medianas devem ser imprimidos
  // tipo_output ->  |  0 = sem flag "F"  |  1 = com flag "F"

  calcularMedianasAeroportos(Voos);

  int N_Mediana=atoi(N_); //Transforma o argumento em um inteiro

  if (N_Mediana>0) { //Se esse inteiro é positivo, então executa a query 7

  ArrayDin* AD_aero=getAeroLista(Voos); //Lista com os nomes dos aeroportos
  int N_aero=getADN(AD_aero);

  ArrayDin* lista=inicia_ArrayDin(N_aero);

  for (int i=0;i<N_aero;i++) {

    char* aero_id=getADItem(AD_aero,i);
    int mediana=getAeroMediana(Voos,aero_id);

    if (mediana<0) continue; //Aeroporto não tem mediana de atraso

    QueryAux* aux=malloc(sizeof(QueryAux));
    //Dado1 -> nome   |  Dado2 -> mediana
    int* n_aux=malloc(sizeof(int));
    *n_aux=mediana;
     aux->dado2=n_aux;

     char* aux_dado2=strdup(aero_id);
     int aux_dado2_n=strlen(aero_id);
     for (int i=0;i<aux_dado2_n;i++) aux_dado2[i] = toupper((unsigned char)aux_dado2[i]);

     aux->dado1=aux_dado2;

     inserir_ArrayDin(aux,lista);
  }

  int aero_validos=getADN(lista);
  quickSort(lista,0,aero_validos-1,&query7_aux);

  int N=N_Mediana; //Indica quantos outputs devem ser imprimidos
  if (N>aero_validos) N=aero_validos; //Se o N de outputs excede o numero de elementos da lista, o N passa a ser o número de elementos da lista

  for (int i=0;i<N;i++) {

      char linha[LinhaMax];
      QueryAux* aux=getADItem(lista,i);

      char* aux_name=aux->dado1;
      int* aux_median=aux->dado2;

    if (tipo_output==0) sprintf(linha,"%s;%d",aux_name,*aux_median);
    if (tipo_output==1) sprintf(linha,"name: %s\nmedian: %d",aux_name,*aux_median);

    inserir_ArrayDin(strdup(linha),output);

  }
    apaga_ArrayDin(lista,&apagaQueryAux7);
    apaga_ADStrings(AD_aero);
  }

return;
}

// ###############################################################################################
//                                QUERY  8
// ###############################################################################################

// Função da query 8
void query8(ArrayDin* output, short tipo_output, char* id, char* begin, char* end, Reservas_Dados* Reservas)
{  
  // id -> indica o id do Hotel
  // begin -> inicio da data  |  end -> fim da data
  // tipo_output ->  |  0 = sem flag "F"  |  1 = com flag "F"
  int priceT=0;
  int c=0;
  if(comparaDatas(begin, end)==1) return; // a primeira data é mais recente que a segunda data
  
  if(getHotelExiste(Reservas,id)==0) return;

     ArrayDin* hotelReservas = getHotelReservas(Reservas,id); // array dinamico com as reservas do hotel
     
    int num_reservas = getADN(hotelReservas);

    for(int i=0;i<num_reservas;i++)
       {
        char* reserva = getADItem(hotelReservas,i);
        
        c = getReservaNightPriceDates(Reservas,reserva,begin,end); // calcula o preço das noites de uma reserva entre as duas datas 

        priceT = priceT + c;   //acumular o preço de cada reserva
        c=0;
       }
       char linha[LinhaMax];

       if (tipo_output==0) sprintf(linha,"%d",priceT);
       if (tipo_output==1) sprintf(linha,"revenue: %d",priceT);
       inserir_ArrayDin(strdup(linha),output);
    apaga_ADStrings(hotelReservas);
   return;   
}

// ###############################################################################################
//                                QUERY  9
// ###############################################################################################

//Função auxiliar que indica se o user tem o prefixo
int query9_prefixo(char* nome, const char*prefixo) {
  //Função que verifica se a string "nome" tem o prefixo definido pela string "prefixo"
  int N=strlen(prefixo);

  //Ciclo para testar char por char
  for (int i=0;i<N;i++) {
    int dif=nome[i]-prefixo[i];
    if (dif>0) return -1; //Se os chars são diferentes, então "nome" não tem o prefixo "prefixo"
    if (dif<0) return 1;
  }

  return 0; //O ciclo acabou, logo a string "nome" tem o prefixo "prefixo"
}

//Função auxiliar que compara os nomes
short query9_aux(void* a_,void* b_) {

    QueryAux* a=a_;
    QueryAux* b=b_;

    int res=strcoll(a->dado2,b->dado2);
    if (res==0) res=strcoll(a->dado1,b->dado1);

    return res;
}

void apagaQueryAux9(void* a_) {
  QueryAux* a=a_;
  char* n=a->dado2;
  char* c=a->dado1;
  free(n);
  free(c);
  free(a);
}

//Função para a execução da query 9
void query9(ArrayDin* output, short tipo_output, char* prefixo,User_Dados* Users) {
  // prefixo -> prefixo que é preciso ter
  // tipo_output ->  |  0 = sem flag "F"  |  1 = com flag "F"

  if (!Users) return;

  ArrayDin* users=getUserLista(Users);
  int N=getADN(users);

  ArrayDin* lista=inicia_ArrayDin(1);

  for (int i=0;i<N;i++) {

    char* user_id=getADItem(users,i);
    char* user_name=getUserName(Users,user_id);

    if (UserAtivo(Users,user_id) && query9_prefixo(user_name,prefixo)==0) {

    QueryAux* aux=malloc(sizeof(QueryAux));
    //Dado1 -> nome   |  Dado2 -> id
    aux->dado1=strdup(user_id);
    aux->dado2=user_name;

     inserir_ArrayDin(aux,lista);
    }

    else free(user_name);

  }

  N=getADN(lista);
  quickSort(lista,0,N-1,&query9_aux);

  for (int i=0;i<N;i++) {

      char linha[LinhaMax];

      QueryAux* aux=getADItem(lista,i);

      char* aux_id=aux->dado1;
      char* aux_name=aux->dado2;

    if (tipo_output==0) sprintf(linha,"%s;%s",aux_id,aux_name);
    if (tipo_output==1) sprintf(linha,"id: %s\nname: %s",aux_id,aux_name);

    inserir_ArrayDin(strdup(linha),output);

  }
  
    apaga_ArrayDin(lista,&apagaQueryAux9);
    apaga_ADStrings(users);

return;
}

// ###############################################################################################
//                                QUERY  10
// ###############################################################################################

//Função auxiliar que compara as medianas
short query10_aux(void* a_,void* b_) {

    char* a=a_;
    char* b=b_;

    int a_int=atoi(a);
    int b_int=atoi(b);

    if (a_int>b_int) return 1;
    else if (a_int<b_int) return -1;
    return 0;
}

void apagaQueryAux10(void* a_) {
  QueryAux* a=a_;
  int* b=a->dado1;
  int* c=a->dado2;
  int* d=a->dado3;
  int* e=a->dado4;
  int* f=a->dado5;
  apaga_ADStrings(a->dado6);
  free(f);
  free(e);
  free(d);
  free(c);
  free(b);
  free(a);
}

//Função Hash Function para a Hash Table da query 10
int hashQ10(const char*s, int Max) {
    int A=atoi(s);
    return (A%Max);
}

//Função que cria um Hotel
QueryAux* criaQA10() {

    QueryAux* novo=malloc(sizeof(QueryAux)); //criar nodo

    int* reservas=malloc(sizeof(int));
    *reservas=0;
    int* voos=malloc(sizeof(int));
    *voos=0;
    int* users=malloc(sizeof(int));
    *users=0;
    int* passengers=malloc(sizeof(int));
    *passengers=0;
    int* passengers_unique=malloc(sizeof(int));
    *passengers_unique=0;

    ArrayDin* uni_pass=inicia_ArrayDin(1);

    novo->dado1=reservas;
    novo->dado2=voos;
    novo->dado3=users;
    novo->dado4=passengers;
    novo->dado5=passengers_unique;
    novo->dado6=uni_pass;

    return novo;
}

//Função para a execução da query 10
void query10(ArrayDin* output, short tipo_output, char* ano_, char* mes_, User_Dados* Users, Reservas_Dados* Reservas, Voos_Dados* Voos) {
  // ano -> qual ano
  // mes -> qual mes
  // tipo_output ->  |  0 = sem flag "F"  |  1 = com flag "F"


  char data[12];
  int data_n=0;
  //data_n: 0 -> todos os anos  |  1 -> todos os meses  |  2-> todos os dias

  if (strlen(ano_)==0) { //Imprime de todos os anos

  sprintf(data,"0000/00/00");
  data_n=0;

  }
  else { //Tem um ano...

    if (strlen(mes_)==0) { //Mostra todos os meses

    sprintf(data,"%d/00/00",atoi(ano_));
    data_n=1;

    }
    else { //Mostra todos os dias do mes

    sprintf(data,"%d/%d/00",atoi(ano_),atoi(mes_));
    data_n=2;

    }

  }

  //printf("Data: %s\nN: %d\n",data,data_n);

  HashTable dados_q10=inicia_HashTable(50);

  ArrayDin* users=getUserLista(Users);
  ArrayDin* reservas=getReservaLista(Reservas);
  ArrayDin* voos=getVooLista(Voos);

  int N_users=getADN(users);
  int N_reservas=getADN(reservas);
  int N_voos=getADN(voos);

  int N=0;

  //Calcula quais das listas têm mais elementos
  if (N_users>N_reservas) N=N_users;
  else N=N_reservas;

  if (N<N_voos) N=N_voos;

  int int_ano=atoi(ano_);
  int int_mes=atoi(mes_);

  for (int i=0;i<N;i++) {

    // Analisa USER ###################
    if (i<N_users) { //Analisa um user

    char* user_id=getADItem(users,i);
    char* user_creation=getUserCreation(Users,user_id);

    short data=-1;

    if (data_n==0) data=getDataAno(user_creation);
    if (data_n==1 && int_ano==getDataAno(user_creation)) data=getDataMes(user_creation);
    if (data_n==2 && int_ano==getDataAno(user_creation) && int_mes==getDataMes(user_creation)) data=getDataDia(user_creation);

    free(user_creation);

    if (data>=0) { //O user deve ser considerado

    char data_string[10];

    sprintf(data_string,"%d",data);

    void* data_existe=getHTItem(data_string,dados_q10,&hashQ10);
    if (!data_existe) insere_HashTable(data_string,dados_q10,criaQA10(),&hashQ10);

    QueryAux* aux=getHTItem(data_string,dados_q10,&hashQ10);
    int* n=aux->dado3;
    (*n)++;

    }
    }

    // Analisa RESERVA ###################
    if (i<N_reservas) { //Analisa uma reserva

    char* reserva_id=getADItem(reservas,i);
    char* reserva_data=getReservaBeginDate(Reservas,reserva_id);

    short data=-1;

    if (data_n==0) data=getDataAno(reserva_data);
    if (data_n==1 && int_ano==getDataAno(reserva_data)) data=getDataMes(reserva_data);
    if (data_n==2 && int_ano==getDataAno(reserva_data) && int_mes==getDataMes(reserva_data)) data=getDataDia(reserva_data);

    free(reserva_data);

    if (data>=0) { //A reserva deve ser considerada

    char data_string[10];

    sprintf(data_string,"%d",data);

    void* data_existe=getHTItem(data_string,dados_q10,&hashQ10);
    if (!data_existe) insere_HashTable(data_string,dados_q10,criaQA10(),&hashQ10);

    QueryAux* aux=getHTItem(data_string,dados_q10,&hashQ10);
    int* n=aux->dado1;
    (*n)++;

    }
    }

    // Analisa VOO ###################
    if (i<N_voos) { //Analisa um voo

    char* voo_id=getADItem(voos,i);
    char* voo_data=getVooDeparture(Voos,voo_id);

    short data=-1;

    if (data_n==0) data=getDataAno(voo_data);
    if (data_n==1 && int_ano==getDataAno(voo_data)) data=getDataMes(voo_data);
    if (data_n==2 && int_ano==getDataAno(voo_data) && int_mes==getDataMes(voo_data)) data=getDataDia(voo_data);

    free(voo_data);

    if (data>=0) { //O voo deve ser considerado

    char data_string[10];

    sprintf(data_string,"%d",data);

    void* data_existe=getHTItem(data_string,dados_q10,&hashQ10);
    if (!data_existe) insere_HashTable(data_string,dados_q10,criaQA10(),&hashQ10);

    QueryAux* aux=getHTItem(data_string,dados_q10,&hashQ10);
    int* n=aux->dado2;
    int* n2=aux->dado4;
    ArrayDin* lista=aux->dado6;
    (*n)++;
    (*n2)+=getVooNPassengers(Voos,voo_id);

    inserir_ArrayDin(strdup(voo_id),lista);

    }
    }


  }

  ArrayDin* datas_q10=HashTable_to_AD(dados_q10);

  int N_datasq10=getADN(datas_q10);
  quickSort(datas_q10,0,N_datasq10-1,&query10_aux);

  apaga_ADStrings(users);
  apaga_ADStrings(reservas);
  apaga_ADStrings(voos);

  for (int i=0;i<N_datasq10;i++) {

      char linha[LinhaMax];
      char* string=getADItem(datas_q10,i);

      QueryAux* aux=getHTItem(string,dados_q10,&hashQ10);

      int* reservas=aux->dado1;
      int* voos=aux->dado2;
      int* users_=aux->dado3;
      int* passengers=aux->dado4;
      int* passengers_unique=aux->dado5;
      ArrayDin* lista=aux->dado6;

      resetUsersUniquePassengers(Users);

      int N=getADN(lista);
      for (int j=0;j<N;j++) {
        char* item=getADItem(lista,j);
      preenchePassageirosUnicos(Users,Voos,item);
      }

      *passengers_unique=getUsersUniquePassengers(Users);      

    if (tipo_output>=0) sprintf(linha,"%s;%d;%d;%d;%d;%d",string,*users_,*voos,*passengers,*passengers_unique,*reservas);
    if (tipo_output==1 && data_n==0) sprintf(linha,"year: %s\nusers: %d\nflights: %d\npassengers: %d\nunique_passengers: %d\nreservations: %d",string,*users_,*voos,*passengers,*passengers_unique,*reservas);
    if (tipo_output==1 && data_n==1) sprintf(linha,"month: %s\nusers: %d\nflights: %d\npassengers: %d\nunique_passengers: %d\nreservations: %d",string,*users_,*voos,*passengers,*passengers_unique,*reservas);
    if (tipo_output==1 && data_n==2) sprintf(linha,"day: %s\nusers: %d\nflights: %d\npassengers: %d\nunique_passengers: %d\nreservations: %d",string,*users_,*voos,*passengers,*passengers_unique,*reservas);

    inserir_ArrayDin(strdup(linha),output);

  }

    apagaHashTable(dados_q10,&apagaQueryAux10);
    apaga_ADStrings(datas_q10);
  
return;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
//                                                        INTERPRETER
//
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//Esta função personaliza o array dinamico colocando as strings para os fields
ArrayDin* colocaFields(ArrayDin* lista) {

    ArrayDin* novo=inicia_ArrayDin(1);

     int field_n=1;

    int N=getADN(lista);

    for (int i=0;i<N;i++) {

            char nova_linha[10000];
            char* linha_array=getADItem(lista,i);
            if (field_n>1) sprintf(nova_linha,"\n--- %d ---\n%s",field_n,linha_array);
            else sprintf(nova_linha,"--- %d ---\n%s",field_n,linha_array);
            inserir_ArrayDin(strdup(nova_linha),novo);
    field_n++;
    }

    return novo;

}


#define MAX_LINHAS 10
#define MAX_COLUNAS 100

//tira o primeiro char da string, neste caso o char é '\"'

void tiraChar(char* str)
{   
    int i;
    int t = strlen(str);
    char* s = strdup(str);

    for (i = 0; i < t - 1; i++) {
        str[i] = s[i + 1];
    }
    str[t - 1] = '\0';
    free(s);
}

//função que vai dar o tamanho da string argumento
int tam(char* s, char* c)
{
    int i = 0;
    char c1=*c;

    while (s[i] != '\0' && s[i] != c1) {
        i++;
    }
    return i;
}

/*
A query2 usada na função strsep precisa de ser atualizada para "avançar" o argumento entre aspas que já é guardado completo, logo
query2 vai passar a ser a string imediatamente após o argumento entre aspas que tivemos a tratar
*/
void auxFunc(char* a, int h,char* s2)
{
   int y=0;
   if(a==NULL) return;
   for(int u=h;s2[u]!= '\0';u++) // h é a posição de onde deve começar a copiar a string
   {
    a[y] = s2[u];
    y++;
   }
   a[y] = '\0';
}

/*
Função que devolve o argumento sem aspas
*/

char* arg_semAspas(char* s,char* s2,char** nova_string)
{
    int i=0,h = 0, t=0;
    char c='\"';   
    tiraChar(s); // tiramos o primeiro char da string que é '\"'
    int t1= tam(s,&c); // tamanho da string até à segunda aspa(tamanho total desse argumento)
    char* str = malloc(t1 + 1); 
    
    while (s[i] != '\0' && s[i] != '"') { //obter a string argumento
        str[i] = s[i];
        i++;
    }
     str[i] = '\0';
    int g = strlen(s2) - strlen(s); // g é a posição onde começa o argumento que estamos a tratar
    for(h=g+i+2;s2[h]!='\0';h++) //este loop é usado para obter o tamanho da string restante(após o argumento entre aspas) até ao fim                     
       {                         
        t++;
       }
    auxFunc(*nova_string,g+i+2,s2); /*esta função vai atualizar a string query2(nova_string),recebe a string original(s2)
    e a posição onde começa a string restante*/

return str;
}

/* 
função que pega numa query e extrai os seus argumentos para uma matriz args.
A função divide a query nos " ". Como existem argumentos que estão entre aspas, por exemplo,
"2023/02/01 16:00" a função verifica se o arg começa com '\"' e trata de forma especial, guardando na matriz
o argumento completo sem aspas.
*/

void quebraQuery(char* query, char args[MAX_LINHAS][MAX_COLUNAS], int* N)
{
    char* arg = NULL;
    char* query2 = strdup(query);
    char* qAux = query2;
    char* copiaQuery = strdup(query);
    char* p;
    char* str;
   
    int l = 0;
    while ((arg = strsep(&query2, " ")) != NULL)
    {   
        if (arg[0] == '\"') // caso em que o argumento está entre aspas
        {  
           
            p = strstr(copiaQuery, arg); // obtém-se a string desde arg até ao final
            str = arg_semAspas(p, copiaQuery, &query2); // esta função retorna o argumento sem aspas, e atualiza a string query2
            strcpy(args[l], str); // guarda na posição certa da matriz
            free(str);      
        }
        else  // caso em que não há argumentos com aspas, guarda direito na matriz 
        {
            strcpy(args[l], arg);
           
        }
        l++;
    } 
    *N = l;
    free(copiaQuery);
    free(qAux);
}

//Função que indica qual o id da query e se tem flag ou não
int processaIdQuery(char* query, short* flag) {

  *flag=0;
  int N=strlen(query);
  for (int i=0;i<N;i++) {
    if (query[i]=='F') *flag=1;
  }

  int id=atoi(query);
  if (id<1 || id>10) return -1;
  return id;
}

//Em cada função de interpretação da querie colocar a matriz usada para a divisão dos arguments
ArrayDin* interpreterQueries(char* query,User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos, bool batch)
{
  // query -> string com todas as informações da query do ficheiro de queries
  // nome_output -> indica qual nome devemos usar para o ficheiro de output
  // batch -> indica se devemos imprimir o resultado no terminal ou não

    char args[MAX_LINHAS][MAX_COLUNAS]; // matriz para guardar argumentos

    //Inicia o array colocando todos os chars como '\0'
    for (int i=0;i<MAX_LINHAS;i++) args[i][0]='\0';
    int N_query=0; //Indica quantos argumentos existem
    char* copiaQuery=strdup(query); //realiza uma copia da query
    quebraQuery(copiaQuery,args,&N_query); //Quebra a string e coloca cada argumento no array de strings "args"
     ArrayDin* output=inicia_ArrayDin(1); //Inicia o array dinamico onde vamos inserir o output

     short tipo_output=0; //número que indica se a query tem a flag 'F'
     int id_query=processaIdQuery(args[0],&tipo_output); //Indica qual o número da query a ser realizado
     switch(id_query) //Numero = número da query
     {
        case 1 : //Query 1
        query1(output,tipo_output,args[1],Users,Reservas,Voos);
        if (batch==true) printf("(Query 01) ");
        break;

        case 2 : //Query 2
        query2(output,args[1],args[2],tipo_output,Users,Voos,Reservas);
        if (batch==true) printf("(Query 02) ");
        break;

        case 3 : //Query 3
        query3(output,tipo_output,args[1],Reservas);
        if (batch==true) printf("(Query 03) ");
        break;

        case 4 : //Query 4
        query4(output,tipo_output,args[1],Reservas);
        if (batch==true) printf("(Query 04) ");
        break;

        case 5 : //Query 5
        query5(output,tipo_output,args[1],args[2],args[3],Voos);
        if (batch==true) printf("(Query 05) ");
        break;

        case 6 : //Query 6
        query6(output,tipo_output,args[1],args[2],Voos);
        if (batch==true) printf("(Query 06) ");
        break;

        case 7 : //Query 7
        query7(output,tipo_output,args[1],Voos);
        if (batch==true) printf("(Query 07) ");
        break;

        case 8 : //Query 8
        query8(output,tipo_output,args[1],args[2],args[3],Reservas);
        if (batch==true) printf("(Query 08) ");
        break;

        case 9 : //Query 9
        query9(output,tipo_output,args[1],Users);
        if (batch==true) printf("(Query 09) ");
        break;

        case 10 : //Query 10
        query10(output,tipo_output,args[1],args[2],Users,Reservas,Voos);
        if (batch==true) printf("(Query 10) ");
        break;

        default: //Query inválida ou não feita
        apaga_ADStrings(output); //Array dinamico deve ser NULL, então apaga-o
        output=NULL; //Define-o como NULL
        if (batch==true) printf("(\?\?\?) ");

        break;
     }
   free(copiaQuery);

   if (tipo_output==1 && output) { //Coloca fields caso a query tenha a flag 'F'
    ArrayDin* novo_output=colocaFields(output);
    apaga_ADStrings(output);
    return novo_output;
   }
   else {
    return output;
   }
}
