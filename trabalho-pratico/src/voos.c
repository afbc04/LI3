#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "users.h"
#include "voos.h"
#include <string.h>
#include "datas.h"
#include "ctype.h"

#include "arraydin.h"
#include "hashtable.h"

#include "ficheiros.h"

//Struct temporaria para a incorporação dos voos
typedef struct pre_dados_flights
{
    char* id;
    char* airline;
    char* plane_model;
    char* total_seats;
    char* origin;
    char* destination;
    char* schedule_departure_date;
    char* schedule_arrival_date;
    char* real_departure_date;
    char* real_arrival_date;
    char* pilot;
    char* copilot;
    char* notes;
} Pre_Voos;

//Struct para os voos
typedef struct voos_dados {
    char* airline; //companhia
    char* plane_model; //modelo do aviao
    char* origin; //Origem
    char* destination; //Destino
    char* schedule_departure; //Partida planeada
    char* schedule_arrival; //Chegada planeada
    int n_passengers; //Número de passageiros
    int max_passengers; //Capacidade máxima do voo
    int delay; //Atraso
    char* id; //Id do voo
    ArrayDin* passageiros; //Lista com os passageiros
} Voos;

//Struct para os Aeroportos
typedef struct aeroporto_dados {
    char* nome; //Nome do aeroporto (aka Origem)
    ArrayDin* voos; //array dinamico com os seus voos
    bool voos_ordenados; //indica se os voos estao ordenados
    int mediana; //mediana do atraso dos voos
    bool mediana_calculada; //indica se a mediana foi calculada
} Aeroporto;

//Estrutura com a base de dados dos voos e Aeroportos
typedef struct voos {
    HashTable Voos; //Voos
    HashTable Aero; //Aeroportos
    char voo_resultado; //Indica o resultado da leitura
    int voo_lidos; //Indica quantos dados foram lidos
    int voo_succ; //Indica quantos dados foram incorporados
    int voo_fail; //Indica quantos dados foram rejeitados
    char pass_resultado; //Indica o resultado da leitura
    int pass_lidos; //Indica quantos dados foram lidos
    int pass_succ; //Indica quantos dados foram incorporados
    int pass_fail; //Indica quantos dados foram rejeitados
} Voos_Dados;


// ##################################################################
//                      GETTERS
// ##################################################################

//Função Hash Function para a Hash Table de Voos
int hashVoos(const char*s, int Max) {
    int A=atoi(s);
    return (A%Max);
}

//Função que indica se um voo existe
int getVooExiste(Voos_Dados* HT, const char* s) {
    void* voo=procura_HashTable(s,HT->Voos,&hashVoos);
    if (voo) return 1;
    else return 0;
}

//Obtem Airline do Voo
char* getVooAirline(Voos_Dados* HT, const char* s) {
    Voos* voo= getHTItem(s,HT->Voos,&hashVoos);
    return strdup(voo->airline);
}

//Obtem Plane Model do Voo
char* getVooPlaneModel(Voos_Dados* HT, const char* s) {
    Voos* voo= getHTItem(s,HT->Voos,&hashVoos);
    return strdup(voo->plane_model);
}

//Obtem Origem do Voo
char* getVooOrigin(Voos_Dados* HT, const char* s) {
    Voos* voo= getHTItem(s,HT->Voos,&hashVoos);
    return strdup(voo->origin);
}

//Obtem Destino do Voo
char* getVooDestination(Voos_Dados* HT, const char* s) {
    Voos* voo= getHTItem(s,HT->Voos,&hashVoos);
    return strdup(voo->destination);
}

//Obtem Schedule Departure do Voo
char* getVooDeparture(Voos_Dados* HT, const char* s) {
    Voos* voo= getHTItem(s,HT->Voos,&hashVoos);
    return strdup(voo->schedule_departure);
}

//Obtem Schedule Arrival do Voo
char* getVooArrival(Voos_Dados* HT, const char* s) {
    Voos* voo= getHTItem(s,HT->Voos,&hashVoos);
    return strdup(voo->schedule_arrival);
}

//Obtem Numero Passageiros do Voo
int getVooNPassengers(Voos_Dados* HT, const char* s) {
    Voos* voo= getHTItem(s,HT->Voos,&hashVoos);
    return (int) voo->n_passengers;
}

