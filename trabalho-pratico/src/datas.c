#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "datas.h"

typedef struct dia {
    short year;
    short month;
    short day;
} Dia;

typedef struct hora {
    short hour;
    short minute;
    short second;
} Hora;

typedef struct data {
    struct dia* dia;
    struct hora* hora;
} Data;

Data* criaData(const char* string);
void apagaData(void* a) ;
int calculaDiasDatas(const char* data1_, const char* data2_);

// ##################################################################
//                      GETTERS
// ##################################################################

//Obtem Ano da Data
short getDataAno(const char* string) {
    short res=-1;
    Data* data=criaData(string);
    if (data && data->dia) res=data->dia->year;
    apagaData(data);
    return res;
}

//Obtem Mes da Data
short getDataMes(const char* string) {
    short res=-1;
    Data* data=criaData(string);
    if (data && data->dia) res=data->dia->month;
    apagaData(data);
    return res;
}

//Obtem Dia da Data
short getDataDia(const char* string) {
    short res=-1;
    Data* data=criaData(string);
    if (data && data->dia) res=data->dia->day;
    apagaData(data);
    return res;
}

//Obtem Hora da Data
short getDataHora(const char* string) {
    short res=-1;
    Data* data=criaData(string);
    if (data && data->hora) res=data->hora->hour;
    apagaData(data);
    return res;
}

//Obtem Minuto da Data
short getDataMinuto(const char* string) {
    short res=-1;
    Data* data=criaData(string);
    if (data && data->hora) res=data->hora->minute;
    apagaData(data);
    return res;
}

//Obtem Segundos da Data
short getDataSegundo(const char* string) {
    short res=-1;
    Data* data=criaData(string);
    if (data && data->hora) res=data->hora->second;
    apagaData(data);
    return res;
}

//Função que indica se uma data esta entre as datas referenciadas
int entreDatas(const char* inicio, const char* fim, const char* inicio_ref, const char* fim_ref) {
//  entre_datas:  true ->  toda a duração da data  |  false -> a data apanha a referencia, mas pode estar fora dela tambem

    if (comparaDatas(inicio,inicio_ref)>=0 && comparaDatas(fim,fim_ref)<=0) return 1;
    else return 0;

}

//Função que obtem o número de noites numa noite
int getNoitesDatas(const char* inicio, const char* fim, const char* inicio_ref, const char* fim_ref) {

    short res_inicio=comparaDatas(inicio,inicio_ref);
    short res_fim=comparaDatas(fim,fim_ref);

    if (comparaDatas(fim,inicio_ref)<1 || comparaDatas(fim_ref,inicio)==-1) return 0; //datas não se intersetam

    if (res_inicio==-1 && res_fim==1) return calculaDiasDatas(inicio_ref,fim_ref)+1; //referencia dentro da data
    if (res_inicio==-1 && res_fim==0) return calculaDiasDatas(inicio_ref,fim_ref); //referencia apanha toda a parte direita da data
    if (res_inicio==-1 && res_fim==-1) return calculaDiasDatas(inicio_ref,fim); //metade das datas se cruzam

    if (res_inicio==0 && res_fim==1) return calculaDiasDatas(inicio,fim_ref)+1; //referencia apanha toda a parte esquerda da data
    if (res_inicio==0 && res_fim==0) return calculaDiasDatas(inicio,fim); //referencia == data
    if (res_inicio==0 && res_fim==-1) return calculaDiasDatas(inicio,fim); //referencia apanha toda a data

    if (res_inicio==1 && res_fim==1) return calculaDiasDatas(inicio,fim_ref)+1; //metade das datas se cruzam
    if (res_inicio==1 && res_fim==0) return calculaDiasDatas(inicio,fim_ref); //referencia apanha toda a data
    if (res_inicio==1 && res_fim==-1) return calculaDiasDatas(inicio,fim); //data está entre a referencia
    
    return 0;
}

// ##################################################################
//                      FUNÇÕES
// ##################################################################

