#include <locale.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "arraydin.h"
#include "hashtable.h"
#include "datas.h"

#include "voos.h"
#include "reservas.h"
#include "users.h"

#include "ficheiros.h"

//Estrutura de Dados para os pre-users
typedef struct pre_dados_users {
    char* user_id;
    char* user_name;
    char* user_email;
    char* user_pNumber;
    char* user_birthDate;
    char* user_sex;
    char* user_passaport;
    char* user_countryCode;
    char* user_adress;
    char* user_accountCreation;
    char* user_metodoPagamento;
    char* user_accountState;
} Pre_Users;

//Estrutura de Dados do array para users
typedef struct dados_users {
    char* user_id; //Id do user
    char* name;  //Nome do user
    char* sex; //Genero
    char age; //Idade do user (char pode ser visto como um int com 1 byte)
    char* country_code; //Código do pais
    char* passport; //Passaporte
    double total_spent; //Total gasto
    ArrayDin* reservations; //Lista com as suas reservas
    ArrayDin* flights; //Lista com os seus voos
    char* account_status; //Indica o estado da conta
    char* creation; //Indica a data de quando o user foi criado
    bool unico_passageiro; //Util para a query 10
} Users;

//Struct com todos os users guardados
typedef struct users {
    HashTable HT; //Hash Table dos Users
    char resultado; //Indica o resultado da leitura
    int lidos; //Indica quantos dados foram lidos
    int succ; //Indica quantos dados foram incorporados
    int fail; //Indica quantos dados foram rejeitados
} User_Dados;

// ##################################################################
//                      GETTERS
// ##################################################################

//Função Hash para a HashTable de Users
int hashFuncUsers(const char* s, int N) {
    unsigned long a=0;
    int b=0;
    int N_=strlen(s);
    for (int i=0;i<N_;i++) {
        b=abs(s[i]);
        a = ((a << 5) + a) + b;
    }

    int res=a%N;
    return res;
}

//Indica se o user é ativo
int UserAtivo(User_Dados* HT, const char* s) {

    if(getUserExiste(HT,s)==0) return 0;

    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    
    char* verifica=strdup(user->account_status); //copia o parametro "account_status" para verificar se o user está ativo

    int res=strcasecmp(verifica,"INACTIVE");
    free(verifica);

   if (res==0) return 0;  //Se o user é inativo, a query é feita mas não é imprimido nada
   return 1;
}

//Obtem Id do User
char* getUserId(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    return strdup(user->user_id);
}

//Obtem Nome do User
char* getUserName(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    return strdup(user->name);
}

//Obtem Genero do User
char* getUserSex(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    return strdup(user->sex);
}

//Obtem Idade do User
int getUserAge(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    return (int) user->age;
}

//Obtem Country Code do User
char* getUserCountryCode(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    return strdup(user->country_code);
}

//Obtem Passaporte do User
char* getUserPassport(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    return strdup(user->passport);
}

//Obtem Status do User
char* getUserStatus(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    return strdup(user->account_status);
}

//Obtem Account Creation do User
char* getUserCreation(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    return strdup(user->creation);
}

//Obtem Total Spent do User
double getUserTotalSpent(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    return (double) user->total_spent;
}

//Obtem Numero de Reservas do User
int getUserNumeroReservas(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    return (int) getADN(user->reservations);
}

//Obtem Numero de Voos do User
int getUserNumeroVoos(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    return (int) getADN(user->flights);
}

//Função que indica se um user existe
int getUserExiste(User_Dados* HT, const char* s) {
    void* user=procura_HashTable(s,HT->HT,&hashFuncUsers);
    if (user) return 1;
    else return 0;
}

//Obtem Reservas do User
ArrayDin* getUserReservas(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    
    ArrayDin* reservas=inicia_ArrayDin(1);

    int N=getADN(user->reservations);

    for (int i=0;i<N;i++) {
        inserir_ArrayDin(strdup(getADItem(user->reservations,i)),reservas);
    }

    return reservas;
}

//Obtem Voos do User
ArrayDin* getUserVoos(User_Dados* HT, const char* s) {
    Users* user= getHTItem(s,HT->HT,&hashFuncUsers);
    
    ArrayDin* voos=inicia_ArrayDin(1);

    int N=getADN(user->flights);

    for (int i=0;i<N;i++) {
        inserir_ArrayDin(strdup(getADItem(user->flights,i)),voos);
    }

    return voos;
}

//Obtem Array dinamico de todos os users
ArrayDin* getUserLista(User_Dados* HT) {

    return HashTable_to_AD(HT->HT);
}