//Obtem Maximo Passageiros do Voo
int getVooMaxPassengers(Voos_Dados* HT, const char* s) {
    Voos* voo= getHTItem(s,HT->Voos,&hashVoos);
    return (int) voo->max_passengers;
}

//Obtem Delay do Voo
int getVooDelay(Voos_Dados* HT, const char* s) {
    Voos* voo= getHTItem(s,HT->Voos,&hashVoos);
    return (int) voo->delay;
}

//Obtem Id do Voo
char* getVooId(Voos_Dados* HT, const char* s) {
    Voos* voo= getHTItem(s,HT->Voos,&hashVoos);
    return strdup(voo->id);
}

//Indica se um voo está entre 2 datas
int getVooEntreDatas(Voos_Dados* HT, const char* s, const char* inicio, const char* fim) {

    if (getVooExiste(HT,s)==0) return -1;

    Voos* voo= getHTItem(s,HT->Voos,&hashVoos);
    return entreDatas(voo->schedule_departure,voo->schedule_departure,inicio,fim);
}

//Obtem Array dinamico de todos os voos
ArrayDin* getVooLista(Voos_Dados* HT) {

    return HashTable_to_AD(HT->Voos);
}

//Função Hash para a HashTable de Aeroportos
int hashAero(const char* s_, int N) {

    char* s=strdup(s_);

    unsigned long a=0;
    int b=0;
    int N_=strlen(s);
    for (int i=0;i<N_;i++) {
        b=abs(s[i]);
        a = ((a << 5) + a) + b;
    }

    free(s);
    int res=a%N;
    return res;
}

//Função que indica se um voo existe
int getAeroExiste(Voos_Dados* HT, const char* s) {
    void* aero=procura_HashTable(s,HT->Aero,&hashAero);
    if (aero) return 1;
    else return 0;
}

//Obtem Nome do Aeroporto
char* getAeroNome(Voos_Dados* HT, const char* s) {
    Aeroporto* aero= getHTItem(s,HT->Aero,&hashAero);
    return strdup(aero->nome);
}

//Obtem Mediana do Aeroporto
int getAeroMediana(Voos_Dados* HT, const char* s) {
    Aeroporto* aero= getHTItem(s,HT->Aero,&hashAero);
    return (int) aero->mediana;
}

//Função que dá um array dinamico com todos os nomes dos aeroportos
ArrayDin* getAeroLista(Voos_Dados* HT) {

    if (!HT) return NULL;
    return HashTable_to_AD(HT->Aero); //Lista com os nomes dos aeroportos

}

//Obtem o número de passageiros de um ano
int getAeroPassageirosAno(Voos_Dados* dados,const char* aero_, short ano) {

    int resultado=0;

    Aeroporto* aero=getHTItem(aero_,dados->Aero,&hashAero);

    int N=getADN(aero->voos);

    for (int i=0;i<N;i++) {

        Voos* voo=getHTItem(getADItem(aero->voos,i),dados->Voos,&hashVoos);
        
        if (ano==getDataAno(voo->schedule_departure)) resultado+=voo->n_passengers;

    }

    return resultado;

}

//Indica qual é o aeroporto de um voo
int getVooToAero(Voos_Dados* dados, const char* voo_, const char* aero) {
    //  result: -1 : aeroporto de partida
    //           0 : aeroporto de partida e chegada
    //           1 : aeroporto de chegada
    //           2 : aeroporto nao existe

    Voos* voo=getHTItem(voo_,dados->Voos,&hashVoos);
    if (strcmp(voo->destination,aero)==0 && strcmp(voo->origin,aero)==0) return 0;
    if (strcmp(voo->destination,aero)==0 && strcmp(voo->origin,aero)!=0) return 1;
    if (strcmp(voo->destination,aero)!=0 && strcmp(voo->origin,aero)==0) return -1;

    return 2;
}

//Obtem Voos do Aeroporto
ArrayDin* getAeroVoos(Voos_Dados* HT, const char* s) {
    Aeroporto* aero= getHTItem(s,HT->Aero,&hashAero);
    
    ArrayDin* voos=inicia_ArrayDin(1);

    int N=getADN(aero->voos);

    for (int i=0;i<N;i++) {
        inserir_ArrayDin(strdup(getADItem(aero->voos,i)),voos);
    }

    return voos;
}


//Obter o resultado dos voos do Voos Dados
int getVooDadosResultado(Voos_Dados* lista) {
    return lista->voo_resultado;
}