//Função que cria um dia
Dia* criaDia(short y, short m, short d) {
    Dia* novo=malloc(sizeof(struct dia));
    novo->year=y;
    novo->month=m;
    novo->day=d;

    return novo;
}

//Função que cria uma hora
Hora* criaHora(short h, short m, short s) {
    Hora* novo=malloc(sizeof(struct hora));
    novo->hour=h;
    novo->minute=m;
    novo->second=s;

    return novo;
}

//Função que junta dia e hora formando uma data
Data* juntaDatas(Dia* dia, Hora* hora) {
    Data* nova=malloc(sizeof(struct data));
    nova->dia=dia;
    nova->hora=hora;

    return nova;
}


//Função que apaga um dia
void apagaDia(void* a) {
    Dia* dia=a;
    if (dia) free(dia);
}

//Função que apaga uma hora
void apagaHora(void* a) {
    Hora* hora=a;
    if (hora) free(hora);
}

//Função que apaga uma data
void apagaData(void* a) {
    Data* data=a;

    if (!data) return;

    if (data->dia) apagaDia(data->dia);
    if (data->hora) apagaHora(data->hora);
    free(data);
    return;
}


//Função que pega numa  string do tipo "23/10/2000" e separa em 3 ints 
Dia* quebraDia(const char* data)
{ 
    Dia* dia=criaDia(0,0,0);

    char* token=NULL;
    char strings[3][5];

    for (int i=0;i<3;i++) {
        for (int j=0;j<5;j++)
            strings[i][j]='0';
    }

    int i=0;

    char* copia=strdup(data);
    char* copia2=copia;
    while( (token = strsep(&copia2,"/")) != NULL && i<3)
        strcpy(strings[i++],token);

          dia->year=(short) atoi(strings[0]); //atoi converte string para int- biblioteca stdlib
          dia->month=(short) atoi(strings[1]);
          dia->day=(short) atoi(strings[2]);
        free(copia);
    

         return dia;
}

//Função que pega numa  string do tipo "14:45:37" e separa em 3 ints
Hora* quebraHora(const char* hora_){
    
    Hora* hora=criaHora(0,0,0);

    char* token=NULL;
    char strings[3][5];
    int i=0;

    char*hora_aux=strdup(hora_);
    char* hora_aux2=hora_aux;

    while((token = strsep(&hora_aux2,":"))!=NULL && i<3)
    {
        strcpy(strings[i],token);
        i++;
    }

        hora->hour=(short) atoi(strings[0]);
        hora->minute=(short) atoi(strings[1]);
        hora->second=(short) atoi(strings[2]);
        free(hora_aux);
    return hora;
}

//Pega numa string com dia e hora e transforma numa data
Data* quebraData(const char* data_) {

    char* token1=NULL;
    char* hours1 = strdup(data_);
    token1 = strsep(&hours1," ");

    Dia* dia = quebraDia(token1);
    Hora* hora = quebraHora(hours1);

    Data* data=juntaDatas(dia,hora);
    
    free(token1);
    return data;
}

//Função que a partir de uma string, cria uma data
Data* criaData(const char* string) {

    int tam=strlen(string);

    if (tam==10) { //Só Dia
        Dia* dia=quebraDia(string);
        Hora* hora=NULL;
        return juntaDatas(dia,hora);
    }
    else if (tam==8) { //Só Hora
        Dia* dia=NULL;
        Hora* hora=quebraHora(string);
        return juntaDatas(dia,hora);
    }
    else if (tam==19) { //Dia e hora
        Data* data=quebraData(string);
        return data;
    }

    return NULL;

} 

int calculaAtraso(const char* a_, const char* b_) {

    Data* a=criaData(a_);
    Data* b=criaData(b_);

    long delay_real=a->dia->year*32140800+a->dia->month*2678400+a->dia->day*86400+a->hora->hour*3600+a->hora->minute*60+a->hora->second;
    long delay_planeado=b->dia->year*32140800+b->dia->month*2678400+b->dia->day*86400+b->hora->hour*3600+b->hora->minute*60+b->hora->second;
    
    int delay=delay_real-delay_planeado;

    apagaData(a);
    apagaData(b);

    if (delay>0) return delay;
    else return (-1)*delay;
}

