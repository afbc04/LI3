#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ficheiros.h"
#include "testes.h"
#include "arraydin.h"

typedef struct ficheiro {
    FILE* ficheiro;
} Ficheiro;


// ##################################################################
//                      GETTERS
// ##################################################################

//Função que indica quantas linhas um ficheiro tem
int LinhasFicheiro(Ficheiro* fich) {

    if (!fich) return 0;

    int linhas=0;

    char* line= NULL;
    size_t read,len;
 
 while((read = getline(&line, &len,fich->ficheiro)) != -1) linhas++;
   free(line);

    rewind(fich->ficheiro); //retorna para o inicio do ficheiro

   return linhas;
}

//Função que escreve linhas num ficheiro
void escreveLinha(Ficheiro* fich,const char* string) {

    if (!fich) return;

    fprintf(fich->ficheiro,"%s\n",string);

} 

//Função que lê linha por linha
char* lerLinha(Ficheiro* fich) {

    if (!fich) return NULL;

    char string[1000];

    if (fgets(string, 1000, fich->ficheiro) != NULL) { //lè linha

        char* linha=string;
        return strdup(linha); //se ha linha, devolve essa linha
    } 
    else return NULL; //se nao ha linhas, devolve NULL
}

// ##################################################################
//                      SETTERS
// ##################################################################

//Função que dá rewind do ficheiro
void rewindFicheiro(Ficheiro* fich) {
    if (!fich) return;
    rewind(fich->ficheiro);
}

//Função que cria um ficheiro
Ficheiro* iniciaFicheiro(const char* nome,const char* modo) {
    FILE* ficheiro=fopen(nome,modo);
    if (!ficheiro) return NULL;

    Ficheiro* novo=malloc(sizeof(Ficheiro));
    novo->ficheiro=ficheiro;
    return novo;
}

//Função que apaga e fecha um ficheiro
void apagaFicheiro(Ficheiro* fich) {

    if (!fich) return;

    if (fich->ficheiro) fclose(fich->ficheiro);
    free(fich);
}


//Função que escreve um ficheiro a partir de um array
int escreveFicheiro(const char* nome_ficheiro, ArrayDin* lista) {

    FILE* ficheiro= fopen(nome_ficheiro, "w+");

    if (!ficheiro) return 0;
    else if (lista) {

        int N=getADN(lista);
        for (int i=0;i<N;i++) {
            char* linha=getADItem(lista,i);
            fprintf(ficheiro,"%s\n",linha);
        }
    }

    fclose(ficheiro);
    return 1;
}


/*
Parser genérico dos ficheiros.
Cada linha do ficheiro de entrada é guardada num array dinâmico
*/
ArrayDin* parser(const char* caminho)
{ 
  FILE *fp = fopen(caminho, "r"); 

  if(!fp) {
     return NULL;
  }
  char* line= NULL;
  size_t read;
  size_t len;
 
  ArrayDin* a=inicia_ArrayDin(1); // inicializar o array dinâmico

 while((read = getline(&line, &len,fp)) != -1) // ler cada linha do ficheiro
      {  
         inserir_ArrayDin(strdup(line),a); // inserir a linha no array
      }
   free(line);
   fclose(fp);
return a;
}

/*
Função que verifica se o conteúdo de um ficheiro é igual ao conteúdo de outro.
Verificar se os outputs obtidos são iguais aos outputs esperados. 
*/
int comparaFich(char* nome1, char* nome2,int c)
{  
    FILE* f1 = fopen(nome1,"r");

    if(!f1) {
     printf("Erro ao abrir ficheiro\n");
     return -1;
  }
    FILE* f2 = fopen(nome2,"r");

  if(!f2){
     printf("Erro ao abrir ficheiro\n");
     return -1;;  
  }
    
    int a;
    char* line1= NULL;
    char* line2 = NULL;
    size_t read1;
    size_t read2;
    size_t len1;
    size_t len2;
    int verifica=1;
    int h=1;
    c=c+1;
 while(((read1 = getline(&line1, &len1,f1)) != -1) && (read2 = getline(&line2, &len2,f2))) //verifica linha a linha
        {
          a=strcmp(line1,line2); // se for a=0 são iguais
          if(a != 0) {
                  verifica=0; 
                  break;
          }
          else {
            verifica = 1;
            }
            h++;
        }  
        if (verifica == 0) 
           {
             printf("Foi encontrada uma incongruência na linha %d do ficheiro de output %d.\n", h,c);
           } 
        else {
            printf("Não foram encontradas incongruências no ficheiro output %d\n",c);
        }
         free(line1);
         free(line2);
         fclose(f1);
         fclose(f2);
return c;
}