//Obter os voos lidos do Voos Dados
int getVooDadosLidos(Voos_Dados* lista) {
    return lista->voo_lidos;
}

//Obter os voos incorporados do Voos Dados
int getVooDadosSucc(Voos_Dados* lista) {
    return lista->voo_succ;
}

//Obter os voos rejeitados do Voos Dados
int getVooDadosFail(Voos_Dados* lista) {
    return lista->voo_fail;
}

//Obter o resultado dos passageiros do Voos Dados
int getPassengersDadosResultado(Voos_Dados* lista) {
    return lista->pass_resultado;
}

//Obter os passageiros lidos do Voos Dados
int getPassengersDadosLidos(Voos_Dados* lista) {
    return lista->pass_lidos;
}

//Obter os passageiros incorporados do Voos Dados
int getPassengersDadosSucc(Voos_Dados* lista) {
    return lista->pass_succ;
}

//Obter os passageiros rejeitados do Voos Dados
int getPassengersDadosFail(Voos_Dados* lista) {
    return lista->pass_fail;
}

// ##################################################################
//                      SETTERS
// ##################################################################

//Função que insere um voo na lista de voos do aeroporto
void insereVooAero(const char* aero_, const char* id, Voos_Dados* Aero) {

    if(getAeroExiste(Aero,aero_)==0) return; //Aeroporto não existe


    Aeroporto* aero=getHTItem(aero_,Aero->Aero,&hashAero);

    char* voo_id=strdup(id);
    inserir_ArrayDin(voo_id,aero->voos);
}

//Função que ordena os voos de um aeroporto
void ordenaVoosAeroporto(const char* aero_, Voos_Dados* Aero) {

    if(getAeroExiste(Aero,aero_)==0) return; //Hotel não existe

    Aeroporto* aero= getHTItem(aero_,Aero->Aero,&hashAero);

    if (aero->voos_ordenados==true) return;

    ArrayDin* novos_voos=ordenaVoos(aero->voos,Aero);
    apaga_ADStrings(aero->voos);
    aero->voos=novos_voos;

    aero->voos_ordenados=true;
    return;
}

// ####### MEDIANA VOOS

//Função auxiliar para comparar numeros
short comparaNumeroMediana(void* a_, void* b_) {

    int* a=a_;
    int* b=b_;

    if (*a < *b) return -1;
    else if (*a > *b) return 1;
    else return 0;

}

//Função que indica a mediana a partir de uma lista de ints
int calculaMediana(ArrayDin* lista) {

    int N=getADN(lista);
    if (N==0) return -1;

    quickSort(lista,0,N-1,&comparaNumeroMediana);

    int mediana=0;

    int med=N/2;

    if (N%2==0) { //par
        int med2=0;
        int* med_aux=getADItem(lista,med);
        med2=*med_aux;

        med_aux=getADItem(lista,med-1);
        med2+=*med_aux;
        med2=med2/2;
        mediana=med2;
    }
    else { //impar
        int* med_aux=getADItem(lista,med);
        mediana=*med_aux;
    }
    
    //printf("\nMediana: %d",mediana);
    return mediana;
}

void apagaIntAux(void *a_) {
    int* a=a_;
    free(a);
}

void calcularMedianaAeroAux(void* a_,void* b_) {

        Voos_Dados* b=b_;
        Aeroporto* aero=a_;
        if (aero->mediana_calculada==true) return;

        int N2=getADN(aero->voos);
        ArrayDin* inteiros=inicia_ArrayDin(N2);
        for (int j=0;j<N2;j++) {

            char* voo_id=getADItem(aero->voos,j);
            if(getVooToAero(b,voo_id,aero->nome)>0) continue; //Se este voo nao existe no aeroporto ou se este voo chega ao aeroporto, descarta

            int* a=malloc(sizeof(int));
            *a= getVooDelay(b,voo_id);
            inserir_ArrayDin(a,inteiros);
        }
        aero->mediana=calculaMediana(inteiros);
        aero->mediana_calculada=true;
        //printf("\nValor Final: %d\n",aero->mediana);
        apaga_ArrayDin(inteiros,&apagaIntAux);

}

//Função que calcula as medianas dos aeroportos
void calcularMedianasAeroportos(Voos_Dados* Voo) {

    applyHashTable(Voo->Aero,&calcularMedianaAeroAux,Voo);

}

