#include <locale.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "arraydin.h"
#include "hashtable.h"
#include "users.h"
#include "reservas.h"
#include "voos.h"
#include "datas.h"

#include <time.h>
#include <sys/resource.h>


#include "batch.h"
#include "ficheiros.h"

#include "queries.h" 
#include "testes.h"
/*
argumentos:
./programa-testes, caminho dataset, caminho ficheiro comandos, caminho pasta para output esperado
*/
int main(int n_arg, char** args) 
{
  if(n_arg == 4) {
      printf("A executar o programa de testes\n\n");
    }
  else {
      printf("O número de argumentos inseridos está incorreto.\nTente novamente!\n");
    }

setlocale(LC_COLLATE,"en_US.UTF-8"); //Para usar o strcoll

    //String para usarmos no terminal
   

    char* caminho_entrada=NULL; //string onde estao os ficheiros de entrada

    struct timespec start,end;
    double elapsed;
    struct rusage r_usage;

    clock_gettime(CLOCK_REALTIME,&start);


    if (n_arg==4) { 

    caminho_entrada=strdup(args[1]);
    
    }
    else { //Não devemos abrir o modo batch nem o interativo
    return 2;
    }

    // ###############################   LEITURA DE DADOS   ###########################

    if (n_arg==4) printf("1. Ler dados\n");


  // ############# USERS ###########

    //antes -> 8.72 sec  389 MB
    //depois -> 8.9 sec  358 MB
    
    int dados_lidos=0,dados_succ=0,dados_fail=0,result_leitura=0;

    if (n_arg==4) printf("    Lendo dados dos utilizadores...\n");

    User_Dados* Users=iniciaUsers(caminho_entrada);

    result_leitura=getUserDadosResultado(Users);
    dados_lidos=getUserDadosLidos(Users);
    dados_succ=getUserDadosSucc(Users);
    dados_fail=getUserDadosFail(Users);

    if (n_arg==4) {
      if (result_leitura>0) {
      if (result_leitura==1) printf("        Ficheiro \"%s/users.csv\" encontrado\n",caminho_entrada);
      if (result_leitura==2) printf("        Ficheiro \"%s/users.csv\" não existe\n        Ficheiro users.csv da pasta default foi lido\n",caminho_entrada);

      printf("            Total de dados lidos: %d\n            Total de dados incorporados: %d\n            Total de dados rejeitados: %d\n\n",dados_lidos,dados_succ,dados_fail);

      }
      else {
        printf("        Não foi lido nenhum ficheiro \"users.csv\" =(\n\n");
      }
    }

  // ############# RESERVAS ###########

    if (n_arg==4) printf("    Lendo dados das reservas...\n");

    Reservas_Dados* Reservas=iniciaReservas(caminho_entrada,Users);
    result_leitura=getReservaDadosResultado(Reservas);
    dados_lidos=getReservaDadosLidos(Reservas);
    dados_succ=getReservaDadosSucc(Reservas);
    dados_fail=getReservaDadosFail(Reservas);


    if (n_arg==4) {
      if (result_leitura>0) {
      if (result_leitura==1) printf("        Ficheiro \"%s/reservations.csv\" encontrado\n",caminho_entrada);
      if (result_leitura==2) printf("        Ficheiro \"%s/reservations.csv\" não existe\n        Ficheiro reservations.csv da pasta default foi lido\n",caminho_entrada);

      printf("            Total de dados lidos: %d\n            Total de dados incorporados: %d\n            Total de dados rejeitados: %d\n\n",dados_lidos,dados_succ,dados_fail);

      }
      else {
        printf("        Não foi lido nenhum ficheiro \"reservations.csv\" =(\n\n");
      }
    }

  // ############# Voos ###########

    if (n_arg==4) printf("    Lendo dados dos voos...\n");

    Voos_Dados* Voos=iniciaVoos(caminho_entrada);
    result_leitura=getVooDadosResultado(Voos);
    dados_lidos=getVooDadosLidos(Voos);
    dados_succ=getVooDadosSucc(Voos);
    dados_fail=getVooDadosFail(Voos);

    if (n_arg==4) {
      if (result_leitura>0) {
      if (result_leitura==1) printf("        Ficheiro \"%s/flights.csv\" encontrado\n",caminho_entrada);
      if (result_leitura==2) printf("        Ficheiro \"%s/flights.csv\" não existe\n        Ficheiro flights.csv da pasta default foi lido\n",caminho_entrada);

      printf("            Total de dados lidos: %d\n            Total de dados incorporados: %d\n            Total de dados rejeitados: %d\n\n",dados_lidos,dados_succ,dados_fail);

      }
      else {
        printf("        Não foi lido nenhum ficheiro \"flights.csv\" =(\n\n");
      }
    }


  // ############# Passageiros ###########

    if (n_arg==4) printf("    Lendo dados dos passageiros...\n");

    iniciaPassageiros(caminho_entrada,Voos,Users);
    result_leitura=getPassengersDadosResultado(Voos);
    dados_lidos=getPassengersDadosLidos(Voos);
    dados_succ=getPassengersDadosSucc(Voos);
    dados_fail=getPassengersDadosFail(Voos);

    if (n_arg==4) {
      if (result_leitura>0) {
      if (result_leitura==1) printf("        Ficheiro \"%s/passengers.csv\" encontrado\n",caminho_entrada);
      if (result_leitura==2) printf("        Ficheiro \"%s/passengers.csv\" não existe\n        Ficheiro passengers.csv da pasta default foi lido\n",caminho_entrada);

      printf("            Total de dados lidos: %d\n            Total de dados incorporados: %d\n            Total de dados rejeitados: %d\n\n",dados_lidos,dados_succ,dados_fail);

      }
      else {
        printf("        Não foi lido nenhum ficheiro \"passengers.csv\" =(\n\n");
      }
    }

    insereVoosAeroportos(Voos); //Insere voos nos aeroportos


  // ##################################################################### QUERIES ###################################################################3

    //printHashTable(Aero,&printAero);

    if (n_arg==4) {
    printf("2. Ler Queries...\n");
    bool testes=true;
    lerQueries(args[2],Users,Reservas,Voos,testes); //argumento - lista de queries
    }
    else {
      //Fase de teste
      int queries_interativo=5;
      while (queries_interativo>0) {

  printf("\n\nPode testar %d queries...\nInsira a query:\n\n",queries_interativo);
    char query_string[10000];
    fgets(query_string,10000,stdin);
    query_string[strlen(query_string)-1]='\0';

    ArrayDin* resultado=interpreterQueries(query_string,Users,Reservas,Voos,false);
    
    if (resultado) {

      printf("\n\nResultado:\n\n--------------------------------------------\n\n");

      int N_resultado_query=getADN(resultado);
      for(int i=0;i<N_resultado_query;i++) {
        char* a=getADItem(resultado,i);
        printf("%s\n",a);
      }

      printf("\n--------------------------------------------\n");

    apaga_ADStrings(resultado);

    }
    else {
      printf("\n\nQuery Invalida:\n\n");
    }
    queries_interativo--;
      }
    }
   
  // ##################################################################### LIMPEZA ###################################################################3

    apagaDadosUsers(Users);
    apagaListaReservasHoteis(Reservas);
    apagaListaVoosAero(Voos);
    if (caminho_entrada) free(caminho_entrada);

    if (n_arg==4) printf("\nPrograma executado com sucesso\nTenha um ótimo dia =)\n\nSofia Oliveira\nAndré Campos\nBeatriz Peixoto\n\n");

  verificacaoOutputs(args[3],args[2]);

  clock_gettime(CLOCK_REALTIME,&end);

  elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9; 
  printf("Tempo: %.6f seconds\n", elapsed);

  getrusage(RUSAGE_SELF,&r_usage);
  printf("Memoria Usada: %ld MB\n",r_usage.ru_maxrss/1000);
  
    return 0;
}