//Função que calcula a idade de uma pessoa usando uma data de referencia
int calculaIdade(const char* pessoa_, const char* referencia_) {

    Data* pessoa=criaData(pessoa_);
    Data* referencia=criaData(referencia_);

    int idade=0;
    if (pessoa->dia && referencia->dia) {

    idade=referencia->dia->year-pessoa->dia->year;
    if (pessoa->dia->month>=referencia->dia->month) {
        if (pessoa->dia->month>referencia->dia->month) idade--;
        if (pessoa->dia->month==referencia->dia->month && pessoa->dia->day>referencia->dia->day) idade--;
    }
    }

    apagaData(pessoa);
    apagaData(referencia);

    return idade;
}

//Função que compara dias
short comparaDias(Dia* a, Dia* b) {
 
    if (a->year < b->year) return -1;
    else if (a->year > b->year) return 1;
    else if (a->year == b->year) { //Mesmo ano

        if (a->month < b->month) return -1;
        else if (a->month > b->month) return 1;
        else if (a->month == b->month) { //Mesmo mês
            if (a->day < b->day) return -1;
            else if (a->day > b->day) return 1;
            else return 0; //Dias iguais
        }
    }
    return 0;
}

//Função que compara horas
short comparaHoras(Hora* a, Hora* b) {
 
    if (a->hour < b->hour) return -1;
    else if (a->hour > b->hour) return 1;
    else if (a->hour == b->hour) { //Mesma hora

        if (a->minute < b->minute) return -1;
        else if (a->minute > b->minute) return 1;
        else if (a->minute == b->minute) { //Mesmo minuto
            if (a->second < b->second) return -1;
            else if (a->second > b->second) return 1;
            else return 0; //Horas iguais
        }
    }
    return 0;
}

//Função que compara datas
short comparaDatas(const char* a_, const char* b_) {
    short res=0;

    Data* a=criaData(a_);
    Data* b=criaData(b_);

    if (a->dia && b->dia) { //Dias existem
        res=comparaDias(a->dia,b->dia);
        if (res==0) {//mesmo dias
            if (a->hora && b->hora) {
                res=comparaHoras(a->hora,b->hora);
            }
        }
    }
    
    apagaData(a);
    apagaData(b);

    return res;
}

//Função que calcula a diferença de dias entre datas
int calculaDiasDatas(const char* data1_, const char* data2_)
{
  Data* data1_data=criaData(data1_);
  Data* data2_data=criaData(data2_);
  int dias; 

  if (data1_data->dia && data2_data->dia) {

    Dia* data1=data1_data->dia;
    Dia* data2=data2_data->dia;
   if(comparaDias(data1, data2) == 1) // caso a data de inicio seja maior que a data de fim
    {
      dias = 1;  // número de noites é 1
    }
   else 
    {
      if((data1->year == data2->year) && (data1->month == data2->month)) // datas de inicio e fim no mesmo ano e mesmo mês
        {
          dias = (data2->day - data1->day);
        }
      else if((data1->year == data2->year) && (data1->month != data2->month)) // mesmo ano mas mês diferente
         {
          dias = (data2->month-data1->month)*31 + (data2->day-data1->day); 
         }
      else 
      {
        if(data1->day == 1 && data1->month == 1) // inicio do ano(primeiro dia do ano), caso particular
        {
           dias = (((data2->year-data1->year)+1)*365) - ((31-data2->day) + (12-(data2->month))*31); 
        }
        else 
        { 
        /* 
        considera o número de dias totais nos anos envolvidos e retira os dias que vão desde o dia 01/01 até à data de início 
        e tira também o número de dias desde a data final até ao dia 31/12 do ano da data final
        */
          dias = ((data2->year-data1->year)+1)*365 - (((data1->day-1) + ((data1->month) -1)*31) +((31-data2->day) + ((12-(data2->month))*31)));
      }
    }
  }
  }
  else {
    dias=0;
  }
  apagaData(data1_data);
  apagaData(data2_data);

  return dias;
}