// ##################################################################
//                      FUNÇÕES
// ##################################################################

//Função que define os passageiros como únicos
void preenchePassageirosUnicos(User_Dados* Users, Voos_Dados* Voo, const char* nome) {

    Voos* voo= getHTItem(nome,Voo->Voos,&hashVoos);

    int N=voo->n_passengers;

    for (int i=0;i<N;i++) {
        setUserUniquePassenger(Users,getADItem(voo->passageiros,i));
    }    

}

//Struct auxiliar para a ordenação de reservas
struct ordena_voos {
    char* id;
    char* data;
};

//Função que serve para comparar Voos e util na ordenação de voos
short ordenaVoosAux(void* a_, void* b_) {

    struct ordena_voos* a=a_;
    struct ordena_voos* b=b_;

    short res=comparaDatas(a->data,b->data);
    if (res==0) res=strcmp(a->id,b->id);

    return res;
}

//Função auxiliar que apaga a lista de voos
void ordenaVoosApaga(void* a_) {
    struct ordena_voos* a=a_;
    free(a->id);
    free(a->data);
    free(a);
    return;
}

//Função que ordena uma lista de voos
ArrayDin* ordenaVoos(ArrayDin* lista,Voos_Dados* Voo) {

    int N=getADN(lista);
    ArrayDin* aux=inicia_ArrayDin(N);
    for (int i=0;i<N;i++) {
        struct ordena_voos* novo=malloc(sizeof(struct ordena_voos));
        Voos* voo=getHTItem(getADItem(lista,i),Voo->Voos,&hashVoos);
        novo->id=strdup(voo->id);
        novo->data=strdup(voo->schedule_departure);
        inserir_ArrayDin(novo,aux);
    }
    quickSort(aux,0,N-1,&ordenaVoosAux);

    ArrayDin* lista2=inicia_ArrayDin(N);
    for (int i=0;i<N;i++) {
        struct ordena_voos* voo=getADItem(aux,i);
        char* voo_id=strdup(voo->id);
        inserir_ArrayDin(voo_id,lista2);
    }

    apaga_ArrayDin(aux,&ordenaVoosApaga);
    return lista2;
}