void getUsersUniquePassengersAux(void* a, void* b) {
    Users* user=a;
    int* n=b;
    if(user->unico_passageiro==true) (*n)++;
}

//Função que coloca a opção passageiros unicos como falso
int getUsersUniquePassengers(User_Dados* dados) {
    int passageiros=0;
    applyHashTable(dados->HT,&getUsersUniquePassengersAux,&passageiros);
    return passageiros;
}

//Obter o resultado do User Dados
int getUserDadosResultado(User_Dados* lista) {
    return lista->resultado;
}

//Obter os dados lidos do User Dados
int getUserDadosLidos(User_Dados* lista) {
    return lista->lidos;
}

//Obter os dados incorporados do User Dados
int getUserDadosSucc(User_Dados* lista) {
    return lista->succ;
}

//Obter os dados rejeitados do User Dados
int getUserDadosFail(User_Dados* lista) {
    return lista->fail;
}

// ##################################################################
//                      SETTERS
// ##################################################################


//Função que insere uma reserva na lista de reservas dos users
void insereReservaUsers(const char* user_, const char* id, User_Dados* HTU) {

    if(getUserExiste(HTU,user_)==0) return; //User não existe

    Users* user=getHTItem(user_,HTU->HT,&hashFuncUsers);
    char* reserva_id=strdup(id);
    inserir_ArrayDin(reserva_id,user->reservations);
}

//Função que insere um voo na lista de voos dos users
void insereVooUsers(const char* user_, const char* id, User_Dados* HTU) {

    if(getUserExiste(HTU,user_)==0) return; //User não existe

    Users* user=getHTItem(user_,HTU->HT,&hashFuncUsers);
    char* voo_id=strdup(id);
    inserir_ArrayDin(voo_id,user->flights);
}

//Aumenta o valor total_spent de um user
void addTotalSpentUser(const char* user_,User_Dados* HTU, double valor) {

    if(getUserExiste(HTU,user_)==0) return; //User não existe

    Users* user=getHTItem(user_,HTU->HT,&hashFuncUsers);
    user->total_spent+=valor;
    return;
}

void resetUsersUniquePassengersAux(void* a, void* b) {
    Users* user=a;
    user->unico_passageiro=false;
}

//Função que coloca a opção passageiros unicos como falso
void resetUsersUniquePassengers(User_Dados* dados) {
    applyHashTable(dados->HT,&resetUsersUniquePassengersAux,NULL);
}

//Função que define o campo "unico passageiro" como true
int setUserUniquePassenger(User_Dados* dados, const char* nome) {
//Resultados:  -1 -> nao existe | 0 -> ja esta true  |  1 -> foi definido
    if(getUserExiste(dados,nome)==0) return -1; //User não existe

    Users* user= getHTItem(nome,dados->HT,&hashFuncUsers);

    if (user->unico_passageiro==true) return 0;
    else {
        user->unico_passageiro=true;
        return 1;
    }

}

// ##################################################################
//                      FUNÇÕES
// ##################################################################

//Função que verifica se um Pre-User é valido
int user_validation(Pre_Users users){
    //user começa a ser considerado válido

        if (users.user_id==NULL) return 0;
        if (users.user_name==NULL) return 0;
        if (users.user_email==NULL) return 0;
        if (users.user_pNumber==NULL) return 0;
        if (users.user_birthDate==NULL) return 0;
        if (users.user_sex==NULL) return 0;
        if (users.user_passaport==NULL) return 0;
        if (users.user_countryCode==NULL) return 0;
        if (users.user_adress==NULL) return 0;
        if (users.user_accountCreation==NULL) return 0;
        if (users.user_metodoPagamento==NULL) return 0;
        if (users.user_accountState==NULL) return 0;

        //O country_code de um utilizador deverá ser formado por duas letras;
        if(strlen(users.user_countryCode)!=2){
            return 0;
            }

        //O account_status de um utilizador deverá ter o valor “active” ou “inactive”, sendo que diferentes combinações de maiúsculas e minúsculas também são válidas (e.g., “Active”, “aCtive”,e “INACTIVE” também são válidos);
        if(strcasecmp(users.user_accountState,"active")!=0 && strcasecmp(users.user_accountState,"inactive")!=0){
            return 0;
           }

        //Os seguintes restantes campos têm que ter tamanho superior a zero: Utilizador: id, name, phone_number, sex, passport, address, pay_method;
        if(strlen(users.user_id)==0 || strlen(users.user_name)==0 || strlen(users.user_pNumber)==0 || strlen(users.user_sex)==0 || strlen(users.user_passaport)==0 || strlen(users.user_adress)==0 || strlen(users.user_metodoPagamento)==0){
            return 0;
            }
            int valid=1;

        //Verificação das datas
        if(dia_validation(users.user_birthDate)==0) return 0;
        if(data_time_validation(users.user_accountCreation)==0) return 0;

        //Nos utilizadores, o birth_date tem que vir antes de account_creation;
    short comparaDatasShort=comparaDatas(users.user_birthDate,"2023/10/01");
    if (comparaDatasShort>=0) valid=0;
    comparaDatasShort=comparaDatas(users.user_birthDate,users.user_accountCreation);
    if (comparaDatasShort>=0) valid=0;

    if (valid==0) return 0;

        //O email de um utilizador tem que ter o seguinte formato: “<username>@<domain>.<TLD>”.O <username> e o <domain> têm que ter pelo menos tamanho 1; O <TLD> tem que ter pelomenos tamanho 2. Exemplo de erros: @email.com, john@.pt, john@email.a, john@email,pt,john.email.pt, . . . ;
        char* user_email=strdup(users.user_email);
        char* user_email2=user_email;

        char* username = strsep(&user_email, "@");
        char* domain = strsep(&user_email, ".");

        if(!username || !domain || !user_email || strlen(username)==0 || strlen(domain)==0 || strlen(user_email)<2){
            valid=0;
            }

        free(user_email2);

    return valid;
}