#define Data_Tamanho 10

//Validação de Dias
int dia_validation(char* data_aux){

    int tamanho=strlen(data_aux);

    if (tamanho!=10) return 0;

    Dia* data_aux_q_ = quebraDia(data_aux);
    Dia data_aux_q=*data_aux_q_;
    free(data_aux_q_);


    //O formato deverá ser sempre do tipo nnnn/nn/nn, onde n é um número entre 0 e 9 (inclusivo).
    int barras=0;
    for(int i=0;i<Data_Tamanho;i++){
        if(data_aux[i]=='/') barras++;
        if (isdigit(data_aux[i])==0 && data_aux[i]!='/'){
            //printf("Inválido: Erro na data (caracter diferente de digito e barra)\n");
            return 0;
        }
    }
    if(barras!=2){
        //printf("Inválido: Erro na data (barras a mais ou a menos)\n");
        return 0;
    }
    if(data_aux[4]!='/' || data_aux[7]!='/'){
        //printf("Inválido: Erro na data (barras não se encontram nas posições corretas)\n");
        return 0;
    }

    //O mês deverá estar entre 1 e 12 (inclusivo) e o dia entre 1 e 31 (inclusivo).
    if(data_aux_q.month > 12 || data_aux_q.month<1 || data_aux_q.day > 31 || data_aux_q.day<1 || data_aux_q.year>2030 || data_aux_q.year<1900){
        //printf("Inválido: Erro na birth_date (mês>12 ou dia>31)\n");
        return 0;
    }

    return 1;
}

#define DataHoraTamanho 8

//Validação de Horas
int hora_validation(char* hora_aux){

    int tamanho=strlen(hora_aux);
    if (tamanho!=8) return 0;

    Hora* hora_aux_q_ = quebraHora(hora_aux);
    Hora hora_aux_q=*hora_aux_q_;
    free(hora_aux_q_);

    //O formato deverá ser sempre do tipo nn:nn:nn, onde n é um número entre 0 e 9 (inclusivo).
    int _2pontos=0;
    for(int i=0;i<DataHoraTamanho;i++){
        if(hora_aux[i]==':') _2pontos++;
        if (isdigit(hora_aux[i])==0 && hora_aux[i]!=':'){
            //printf("Inválido: Erro na hora (caracter diferente de digito e 2 pontos)\n");
            return 0;
        }
    }
    if(_2pontos!=2){
        //printf("Inválido: Erro na hora (2 pontos a mais ou a menos)\n");
        return 0;
    }
    if(hora_aux[2]!=':' || hora_aux[5]!=':'){
        //printf("Inválido: Erro na hora (os 2 pontos não se encontram nas posições corretas)\n");
        return 0;
    }

    //A hora deverá estar entre 0 e 23 (inclusivo), os minutos entre 0 e 59 (inclusivo), e os segundos entre 0 e 59 (inclusivo)
    if(hora_aux_q.hour > 23 || hora_aux_q.hour < 0 || hora_aux_q.minute > 59 || hora_aux_q.minute < 0 || hora_aux_q.second > 59 || hora_aux_q.second < 0){
        //printf("Inválido: Erro na birth_date (mês>12 ou dia>31)\n");
        return 0;
    }

    return 1;
}

int data_time_validation(char* dado_) {

    int valid=1;

    int tamanho=strlen(dado_);
    if (tamanho!=19) return 0;

    char*dado=strdup(dado_);
    char* dado2=dado;

    char* token1 = strsep(&dado, " ");
    char* token2 = strsep(&dado, " ");
    if (dia_validation(token1)==0) {
        valid=0;
    }
    else {
        valid=hora_validation(token2);
    }
    free(dado2);
    return valid;
}
