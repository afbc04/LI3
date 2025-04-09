#include <stdio.h>    
#include <string.h>  
#include <stdlib.h>

#include "testes.h"
#include "arraydin.h"
#include "ficheiros.h"


// Função que retorna o nome do ficheiro
char* auxNomeOutput(char* str, int tamanho)
{   
    char *s1 = malloc(tamanho); 
    strcpy(s1, "command");
    strcat(s1, str);
    strcat(s1, "_output.txt");
return s1;
}

/*
Função que guarda num array dinâmico, o caminho e os nomes dos ficheiros de output ou resultados
*/
void nomeOutput(ArrayDin* a,char* s,int t)
{
    int tam=0;
    
    for(int i=1;i<=t;i++)    //o i corresponde ao X em "commandX_output.txt"
    {  
      if(i>0 && i<10) tam = 1;   //obter o tamanho do X(pode ser 1 dígito, 2 ou 3)
      if(i>=10 && i<100) tam = 2;
      else tam = 3;
    
      int tamanho=18+tam+1;   //tamanho total do nome "caminhoX_output.txt"
      char* str=malloc(tam+1); 
      sprintf(str,"%d",i);   //armazenar em str o inteiro correspondente a i;
      char* s1 = auxNomeOutput(str,tamanho); // obtem-se o nome "caminhoX_output.txt"

      char path[1000]; // tamanho do caminho total no máximo
      strcpy(path,s);
      strcat(path,s1);
      inserir_ArrayDin(strdup(path),a); // inserir o caminho e o nome do ficheiro no array dinâmico

      free(str);
      free(s1);
    }
}


void verificacaoOutputs(char* caminho_output,char* fich_input) //caminho_output-resultados dos professores
    { 
     ArrayDin* a1;
     ArrayDin* a2; 
     int c=0;
     int t;
     int l;
     
    Ficheiro* f = iniciaFicheiro(fich_input,"r");
    l=LinhasFicheiro(f); // calcula o número de linhas do ficheiro input
 
    t=l;

    a1 = inicia_ArrayDin(1);
    a2 = inicia_ArrayDin(1);

    char* caminho_resultados = malloc(12);
    strcpy (caminho_resultados,"Resultados/");
    char* s=malloc(2);
    strcpy(s,"/");
    strcat(caminho_output,s);

    nomeOutput(a1,caminho_output,t);//esperados
    nomeOutput(a2,caminho_resultados,t); //real 
       for(int i=0;i<t;i++)
         {  
            char* caminho_a1 = getADItem(a1,i); // tira o caminho para os outputs esperados

            char* caminho_a2 = getADItem(a2,i); // tira o caminho para os resultados obtidos
           
           c= comparaFich(caminho_a1,caminho_a2,c);
         } 
  
        free(caminho_resultados);
        free(s);
        apaga_ADStrings(a1);
        apaga_ADStrings(a2);
        apagaFicheiro(f);
    }
    