int flight_validation(Pre_Voos flight){


    if (flight.id==NULL) return 0;
    if (flight.airline==NULL) return 0;
    if (flight.plane_model==NULL) return 0;
    if (flight.total_seats==NULL) return 0;
    if (flight.origin==NULL) return 0;
    if (flight.destination==NULL) return 0;
    if (flight.schedule_arrival_date==NULL) return 0;
    if (flight.schedule_departure_date==NULL) return 0;
    if (flight.real_departure_date==NULL) return 0;
    if (flight.real_arrival_date==NULL) return 0;
    if (flight.pilot==NULL) return 0;
    if (flight.copilot==NULL) return 0;
    if (flight.notes==NULL) return 0;

    //Validação das Datas com Tempo
    char* token1=NULL;
    char* hours1=NULL;
    char* string1 = strdup(flight.schedule_departure_date);
    char* string1_aux=string1;
    token1 = strsep(&string1," ");
    hours1 = strsep(&string1," ");

    char* token2=NULL;
    char* hours2=NULL;
    char* string2 = strdup(flight.schedule_arrival_date);
    char* string2_aux=string2;
    token2 = strsep(&string2," ");
    hours2 = strsep(&string2," ");

    char* token3=NULL;
    char* hours3=NULL;
    char* string3 = strdup(flight.real_departure_date);
    char* string3_aux=string3;
    token3 = strsep(&string3," ");
    hours3 = strsep(&string3," ");

    char* token4=NULL;
    char* hours4=NULL;
    char* string4 = strdup(flight.real_arrival_date);
    char* string4_aux=string4;
    token4 = strsep(&string4," ");
    hours4 = strsep(&string4," ");

    int valid=1;

    //Verifica que a data com tempo não possui apenas hora ou apenas data
    if(!token1 || !token2 || !token3 || !token4 || !hours1 || !hours2 || !hours3 || !hours4 || strlen(token1)<2 || strlen(hours1)<2 || strlen(token2)<2 || strlen(hours2)<2 || strlen(token3)<2 || strlen(hours3)<2 || strlen(token4)<2 || strlen(hours4)<2) valid=0;

if (valid==1) {

    //Verifica as datas
    if (dia_validation(token1)==0) valid=0;
    if (dia_validation(token2)==0) valid=0;
    if (dia_validation(token3)==0) valid=0;
    if (dia_validation(token4)==0) valid=0;

    //Verifica as Horas
    if (hora_validation(hours1)==0) valid=0;
    if (hora_validation(hours2)==0) valid=0;
    if (hora_validation(hours3)==0) valid=0;
    if (hora_validation(hours4)==0) valid=0;

    if (valid==1) {

    //o schedule_departure_date tem que vir antes de schedule_arrival_date, e o real_departure_date tem que vir antes de real_arrival_date;

    //schedule_departure_date tem que vir antes de schedule_arrival_date
    short ComparaDatasRes=comparaDatas(flight.schedule_arrival_date,flight.schedule_departure_date);
    //short ComparaDatasRes=comparaDatas(sch_a,sch_d);
    if (ComparaDatasRes==-1) valid=0;

    //real_departure_date tem que vir antes de real_arrival_date;
    ComparaDatasRes=comparaDatas(flight.real_arrival_date,flight.real_departure_date);
    //ComparaDatasRes=comparaDatas(real_a,real_d);
    if (ComparaDatasRes==-1) valid=0;

    ComparaDatasRes=comparaDatas(flight.real_departure_date,flight.schedule_departure_date);
    //ComparaDatasRes=comparaDatas(real,planeada);
    if (ComparaDatasRes==-1) valid=0;

    }
}
    free(string1_aux);
    free(string2_aux);
    free(string3_aux);
    free(string4_aux);

    if (valid==0) return 0;

    //Os aeroportos de origem e destino de um voo tem que ser constituídos por 3 letras
    if(strlen(flight.origin)!=3 || strlen(flight.destination)!=3) return 0;

    for(int i=0;i<3;i++){
        if(isalpha(flight.origin[i])==0) return 0;
    }
    for(int j=0;j<3;j++){
        if(isalpha(flight.destination[j])==0) return 0;
    }

    int N_lugares=atoi(flight.total_seats);
    if (N_lugares==0) return 0;

    //Os seguintes restantes campos têm que ter tamanho superior a zero:
    //Voo: id, airline, plane_model, pilot, copilot;
    if(strlen(flight.id)==0 || strlen(flight.airline)==0 || strlen(flight.plane_model)==0 || strlen(flight.pilot)==0 || strlen(flight.copilot)==0) return 0;


    return 1;
}

//Função que cria um dado 'Voos' com base nas informações recolhidas no parser de Voos
Voos* PreVoos_para_Voos(Pre_Voos dados) {

    Voos* novo_voo=malloc(sizeof(struct voos_dados));
    novo_voo->plane_model=strdup(dados.plane_model);
    novo_voo->origin=strdup(dados.origin);

    int N_origin=strlen(dados.origin);
    for (int i=0;i<N_origin;i++) novo_voo->origin[i] = toupper((unsigned char)novo_voo->origin[i]);

    novo_voo->destination=strdup(dados.destination);

    int N_destination=strlen(dados.destination);
    for (int i=0;i<N_destination;i++) novo_voo->destination[i] = toupper((unsigned char)novo_voo->destination[i]);

    novo_voo->schedule_arrival=strdup(dados.schedule_arrival_date);
    novo_voo->schedule_departure=strdup(dados.schedule_departure_date);

    novo_voo->delay=calculaAtraso(dados.schedule_departure_date,dados.real_departure_date);

    novo_voo->n_passengers=0;
    novo_voo->max_passengers=atoi(dados.total_seats);
    novo_voo->airline=strdup(dados.airline);
    novo_voo->id=strdup(dados.id);
    novo_voo->passageiros=inicia_ArrayDin(1);

    return novo_voo;
}