// Cria um 'Users' com base nos dados trazidos pelo parser dos users
Users* PreUser_para_User(Pre_Users dados) {
        Users* novo=malloc(sizeof(Users));
        
            novo->user_id=strdup(dados.user_id);
            novo->name=strdup(dados.user_name);
            novo->sex=strdup(dados.user_sex);

            novo->age=calculaIdade(dados.user_birthDate,"2023/10/01");

            novo->passport=strdup(dados.user_passaport);
            novo->total_spent=0;
            novo->country_code = strdup(dados.user_countryCode);
            novo->reservations=inicia_ArrayDin(1);
            novo->flights=inicia_ArrayDin(1);
            novo->account_status=strdup(dados.user_accountState);

            char* criacao_or=strdup(dados.user_accountCreation);
            char* criacao=malloc(11);
            for (int i=0;i<10;i++) criacao[i]=criacao_or[i];
            criacao[10]='\0';

            novo->creation=criacao;
            free(criacao_or);

            novo->unico_passageiro=false;

        return novo;
}

//Função que coleta todos os dados dos users, processa-os e preenche a hash table de users
User_Dados* iniciaUsers(const char* caminho_) {
//   Result:  0 -> nao há users  |  1 -> ha users da pasta caminho   |  2 -> ha users da pasta alternativa

    User_Dados* dados=malloc(sizeof(User_Dados)); //criação dos dados do user
    dados->lidos=0;
    dados->fail=0;
    dados->succ=0;

    if (!caminho_) {//Caminho inválido
        dados->resultado=0;
        dados->HT=inicia_HashTable(1);
        return dados;
    }

    dados->resultado=1;

    // ################ Parser ##############

    char caminho[1000]; //cria caminho
    sprintf(caminho,"%s/users.csv",caminho_);

    Ficheiro* ficheiro=iniciaFicheiro(caminho,"r");
    Ficheiro* erros=iniciaFicheiro("Resultados/users_errors.csv","w+");
    
    if (!erros) {
        apagaFicheiro(ficheiro);
        dados->resultado=0;
        dados->HT=inicia_HashTable(1);
        return dados;
    }

    if (ficheiro==NULL) {
        ficheiro=iniciaFicheiro("users.csv","r"); //Nao encontrou, procura na pasta default
        if (ficheiro) dados->resultado=2;
    }
    
    if (ficheiro==NULL) { //Dados nao encontrados
        apagaFicheiro(erros);
        dados->resultado=0;
        dados->HT=inicia_HashTable(1);
        return dados;
    }

    // ################ Incorporação dos Users ####################

    int N_ficheiro=LinhasFicheiro(ficheiro);

    dados->HT=inicia_HashTable(N_ficheiro-1); //inicia a hash table
    
    for (int i=0;i<N_ficheiro;i++) {
        int j=0; //Numero do campo
        char* string=lerLinha(ficheiro); //Obtem a string
        string[strlen(string)-1]='\0'; //tira o '\n' da string

        char* string_copia=strdup(string); //para o ficheiro de erros
    if (i==0) { //mete a primeira linha nos users errors

        escreveLinha(erros,string_copia);
        free(string);

        }

    if (i>0) {

        //Criação do Pre-User
        Pre_Users pre_user;

        char* token=NULL;
        char* string_aux=string;

        while(string) {          
              token=strsep(&string, ";");

                switch (j) {
                case 0: pre_user.user_id=strdup(token);
                break;

                case 1: pre_user.user_name=strdup(token);
                break;

                case 2: pre_user.user_email=strdup(token);
                break;

                case 3: pre_user.user_pNumber=strdup(token);
                break;

                case 4: pre_user.user_birthDate=strdup(token);
                break;

                case 5: pre_user.user_sex=strdup(token);
                break;

                case 6: pre_user.user_passaport=strdup(token);
                break;

                case 7: pre_user.user_countryCode=strdup(token);
                break;

                case 8: pre_user.user_adress=strdup(token);
                break;

                case 9: pre_user.user_accountCreation=strdup(token);
                break;

                case 10: pre_user.user_metodoPagamento=strdup(token);
                break;

                case 11: pre_user.user_accountState=strdup(token);
                break;
                
                default:
                    break;
                }

              j++;
          }

/*
    printf("\nPre-User:\n");
    printf("Id: %s\n",pre_user.user_id);
    printf("Nome: %s\n",pre_user.user_name);
    printf("Email: %s\n",pre_user.user_email);
    printf("Numero Tele: %s\n",pre_user.user_pNumber);
    printf("BDay: %s\n",pre_user.user_birthDate);
    printf("Sex: %s\n",pre_user.user_sex);
    printf("Passport: %s\n",pre_user.user_passaport);
    printf("CC: %s\n",pre_user.user_countryCode);
    printf("Adress: %s\n",pre_user.user_adress);
    printf("Creation: %s\n",pre_user.user_accountCreation);
    printf("Pagamento: %s\n",pre_user.user_metodoPagamento);
    printf("Estado: %s\n",pre_user.user_accountState);*/

        dados->lidos++;

        //if (1) {
        if (user_validation(pre_user)==1) { //Dado é valido, Deve ser incorporado
        
            Users* user=PreUser_para_User(pre_user);
            insere_HashTable(user->user_id,dados->HT,user,&hashFuncUsers);

            dados->succ++;
        }
        else { //Dado é invalido, deve ser excluido
        escreveLinha(erros,string_copia);
        dados->fail++;
        }

    free(string_aux);

    free(pre_user.user_id);
    free(pre_user.user_name);
    free(pre_user.user_email);
    free(pre_user.user_pNumber);
    free(pre_user.user_birthDate);
    free(pre_user.user_sex);
    free(pre_user.user_passaport);
    free(pre_user.user_countryCode);
    free(pre_user.user_adress);
    free(pre_user.user_accountCreation);
    free(pre_user.user_metodoPagamento);
    free(pre_user.user_accountState);

    }

    free(string_copia);

    }

    // ############ fim ###########

    apagaFicheiro(ficheiro);
    apagaFicheiro(erros);

    return dados;
}