//Função que coleta todos os dados dos voos, processa-os e preenche a hash table de voos e aeroportos
Voos_Dados* iniciaVoos(const char* caminho_) {
//   Result:  0 -> nao há voos  |  1 -> ha voos da pasta caminho   |  2 -> ha voos da pasta alternativa

    Voos_Dados* dados=malloc(sizeof(Voos_Dados));
    dados->voo_lidos=0;
    dados->voo_fail=0;
    dados->voo_succ=0;

    if (!caminho_) {//Caminho inválido
        dados->voo_resultado=0;
        dados->Voos=inicia_HashTable(1);
        return dados;
    }

    dados->voo_resultado=1;

    // ################ Parser ##############

    char caminho[1000]; //cria caminho
    sprintf(caminho,"%s/flights.csv",caminho_);

    Ficheiro* ficheiro=iniciaFicheiro(caminho,"r");
    Ficheiro* erros=iniciaFicheiro("Resultados/flights_errors.csv","w+");
    
    if (!erros) {
        apagaFicheiro(ficheiro);
        dados->voo_resultado=0;
        dados->Voos=inicia_HashTable(1);
        return dados;
    }

    if (ficheiro==NULL) {
        ficheiro=iniciaFicheiro("flights.csv","r"); //Nao encontrou, procura na pasta default
        if (ficheiro) dados->voo_resultado=2;
    }
    
    if (ficheiro==NULL) { //Dados nao encontrados
        apagaFicheiro(erros);
        dados->voo_resultado=0;
        dados->Voos=inicia_HashTable(1);
        return dados;
    }

    // ################ Incorporação dos Voos ####################

    int N_ficheiro=LinhasFicheiro(ficheiro);

    dados->Voos=inicia_HashTable(N_ficheiro-1); //inicia a hash table de voos
    dados->Aero=inicia_HashTable((N_ficheiro-1)/100); //inicia a hash table de aeroportos

    for (int i=0;i<N_ficheiro;i++) {

        int j=0; //Numero do campo
        char* string=lerLinha(ficheiro); //Obtem a string
        string[strlen(string)-1]='\0'; //tira o '\n' da string

        char* string_copia=strdup(string); //para o ficheiro de erros

    if (i==0) {  //mete a primeira linha nos flights errors
        
        escreveLinha(erros,string_copia);
        free(string);

        }

    if (i>0) {

        //Criação do Pre-Voo
        Pre_Voos pre_voos;

        char* token=NULL;
        char* string_aux=string;

        while(string) {          
              token=strsep(&string, ";");

                switch (j) {
                case 0: pre_voos.id=strdup(token);
                break;

                case 1: pre_voos.airline=strdup(token);
                break;

                case 2: pre_voos.plane_model=strdup(token);
                break;

                case 3: pre_voos.total_seats=strdup(token);
                break;

                case 4: pre_voos.origin=strdup(token);
                break;

                case 5: pre_voos.destination=strdup(token);
                break;

                case 6: pre_voos.schedule_departure_date=strdup(token);
                break;

                case 7: pre_voos.schedule_arrival_date=strdup(token);
                break;

                case 8: pre_voos.real_departure_date=strdup(token);
                break;

                case 9: pre_voos.real_arrival_date=strdup(token);
                break;

                case 10: pre_voos.pilot=strdup(token);
                break;

                case 11: pre_voos.copilot=strdup(token);
                break;

                case 12: pre_voos.notes=strdup(token);
                break;
                
                default:
                    break;
                }

              j++;
          }
/*
    printf("Pre-Voo:\n");
    printf("Id: %s\n",pre_voos.id);
    printf("Airline: %s\n",pre_voos.airline);
    printf("Plane Model: %s\n",pre_voos.plane_model);
    printf("Total Seats: %s\n",pre_voos.total_seats);
    printf("Origin: %s\n",pre_voos.origin);
    printf("Destino: %s\n",pre_voos.destination);
    printf("S Departure: %s\n",pre_voos.schedule_departure_date);
    printf("S Arrival: %s\n",pre_voos.schedule_arrival_date);
    printf("R Departure: %s\n",pre_voos.real_departure_date);
    printf("R Arrival: %s\n",pre_voos.real_arrival_date);
    printf("Piloto: %s\n",pre_voos.pilot);
    printf("Copiloto: %s\n",pre_voos.copilot);
    printf("Notas: %s\n",pre_voos.notes);*/

        dados->voo_lidos++;

        if (flight_validation(pre_voos)==1) { //Dado é valido, Deve ser incorporado
        
            Voos* voos=PreVoos_para_Voos(pre_voos);
            insere_HashTable(voos->id,dados->Voos,voos,&hashVoos);

            dados->voo_succ++;
        }
        else { //Dado é invalido, deve ser excluido
        escreveLinha(erros,string_copia);
        dados->voo_fail++;
        }

    free(string_aux);

    free(pre_voos.id);
    free(pre_voos.airline);
    free(pre_voos.plane_model);
    free(pre_voos.total_seats);
    free(pre_voos.origin);
    free(pre_voos.destination);
    free(pre_voos.schedule_departure_date );
    free(pre_voos.schedule_arrival_date);
    free(pre_voos.real_departure_date);
    free(pre_voos.real_arrival_date);
    free(pre_voos.pilot);
    free(pre_voos.copilot);
    free(pre_voos.notes); 

    }

    free(string_copia);

    }

    // ############ fim ###########

    //printHashTable(dados->reservas,&printReservas);
    //printHashTable(dados->hoteis,&printHotel);

    apagaFicheiro(ficheiro);
    apagaFicheiro(erros);

    return dados;
}

//Função que cria um Nodo para a Hash Table de Aeroportos
Aeroporto* criaAeroporto(Voos* dados,char* nome) {
    Aeroporto* novo=malloc(sizeof(struct aeroporto_dados)); //criar nodo

    novo->nome=strdup(nome);
    novo->voos=inicia_ArrayDin(1);

    novo->mediana=-1;
    novo->mediana_calculada=false;
    novo->voos_ordenados=false;
    //inserir as informações

    return novo;
}

void apagaVoo(void* a);

//Função que coleta todos os dados dos passageiros, processa-os e preenche a hash table de voos e aeroportos
void iniciaPassageiros(const char* caminho_,Voos_Dados* Voo,User_Dados* Users) {
//   Result:  0 -> nao há voos  |  1 -> ha voos da pasta caminho   |  2 -> ha voos da pasta alternativa

    if (!caminho_ || !Voo || !Users) {//Caminho inválido ou dados necessarios inexistentes
        Voo->pass_resultado=0;
        return;
    }

    Voo->pass_resultado=1;

    // ################ Parser ##############

    char caminho[1000]; //cria caminho
    sprintf(caminho,"%s/passengers.csv",caminho_);

    Ficheiro* ficheiro=iniciaFicheiro(caminho,"r");
    Ficheiro* erros=iniciaFicheiro("Resultados/passengers_errors.csv","w+");

    if (!erros) {
        Voo->pass_resultado=0;
        apagaFicheiro(ficheiro);
        return;
    }

    if (ficheiro==NULL) {
        ficheiro=iniciaFicheiro("passengers.csv","r"); //Nao encontrou, procura na pasta default
        if (ficheiro) Voo->pass_resultado=2;
    }
    
    if (ficheiro==NULL) { //Dados nao encontrados
        apagaFicheiro(erros);
        Voo->pass_resultado=0;
        return;
    }

    // ################ Incorporação dos passageiros ####################

    int N_ficheiro=LinhasFicheiro(ficheiro);

    //Verifica se há overbooking
    for (int i=0;i<N_ficheiro;i++) {

        int j=0; //Numero do campo
        char* string=lerLinha(ficheiro); //Obtem a string
        string[strlen(string)-1]='\0'; //tira o '\n' da string

        if (i>0) {

        char* token=NULL;
        char* string_aux=string;

        char* passenger_user=NULL;
        char* passenger_flight=NULL;

        while(string) {          
              token=strsep(&string, ";");

                switch (j) {
                case 0: passenger_flight=strdup(token);
                break;

                case 1: passenger_user=strdup(token);
                break;
                
                default:
                    break;
                }

              j++;
          }

        //Verifica se o Voo e se o User existem
        if (getUserExiste(Users,passenger_user)==1 && getVooExiste(Voo,passenger_flight)==1) {

            Voos* voo=getHTItem(passenger_flight,Voo->Voos,&hashVoos);

            voo->n_passengers++;
            if (voo->n_passengers > voo->max_passengers) {
            remove_HashTable(passenger_flight,Voo->Voos,&hashVoos,&apagaVoo);
            }

        }

        free(string_aux);
        free(passenger_flight);
        free(passenger_user);

    }
    else free(string);

    }

    Voo->pass_fail=0;
    Voo->pass_lidos=0;
    Voo->pass_succ=0;

    rewindFicheiro(ficheiro); //Leitura do ficheiro vai começar no inicio

    //Adiciona voos aos users
    for (int i=0;i<N_ficheiro;i++) {

        int j=0; //Numero do campo
        char* string=lerLinha(ficheiro); //Obtem a string
        string[strlen(string)-1]='\0'; //tira o '\n' da string

        char* string_copia=strdup(string); //para o ficheiro de erros

        if (i==0) {  //mete a primeira linha nos passengers errors
            
            escreveLinha(erros,string_copia);
            free(string);

        }

        if (i>0) {

        char* token=NULL;
        char* string_aux=string;

        char* passenger_user=NULL;
        char* passenger_flight=NULL;

        while(string) {          
              token=strsep(&string, ";");

                switch (j) {
                case 0: passenger_flight=strdup(token);
                break;

                case 1: passenger_user=strdup(token);
                break;
                
                default:
                    break;
                }

              j++;
          }
/*
    printf("Passageiro:\n");
    printf("Voo Id: %s\n",passenger_flight);
    printf("User Id: %s\n",passenger_user);*/

        Voo->pass_lidos++;

    //Verifica se o Voo e se o User existem
    if (getUserExiste(Users,passenger_user)==1 && getVooExiste(Voo,passenger_flight)==1) {

        Voos* voo=getHTItem(passenger_flight,Voo->Voos,&hashVoos);
        inserir_ArrayDin(strdup(passenger_user),voo->passageiros);

        insereVooUsers(passenger_user,passenger_flight,Users);

        Voo->pass_succ++;

    }
    else { //Dado é invalido, deve ser excluido
        escreveLinha(erros,string_copia);
        Voo->pass_fail++;
    }

        free(string_aux);
        free(passenger_flight);
        free(passenger_user);

    }

    free(string_copia);

    }

    // ############ fim ###########

    //printHashTable(dados->reservas,&printReservas);
    //printHashTable(dados->hoteis,&printHotel);

    apagaFicheiro(ficheiro);
    apagaFicheiro(erros);

    return;
}