//Funçaõ que apaga o array dinamico dos Users
void apaga_Users(void* user_) {
    Users* user=user_;
        free(user->user_id);
        free(user->name);
        free(user->sex);
        free(user->country_code);
        free(user->passport);
        free(user->account_status);
        free(user->creation);
        apaga_ADStrings(user->flights);
        apaga_ADStrings(user->reservations);
        free(user);
    return;
}

//Função que apaga a Hash Table de Users
void apagaDadosUsers(User_Dados* lista) {
    if (!lista) return;
    apagaHashTable(lista->HT,&apaga_Users);
    free(lista);
}

//Função que imprime um User (só para testes)
void printUser(void* a_) {
    Users* a=a_;
    printf("%s; %s; %s; %d; %s; %f; %s; %s; %s (%d %d)",a->user_id,a->name,a->sex,a->age,a->passport,a->total_spent,a->country_code,a->account_status,a->creation,getADN(a->reservations),getADN(a->flights));
    printf("reservas: ");
    for (int i=0;i<getADN(a->reservations);i++) {
        char* b=getADItem(a->reservations,i);
        printf("%s ",b);
    }
}

void printPreUser(void* a_) {
    Pre_Users* a=a_;
    printf("%s",a->user_id);
}

//Função que imprime os dados dos Users
void printUsers(User_Dados* User, short modo) {
//  modo:  0 -> tudo  |  1 -> só users   |  2 -> user_q9  

    if (!User) return;

    if (modo!=2) printHashTable(User->HT,&printUser);
}