//Função que apaga um aeroporto
void apagaAeroporto (void* a) {
    Aeroporto* aero=a;
    free(aero->nome);
    apaga_ADStrings(aero->voos);
    free(aero);
}

//Função que apaga um voo
void apagaVoo(void* a) {
    Voos* voo=a;
    free(voo->plane_model);
    free(voo->origin);
    free(voo->destination);
    free(voo->schedule_arrival);
    free(voo->schedule_departure);
    free(voo->airline);
    free(voo->id);
    apaga_ADStrings(voo->passageiros);
    
    free(voo);
    return;
}

//Função que apaga as Hash Tables de Voos e Aeroportos
void apagaListaVoosAero(Voos_Dados* dados) {

    if (!dados) return;

    apagaHashTable(dados->Voos,&apagaVoo);
    apagaHashTable(dados->Aero,&apagaAeroporto);
    free(dados);
}


//Função que insere os voos nos users e aeroportos
void insereVoosAeroportos(Voos_Dados* Aero) {

    if (getHTN(Aero->Voos)<=0) {
        Aero->Aero=inicia_HashTable(1);
        return;
    }

    ArrayDin* novo=HashTable_to_AD(Aero->Voos);
    int N=getADN(novo);

    for (int i=0;i<N;i++) {

    char* voo_id=getADItem(novo,i);
    Voos* voo=getHTItem(voo_id,Aero->Voos,&hashVoos);

    //Aeroporto de partida
    int aero_existe=getAeroExiste(Aero,voo->origin);
    if (aero_existe==0) insere_HashTable(voo->origin,Aero->Aero,criaAeroporto(voo,voo->origin),&hashAero);
        
    insereVooAero(voo->origin,voo->id,Aero);

    //Aeroporto de chegada
    aero_existe=getAeroExiste(Aero,voo->destination);
    if (aero_existe==0) insere_HashTable(voo->destination,Aero->Aero,criaAeroporto(voo,voo->destination),&hashAero);

    //Caso os aeroportos de destino e chegada sejam os mesmo, não insere o voo    
    if(strcmp(voo->origin,voo->destination)!=0)  insereVooAero(voo->destination,voo->id,Aero);    

    }
    apaga_ADStrings(novo);

    return;
}

void printVoo(void* a) {
    Voos* voo=a;
    printf("%s",voo->id);
    return;
}

void printAero(void* a) {
    Aeroporto* aero=a;
    printf("%s %d %d",aero->nome,aero->mediana,getADN(aero->voos));
    /*
    printf(" voos: ");
    for (int i=0;i<getADN(aero->voos);i++) {
        char* a=getADItem(aero->voos,i);
        printf("%s ",a);
    }*/
}