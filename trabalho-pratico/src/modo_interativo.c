#include "modo_interativo.h"

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "queries.h"

#include "users.h"
#include "voos.h"
#include "reservas.h"
#include "datas.h"


void imprimir_centro(char *texto){
    int x,y;
    getmaxyx(stdscr, y, x);

    int ycentro = y/2;
    int xcentro = x/2;

    clear();

    move(ycentro, xcentro-(strlen(texto)/2));
    printw("%s", texto);
}

void escreve_queries(int yquarter, int xquarter){
    for (int i=1; i<=10; i++) {
        move(yquarter + i + 1, xquarter - (strlen("Query x") / 2) + 2);
        clrtoeol();
        printw("Query %d", i);
        move(yquarter + i + 2,0);
        clrtoeol();
    }
}

void main_title();


void menu_title(){
    //funções iniciais Ncurses
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, FALSE);
    curs_set(0);

    imprimir_centro("Bem vindo ao modo interativo!");

    refresh();

    // getch();

    while (1) {
        int gch = getch();
        if (gch == KEY_RESIZE) {
            //lida com o resize do terminal
            endwin();
            refresh();
            initscr();
            noecho();

            imprimir_centro("Bem vindo ao modo interativo!");
        } 
        if(gch=='q'){
            endwin();
            exit(0);
        }
        else break;
    }

    endwin();
}


void highlight_query(int query_number, int yquarter, int xquarter) {
    
    char *q1 = "Listar o resumo de um utilizador, voo, ou reserva";
    char *q2 = "Listar os voos ou reservas de um utilizador";
    char *q3 = "Apresentar a classificação média de um hotel";
    char *q4 = "Listar as reservas de um hotel";
    char *q5 = "Listar os voos com origem num dado aeroporto, entre duas datas";
    char *q6 = "Listar o top N aeroportos com mais passageiros, para um dado ano";
    char *q7 = "Listar o top N aeroportos com a maior mediana de atrasos";
    char *q8 = "Apresentar a receita total de um hotel entre duas datas (inclusive)";
    char *q9 = "Listar todos os utilizadores cujo nome começa com o prefixo passado por argumento";
    char *q10 = "Apresentar várias métricas gerais da aplicação";

    attron(A_REVERSE);
    move(yquarter + query_number + 1, xquarter - (strlen("Query x") / 2) + 2);
    printw("Query %d: ", query_number);
    attroff(A_REVERSE);

    if(query_number==1){
        attron(A_REVERSE);
        printw("%s",q1);
        attroff(A_REVERSE);
    }
    if(query_number==2){
        attron(A_REVERSE);
        printw("%s",q2);
        attroff(A_REVERSE);
    }
    if(query_number==3){
        attron(A_REVERSE);
        printw("%s",q3);
        attroff(A_REVERSE);
    }
    if(query_number==4){
        attron(A_REVERSE);
        printw("%s",q4);
        attroff(A_REVERSE);
    }
    if(query_number==5){
        attron(A_REVERSE);
        printw("%s",q5);
        attroff(A_REVERSE);
    }
    if(query_number==6){
        attron(A_REVERSE);
        printw("%s",q6);
        attroff(A_REVERSE);
    }
    if(query_number==7){
        attron(A_REVERSE);
        printw("%s",q7);
        attroff(A_REVERSE);
    }
    if(query_number==8){
        attron(A_REVERSE);
        printw("%s",q8);
        attroff(A_REVERSE);
    }
    if(query_number==9){
        attron(A_REVERSE);
        printw("%s",q9);
        attroff(A_REVERSE);
    }
    if(query_number==10){
        attron(A_REVERSE);
        printw("%s",q10);
        attroff(A_REVERSE);
    }
    
}

void submenu_q1(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos);
void submenu_q2(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos);
void submenu_q3(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos);
void submenu_q4(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos);
void submenu_q5(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos);
void submenu_q6(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos);
void submenu_q7(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos);
void submenu_q8(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos);
void submenu_q9(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos);
void submenu_q10(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos);

void main_title(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){
    //initscr();
    //raw();
    //noecho();

    timeout(1);

    keypad(stdscr, TRUE);

    /*
    --------------------
    |                  |
    |     queries      |
    |                  |
    |                  |
    |                  |
    --------------------
    */
    
    clear();

    int x,y;
    getmaxyx(stdscr, y, x);
    int yquarter = y/4;
    int xquarter = x/4;

    int query_atual = 1;

    while(1){

        // clear();

        attron(A_BOLD);
        attron(A_UNDERLINE);
        char *txt = "Por favor selecione a query desejada:";
        move(yquarter, xquarter-(strlen(txt)/2));
        printw("%s", txt);
        attroff(A_BOLD);
        attroff(A_UNDERLINE);

        escreve_queries(yquarter, xquarter);
        
        highlight_query(query_atual, yquarter, xquarter);

        refresh();

        int ch = getch();

        switch(ch){
        case KEY_UP:
        case 'w':
            query_atual = ((query_atual -2+10) %10)+1;
            break;
        case KEY_DOWN:
        case 's':
            query_atual = (query_atual %10)+1;
            break;
        case 27:
        //Caso para Esc
            endwin();
            exit(0);
            break;
        //Introduzir case 10(Enter)
        case 10:
            if (query_atual == 1) {
                clear();
                submenu_q1(Users,Reservas,Voos);
                endwin();
                exit(0);
            }
            if (query_atual == 2) {
                clear();
                submenu_q2(Users,Reservas,Voos);
                endwin();
                exit(0);
            }
            if (query_atual == 3) {
                clear();
                submenu_q3(Users,Reservas,Voos);
                endwin();
                exit(0);
            }
            if (query_atual == 4) {
                clear();
                submenu_q4(Users,Reservas,Voos);
                endwin();
                exit(0);
            }
            if (query_atual == 5) {
                clear();
                submenu_q5(Users,Reservas,Voos);
                endwin();
                exit(0);
            }
            if (query_atual == 6) {
                clear();
                submenu_q6(Users,Reservas,Voos);
                endwin();
                exit(0);
            }
            if (query_atual == 7) {
                clear();
                submenu_q7(Users,Reservas,Voos);
                endwin();
                exit(0);
            }
            if (query_atual == 8) {
                clear();
                submenu_q8(Users,Reservas,Voos);
                endwin();
                exit(0);
            }
            if (query_atual == 9) {
                clear();
                submenu_q9(Users,Reservas,Voos);
                endwin();
                exit(0);
            }
            if (query_atual == 10) {
                clear();
                submenu_q10(Users,Reservas,Voos);
                endwin();
                exit(0);
            }
            break;
        default:
            break;
        }
        
    }

    refresh();
    getch();

    endwin();
}


int deseja_flag(WINDOW *winHalf, int ywin, int xwin){
    //Descobre se o user deseja colocar flag
    wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor indique se irá desejar ou não colocar flag")/2);
    wprintw(winHalf, "Por favor indique se irá desejar ou não colocar flag");
    wrefresh(winHalf);

    int deseja_flag;
    
    char *f = "Colocar flag";
    char *nf = "Não colocar flag";
    int pos_atual=1;

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor indique se irá desejar ou não colocar flag")/2);
    wattron(winHalf, A_REVERSE);
    wprintw(winHalf, "%s", f);
    wattroff(winHalf, A_REVERSE);
    wmove(winHalf, ywin/3+3, xwin/2 - strlen("Por favor indique se irá desejar ou não colocar flag")/2);
    wprintw(winHalf, "%s", nf);
    wrefresh(winHalf);

   int ch = '\0';
    while(1){
        ch = getch();
        if (ch == KEY_RESIZE) {
            //lida com o resize do terminal
            clear();
            wclear(winHalf);
            endwin();
            refresh();
            initscr();
            noecho();
            int x,y;
            getmaxyx(stdscr, y, x);

            mvprintw(y/8, 2, "Q1: Listar o resumo de um utilizador, voo, ou reserva, consoante o identificador recebido por argumento.");
            wresize(winHalf, y/2, x);
            mvwin(winHalf, y/2, 0);
            wborder(winHalf, '|', '|', '_', '_', '+', '+', '+', '+');
            wrefresh(winHalf);

            int ywin, xwin;
            getmaxyx(winHalf, ywin, xwin);

            wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor indique se irá desejar ou não colocar flag")/2);
            wprintw(winHalf, "Por favor indique se irá desejar ou não colocar flag");
            wrefresh(winHalf);
        }
        else if(ch=='q'){
            endwin();
            exit(0);
        } //rever e modificar
        else if(ch==10){
            wclear(winHalf);
            if(pos_atual==1) deseja_flag=1; //pressiona Enter em "Colocar flag"
            else if(pos_atual==2) deseja_flag=0; //pressiona Enter em "Não colocar flag"
            break;
        }
        else{
            wclear(winHalf);

            box(winHalf, 0, 0);
            wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
            wrefresh(winHalf);

            //Descobre se o user deseja colocar flag
            wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor indique se irá desejar ou não colocar flag")/2);
            wprintw(winHalf, "Por favor indique se irá desejar ou não colocar flag");
            wrefresh(winHalf);

            if(pos_atual==1 && (ch==KEY_UP || ch==KEY_DOWN)){
                pos_atual=2;
            } 
            else if(pos_atual==2 && (ch==KEY_UP || ch==KEY_DOWN)) {
                pos_atual=1;
            }


            if(pos_atual==1){
                wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor indique se irá desejar ou não colocar flag")/2);
                wattron(winHalf, A_REVERSE);
                wprintw(winHalf, "%s", f);
                wattroff(winHalf, A_REVERSE);
                wmove(winHalf, ywin/3+3, xwin/2 - strlen("Por favor indique se irá desejar ou não colocar flag")/2);
                wprintw(winHalf, "%s", nf);
            }
            if(pos_atual==2){
                wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor indique se irá desejar ou não colocar flag")/2);
                wprintw(winHalf, "%s", f);
                wmove(winHalf, ywin/3+3, xwin/2 - strlen("Por favor indique se irá desejar ou não colocar flag")/2);
                wattron(winHalf, A_REVERSE);
                wprintw(winHalf, "%s", nf);
                wattroff(winHalf, A_REVERSE);
            }
            wattroff(winHalf, A_REVERSE);
            wrefresh(winHalf);
        }

    }

    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    wmove(winHalf, ywin/2 , xwin/3);
    if(deseja_flag==1) wprintw(winHalf, "Escolheu colocar flag (%d)",deseja_flag);
    else if(deseja_flag==0) wprintw(winHalf, "Escolheu não colocar flag (%d)", deseja_flag);
    wrefresh(winHalf);

    getch();
    wclear(winHalf);

    return deseja_flag;
}


//###############################################################################################################################################
//###################################################################-Dataset-###################################################################
//###############################################################################################################################################

 char* pede_path(){
    initscr();
    curs_set(0);
    echo();
    cbreak();
    
    nodelay(stdscr, FALSE);

    int x,y;
    getmaxyx(stdscr, y, x);
    //int yquarter1 = y/4;
    //int xquarter1 = x/4;


    WINDOW *winpath = newwin(y, x, 0, 0); //Janela com caixa metade de baixo
    keypad(stdscr, true);
    box(winpath, 0, 0);
    wborder(winpath, '#', '#', '#', '#', '#', '#', '#', '#');
    wrefresh(winpath);

    char input_path[10000];
    char *dataset_path;

    wmove(winpath,y/2, x/2 - strlen("Por favor introduza o caminho para o dataset")/2);
    wprintw(winpath,"Por favor introduza o caminho para o dataset");
    wrefresh(winpath);

    wmove(winpath,y/2+1, x/2 - strlen("Por favor introduza o caminho para o dataset")/2);
    move(y/2+2, x/2 - strlen("Por favor introduza o caminho para o dataset")/2);
    scanw("%s", input_path);

    dataset_path = strdup(input_path);

    wmove(winpath,y/2+2, x/2 - strlen("Por favor introduza o caminho para o dataset")/2);
    wprintw(winpath,"-> %s", dataset_path);
    wrefresh(winpath);

    //getch();
    wclear(winpath);

    getch();
    //endwin();

    return dataset_path;
}

//###########################################################################################################################################
//#############################################################-OUTPUTS-#####################################################################
//###########################################################################################################################################

    void print_output(ArrayDin* resultado, int y, int x, User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){
        clear();
        //keypad(stdscr, true);
        while(1){
            
            //move(y/2, x/2 - strlen("Prima qualquer tecla para ver o resultado ou prima 'q' para sair")/2);
            //printw("Prima qualquer tecla para ver o resultado ou prima 'q' para sair");
            //refresh();

            int end_ch = getch();
            //clear();

            if(end_ch=='q'){
                endwin();
                exit(0);
            }
            
            if(end_ch==27){
                main_title(Users, Reservas, Voos);
            }

            move(1,x/2 - strlen("----Resultado(s):----")/2);
            printw("----Resultado(s):----");
            refresh();
            int N_resultado_query = getADN(resultado);
/*
            int count_lines(const char *str) {
                int lines = 0;
                while (*str) {
                    if (*str == '\n') {
                        lines++;
                    }
                    str++;
                }
                return lines;
            }*/

            if(N_resultado_query<y-2){
                for(int i=0; i<N_resultado_query; i++) {
                char* linha = getADItem(resultado,i);
                move(2+i, 1);
                printw("%s\n", linha);
                refresh();
                }
            }
            else{
                clear();

                int anon = 0;
                int anon_end = y-2;

                //int ch = -1;
                while(1){
                    clear();
                    /*
                    if(ch==KEY_DOWN){
                        if(anon_end < n_linhas_resultado){
                            anon += y-2;
                            anon_end += y-2;
                        }
                    }
                    if(ch==KEY_UP){
                        if(anon >= 0){
                            anon -= y-2;
                            anon_end -= y-2;
                        }
                    }*/

                    for(int k=anon; k<anon_end; k++) {
                        char* linha = getADItem(resultado,k);
                        move(2+k, 1);
                        printw("%s\n", linha);
                        refresh();
                    }
                    //int ch = getch();
                }
                
            }
        }
        
        
    }
/*
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
      }*/


//###########################################################################################################################################
//##########################################################-SUBMENU Q1-#####################################################################
//###########################################################################################################################################

void submenu_q1(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){

    cbreak();
    noecho();
    nodelay(stdscr, FALSE);

    int x,y;
    getmaxyx(stdscr, y, x);
    //int yquarter1 = y/4;
    //int xquarter1 = x/4;

    mvprintw(y/8, 2, "Q1: Listar o resumo de um utilizador, voo, ou reserva, consoante o identificador recebido por argumento.");
    refresh();
    
    WINDOW *winHalf = newwin(y/2, x, y/2, 0); //Janela com caixa metade de baixo
    keypad(stdscr, true);
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    //wrefresh(winHalf);

    int ywin, xwin;
    getmaxyx(winHalf, ywin, xwin);

    //#############################################-User Deseja Flag?-###########################################################

    int d_flag = deseja_flag(winHalf, ywin, xwin);

    //#############################################-Pedir ID-###########################################################
        
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char id[20];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ID correspondente ao elemento desejado")/2);
    wprintw(winHalf, "Por favor escreva o ID correspondente ao elemento desejado");
    wrefresh(winHalf);

    wmove(winHalf , ywin/3+2, xwin/2 - strlen("Por favor escreva o ID correspondente ao elemento desejado")/2);
    echo();
    wscanw(winHalf,"%19s", id);
/*
    //Caso de introduzir id inválido
    while(getVooExiste(Voos,id)!=1 && getUserExiste(Users,id)!=1 && getReservaExiste(Reservas,id)!=1){
        wclear(winHalf);

        box(winHalf, 0, 0);
        wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ID correspondente ao elemento desejado")/2);
        wprintw(winHalf, "Por favor escreva o ID correspondente ao elemento desejado");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva o ID correspondente ao elemento desejado")/2);
        wprintw(winHalf, "--Parece que introduziu um id inválido. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva o ID correspondente ao elemento desejado")/2);
        scanw("%s", id);
    }
*/
    noecho();
    
    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ID correspondente ao elemento desejado")/2);
    wprintw(winHalf, "-> %s", id);
    wrefresh(winHalf);

    getch();
    delwin(winHalf);
    clear();

    char query_string[100];

    if(d_flag==0) snprintf(query_string, sizeof(query_string), "1 %s", id);
    if(d_flag==1) snprintf(query_string, sizeof(query_string), "1F %s", id);

    ArrayDin* resultado = interpreterQueries(query_string, Users, Reservas, Voos, false);
    //move(y/2, x/2 -strlen("Função interpreterQueries done\n")/2);
    //printw("Função interpreterQueries done\n");

    //getch();
    print_output(resultado, y, x, Users, Reservas, Voos);

    //getch();
    //endwin();

}

//###########################################################################################################################################
//##########################################################-SUBMENU Q2-#####################################################################
//###########################################################################################################################################

void submenu_q2(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){
    cbreak();
    noecho();
    nodelay(stdscr, FALSE);

    int x,y;
    getmaxyx(stdscr, y, x);
    //int yquarter1 = y/4;
    //int xquarter1 = x/4;

    mvprintw(y/8, 2, "Q2: Listar os voos ou reservas de um utilizador ordenados por data (da mais recente para a mais antiga).");
    refresh();
    
    WINDOW *winHalf = newwin(y/2, x, y/2, 0); //Janela com caixa metade de baixo
    keypad(stdscr, true);
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    int ywin, xwin;
    getmaxyx(winHalf, ywin, xwin);

    //#############################################-User Deseja Flag?-###########################################################

    int d_flag = deseja_flag(winHalf, ywin, xwin);

    //#############################################-Pedir ID do User-###########################################################
    
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char id[30];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ID correspondente ao utilizador desejado")/2);
    wprintw(winHalf, "Por favor escreva o ID correspondente ao utilizador desejado");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ID correspondente ao utilizador desejado")/2);
    echo();
    wscanw(winHalf,"%s", id);

/*
    //Caso de introduzir id inválido
    while(getUserExiste(Users,id)!=1){
        wclear(winHalf);

        box(winHalf, 0, 0);
        wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ID correspondente ao utilizador desejado")/2);
        wprintw(winHalf, "Por favor escreva o ID correspondente ao utilizador desejado");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva o ID correspondente ao utilizador desejado")/2);
        wprintw(winHalf, "--Parece que introduziu um id inválido. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva o ID correspondente ao utilizador desejado")/2);
        scanw("%s", id);
    }
*/
    
    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ID correspondente ao utilizador desejado")/2);
    wprintw(winHalf, "-> %s", id);
    wrefresh(winHalf);

    getch();
    wclear(winHalf);

    //#############################################-Reservas, Voos, Ambos-#######################################################

    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);
    
    //Descobre se o user deseja ver voos, reservas ou ambos
    wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
    wprintw(winHalf, "Por favor indique se deseja ver voos, reservas ou ambos");
    wrefresh(winHalf);

    int vra;
    
    char *v = "Voos";
    char *r = "Reservas";
    char *a = "Voos e Reservas";
    int pos_atual_vra=1;

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
    wattron(winHalf, A_REVERSE);
    wprintw(winHalf, "%s", v);
    wattroff(winHalf, A_REVERSE);
    wmove(winHalf, ywin/3+3, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
    wprintw(winHalf, "%s", r);
    wmove(winHalf, ywin/3+4, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
    wprintw(winHalf, "%s", a);
    wrefresh(winHalf);

    while(1){
        int ch = getch();
        if (ch == KEY_RESIZE) {
            //lida com o resize do terminal
            clear();
            wclear(winHalf);
            endwin();
            refresh();
            initscr();
            noecho();
            int x,y;
            getmaxyx(stdscr, y, x);

            mvprintw(y/8, 2, "Q2: Listar os voos ou reservas de um utilizador ordenados por data (da mais recente para a mais antiga).");
            wresize(winHalf, y/2, x);
            mvwin(winHalf, y/2, 0);
            wborder(winHalf, '|', '|', '_', '_', '+', '+', '+', '+');
            wrefresh(winHalf);

            int ywin, xwin;
            getmaxyx(winHalf, ywin, xwin);

            wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
            wprintw(winHalf, "Por favor indique se deseja ver voos, reservas ou ambos");
            wrefresh(winHalf);
        }
        else if(ch=='q'){
            endwin();
            exit(0);
        } //rever e modificar
        else if(ch==10){
            wclear(winHalf);
            if(pos_atual_vra==1) vra=1; //pressiona Enter em "Voos"
            else if(pos_atual_vra==2) vra=2; //pressiona Enter em "Reservas"
            else if(pos_atual_vra==3) vra=3; //pressiona Enter em "Voos e Reservas"
            break;
        }
        else{
            wclear(winHalf);

            box(winHalf, 0, 0);
            wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
            wrefresh(winHalf);

            //Descobre se o user deseja ver voos, reservas ou ambos
            wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
            wprintw(winHalf, "Por favor indique se deseja ver voos, reservas ou ambos");
            wrefresh(winHalf);

            
            if(pos_atual_vra==1 && ch==KEY_UP) pos_atual_vra=3;
            else if(pos_atual_vra==3 && ch==KEY_DOWN) pos_atual_vra=1;
            else if(ch==KEY_DOWN) pos_atual_vra++;
            else if(ch==KEY_UP) pos_atual_vra--;


            if(pos_atual_vra==1){
                wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
                wattron(winHalf, A_REVERSE);
                wprintw(winHalf, "%s", v);
                wattroff(winHalf, A_REVERSE);
                wmove(winHalf, ywin/3+3, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
                wprintw(winHalf, "%s", r);
                wmove(winHalf, ywin/3+4, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
                wprintw(winHalf, "%s", a);
            }
            if(pos_atual_vra==2){
                wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
                wprintw(winHalf, "%s", v);
                wmove(winHalf, ywin/3+3, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
                wattron(winHalf, A_REVERSE);
                wprintw(winHalf, "%s", r);
                wattroff(winHalf, A_REVERSE);
                wmove(winHalf, ywin/3+4, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
                wprintw(winHalf, "%s", a);
            }
            if(pos_atual_vra==3){
                wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
                wprintw(winHalf, "%s", v);
                wmove(winHalf, ywin/3+3, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
                wprintw(winHalf, "%s", r);
                wmove(winHalf, ywin/3+4, xwin/2 - strlen("Por favor indique se deseja ver voos, reservas ou ambos")/2);
                wattron(winHalf, A_REVERSE);
                wprintw(winHalf, "%s", a);
                wattroff(winHalf, A_REVERSE);
            }
            //wattroff(winHalf, A_REVERSE);
            wrefresh(winHalf);
        }

    }

    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    wmove(winHalf, ywin/2 , xwin/3);
    if(vra==1) wprintw(winHalf, "Escolheu ver voos (%d)", vra);
    else if(vra==2) wprintw(winHalf, "Escolheu ver reservas (%d)", vra);
    else if(vra==3) wprintw(winHalf, "Escolheu ver voos e reservas (%d)", vra);
    wrefresh(winHalf);

    getch();
    delwin(winHalf);
    clear();

    char query_string[100];

    if(d_flag==0){
        if(vra==1) snprintf(query_string, sizeof(query_string), "2 %s flights", id);
        if(vra==2) snprintf(query_string, sizeof(query_string), "2 %s reservations", id);
        if(vra==3) snprintf(query_string, sizeof(query_string), "2 %s", id);  
    } 
    if(d_flag==1){
        if(vra==1) snprintf(query_string, sizeof(query_string), "2F %s flights", id);
        if(vra==2) snprintf(query_string, sizeof(query_string), "2F %s reservations", id);
        if(vra==3) snprintf(query_string, sizeof(query_string), "2F %s", id);
    }

    ArrayDin* resultado = interpreterQueries(query_string, Users, Reservas, Voos, false);
    //move(y/2, x/2 -strlen("Função interpreterQueries done\n")/2);
    //printw("Função interpreterQueries done\n");

    //getch();
    print_output(resultado, y, x, Users, Reservas, Voos);

    //getch();
    //endwin();
}

//####################################################################################################################
//#################################################-SUBMENU Q3-#######################################################
//####################################################################################################################

void submenu_q3(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){
    cbreak();
    noecho();
    nodelay(stdscr, FALSE);

    int x,y;
    getmaxyx(stdscr, y, x);
    //int yquarter1 = y/4;
    //int xquarter1 = x/4;

    mvprintw(y/8, 2, "Q3: Apresentar a classificação média de um hotel, a partir do seu identificador.");
    refresh();
    
    WINDOW *winHalf = newwin(y/2, x, y/2, 0); //Janela com caixa metade de baixo
    keypad(stdscr, true);
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    int ywin, xwin;
    getmaxyx(winHalf, ywin, xwin);

    //#############################################-User Deseja Flag?-###########################################################

    int d_flag = deseja_flag(winHalf, ywin, xwin);

    //#############################################-Pedir ID do Hotel-###########################################################

    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);
    
    char id[30];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
    wprintw(winHalf, "Por favor escreva o ID correspondente ao hotel desejado");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
    echo();
    wscanw(winHalf,"%s", id);
/*
//Caso de introduzir id inválido
    while(getHotelExiste(Reservas, id)!=1){
        wclear(winHalf);

        box(winHalf, 0, 0);
        wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
        wprintw(winHalf, "Por favor escreva o ID correspondente ao hotel desejado");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
        wprintw(winHalf, "--Parece que introduziu um id inválido. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
        scanw("%s", id);
    }
*/
    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
    wprintw(winHalf, "-> %s", id);
    wrefresh(winHalf);

    getch();
    delwin(winHalf);
    clear();

    char query_string[100];

    if(d_flag==0) snprintf(query_string, sizeof(query_string), "3 %s", id);
    if(d_flag==1) snprintf(query_string, sizeof(query_string), "3F %s", id);

    ArrayDin* resultado = interpreterQueries(query_string, Users, Reservas, Voos, false);
    //move(y/2, x/2 -strlen("Função interpreterQueries done\n")/2);
    //printw("Função interpreterQueries done\n");

    //getch();
    print_output(resultado, y, x, Users, Reservas, Voos);

    //getch();
    //endwin();

}

//####################################################################################################################
//#################################################-SUBMENU Q4-#######################################################
//####################################################################################################################


void submenu_q4(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){
    
    cbreak();
    noecho();
    nodelay(stdscr, FALSE);

    int x,y;
    getmaxyx(stdscr, y, x);
    //int yquarter1 = y/4;
    //int xquarter1 = x/4;

    mvprintw(y/8, 2, "Q4: Listar as reservas de um hotel, ordenadas por data de início (da mais recente para a mais antiga).");
    refresh();
    
    WINDOW *winHalf = newwin(y/2, x, y/2, 0); //Janela com caixa metade de baixo
    keypad(stdscr, true);
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    int ywin, xwin;
    getmaxyx(winHalf, ywin, xwin);

    //#############################################-User Deseja Flag?-###########################################################

    int d_flag = deseja_flag(winHalf, ywin, xwin);

    //#############################################-Pedir ID do hotel-###########################################################
    
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char id[30];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
    wprintw(winHalf, "Por favor escreva o ID correspondente ao hotel desejado");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
    echo();
    wscanw(winHalf,"%s", id);
/*
    //Caso de introduzir id inválido
    while(getHotelExiste(HT, id)!=1){
        wclear(winHalf);

        box(winHalf, 0, 0);
        wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
        wprintw(winHalf, "Por favor escreva o ID correspondente ao hotel desejado");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
        wprintw(winHalf, "--Parece que introduziu um id inválido. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
        scanw("%s", id);
    }
*/
    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
    wprintw(winHalf, "-> %s", id);
    wrefresh(winHalf);

    getch();
    delwin(winHalf);
    clear();

    char query_string[100];

    if(d_flag==0) snprintf(query_string, sizeof(query_string), "4 %s", id);
    if(d_flag==1) snprintf(query_string, sizeof(query_string), "4F %s", id);

    ArrayDin* resultado = interpreterQueries(query_string, Users, Reservas, Voos, false);
    //move(y/2, x/2 -strlen("Função interpreterQueries done\n")/2);
    //printw("Função interpreterQueries done\n");

    //getch();
    print_output(resultado, y, x, Users, Reservas, Voos);

    //getch();
    //endwin();
}

//####################################################################################################################
//#################################################-SUBMENU Q5-#######################################################
//####################################################################################################################

void submenu_q5(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){
    cbreak();
    noecho();
    nodelay(stdscr, FALSE);

    int x,y;
    getmaxyx(stdscr, y, x);
    //int yquarter1 = y/4;
    //int xquarter1 = x/4;

    mvprintw(y/8, 2, "Q5: Listar os voos com origem num dado aeroporto, entre duas datas, ordenados por data de partida estimada.");
    refresh();
    
    WINDOW *winHalf = newwin(y/2, x, y/2, 0); //Janela com caixa metade de baixo
    keypad(stdscr, true);
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    int ywin, xwin;
    getmaxyx(winHalf, ywin, xwin);

    //#############################################-User Deseja Flag?-###########################################################

    int d_flag = deseja_flag(winHalf, ywin, xwin);

    //#############################################-Nome do Aeroporto-###########################################################
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char nom_aero[6];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o nome do aeroporto desejado")/2);
    wprintw(winHalf, "Por favor escreva o nome do aeroporto desejado");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o nome do aeroporto desejado")/2);
    echo();
    wscanw(winHalf,"%s", nom_aero);

/*
    //Caso de introduzir id inválido
    while(getHotelExiste(HT, nom_aero)!=1){
        wclear(winHalf);

        box(winHalf, 0, 0);
        wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o nome do aeroporto desejado")/2);
        wprintw(winHalf, "Por favor escreva o nome do aeroporto desejado");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva o nome do aeroporto desejado")/2);
        wprintw(winHalf, "--Parece que introduziu um nome inválido. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva o nome do aeroporto desejado")/2);
        scanw("%s", nom_aero);
    }
*/
    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o nome do aeroporto desejado")/2);
    wprintw(winHalf, "-> %s", nom_aero);
    wrefresh(winHalf);

    getch();
    wclear(winHalf);

    //#############################################-Data inicial-###########################################################
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char q5_di[30];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd hh:mm:ss)")/2);
    wprintw(winHalf, "Por favor escreva a data inicial (no formato aaaa/mm/dd hh:mm:ss)");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd hh:mm:ss)")/2);
    echo();
    wscanw(winHalf,"%s", q5_di);

/*
    //Caso de introduzir data inválida
    while(dia_validation(q5_di)!=1){
        wclear(winHalf);

        box(winHalf, 0, 0);
        wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd)")/2);
        wprintw(winHalf, "Por favor escreva a data inicial (no formato aaaa/mm/dd)");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd)")/2);
        wprintw(winHalf, "--Parece que introduziu uma data inválida. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd)")/2);
        scanw("%s", q5_di);
    }
*/
    
    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd hh:mm:ss)")/2);
    wprintw(winHalf, "-> %s", q5_di);
    wrefresh(winHalf);

    getch();
    wclear(winHalf);

    //#############################################-Data final-###########################################################
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char q5_df[30];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd hh:mm:ss)")/2);
    wprintw(winHalf, "Por favor escreva a data final (no formato aaaa/mm/dd hh:mm:ss)");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd hh:mm:ss)")/2);
    echo();
    wscanw(winHalf,"%s", q5_df);

/*
    //Caso de introduzir data inválida
    while(dia_validation(q5_df)!=1){
        wclear(winHalf);

        box(winHalf, 0, 0);
        wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd)")/2);
        wprintw(winHalf, "Por favor escreva a data final (no formato aaaa/mm/dd)");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd)")/2);
        wprintw(winHalf, "--Parece que introduziu uma data inválida. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd)")/2);
        scanw("%s", q5_df);
    }
*/
    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd hh:mm:ss)")/2);
    wprintw(winHalf, "-> %s", q5_df);
    wrefresh(winHalf);

    getch();
    delwin(winHalf);
    clear();

    char query_string[100];

    if(d_flag==0) snprintf(query_string, sizeof(query_string), "5 %s \"%s\" \"%s\"", nom_aero, q5_di, q5_df);
    if(d_flag==1) snprintf(query_string, sizeof(query_string), "5F %s \"%s\" \"%s\"", nom_aero, q5_di, q5_df);

    ArrayDin* resultado = interpreterQueries(query_string, Users, Reservas, Voos, false);
    //move(y/2, x/2 -strlen("Função interpreterQueries done\n")/2);
    //printw("Função interpreterQueries done\n");

    //getch();
    print_output(resultado, y, x, Users, Reservas, Voos);

    //getch();
    //endwin();
}

//####################################################################################################################
//#################################################-SUBMENU Q6-#######################################################
//####################################################################################################################

void submenu_q6(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){
    cbreak();
    noecho();
    nodelay(stdscr, FALSE);

    int x,y;
    getmaxyx(stdscr, y, x);
    //int yquarter1 = y/4;
    //int xquarter1 = x/4;

    mvprintw(y/8, 2, "Q6: Listar o top N aeroportos com mais passageiros, para um dado ano.");
    refresh();
    
    WINDOW *winHalf = newwin(y/2, x, y/2, 0); //Janela com caixa metade de baixo
    keypad(stdscr, true);
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    int ywin, xwin;
    getmaxyx(winHalf, ywin, xwin);

    //#############################################-User Deseja Flag?-###########################################################

    int d_flag = deseja_flag(winHalf, ywin, xwin);

    //#############################################-Ano-###########################################################
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char q6_ano[5];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ano desejado")/2);
    wprintw(winHalf, "Por favor escreva o ano desejado");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ano desejado")/2);
    echo();
    wscanw(winHalf,"%s", q6_ano);

    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ano desejado")/2);
    wprintw(winHalf, "-> %s", q6_ano);
    wrefresh(winHalf);

    getch();
    wclear(winHalf);

    //#############################################-Top N-###########################################################
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char q6_N[5];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva qual o N correspondente top N desejado")/2);
    wprintw(winHalf, "Por favor escreva qual o N correspondente top N desejado");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva qual o N correspondente top N desejado")/2);
    echo();
    wscanw(winHalf,"%s", q6_N);

    /*
    while(q6_N<1){
        wclear(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva qual o N correspondente ao top N desejado")/2);
        wprintw(winHalf, "Por favor escreva qual o N correspondente ao top N desejado");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva qual o N correspondente ao top N desejado")/2);
        wprintw(winHalf, "--Parece que introduziu um número igual ou menor que 0. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva qual o N correspondente ao top N desejado")/2);
        scanw("%d", q6_N);
    }
    */

   noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva qual o N correspondente top N desejado")/2);
    wprintw(winHalf, "-> %s", q6_N);
    wrefresh(winHalf);

    getch();
    delwin(winHalf);
    clear();

    char query_string[100];

    if(d_flag==0) snprintf(query_string, sizeof(query_string), "6 %s %s", q6_ano, q6_N);
    if(d_flag==1) snprintf(query_string, sizeof(query_string), "6F %s %s", q6_ano, q6_N);

    ArrayDin* resultado = interpreterQueries(query_string, Users, Reservas, Voos, false);
    //move(y/2, x/2 -strlen("Função interpreterQueries done\n")/2);
    //printw("Função interpreterQueries done\n");

    //getch();
    print_output(resultado, y, x, Users, Reservas, Voos);

    //getch();
    //endwin();
}

//####################################################################################################################
//#################################################-SUBMENU Q7-#######################################################
//####################################################################################################################

void submenu_q7(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){
    cbreak();
    noecho();
    nodelay(stdscr, FALSE);

    int x,y;
    getmaxyx(stdscr, y, x);
    //int yquarter1 = y/4;
    //int xquarter1 = x/4;

    mvprintw(y/8, 2, "Q7: Listar o top N aeroportos com a maior mediana de atrasos.");
    refresh();
    
    WINDOW *winHalf = newwin(y/2, x, y/2, 0); //Janela com caixa metade de baixo
    keypad(stdscr, true);
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    int ywin, xwin;
    getmaxyx(winHalf, ywin, xwin);

    //#############################################-User Deseja Flag?-###########################################################

    int d_flag = deseja_flag(winHalf, ywin, xwin);

    //#############################################-Top N-###########################################################
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char q7_N[5];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva qual o N correspondente ao top N desejado")/2);
    wprintw(winHalf, "Por favor escreva qual o N correspondente ao top N desejado");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva qual o N correspondente ao top N desejado")/2);
    echo();
    wscanw(winHalf,"%s", q7_N);

    /*
    while(q6_N<1){
        wclear(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva qual o N correspondente ao top N desejado")/2);
        wprintw(winHalf, "Por favor escreva qual o N correspondente ao top N desejado");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva qual o N correspondente ao top N desejado")/2);
        wprintw(winHalf, "--Parece que introduziu um número igual ou menor que 0. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva qual o N correspondente ao top N desejado")/2);
        scanw("%d", q6_N);
    }
    */

    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva qual o N correspondente ao top N desejado")/2);
    wprintw(winHalf, "-> %s", q7_N);
    wrefresh(winHalf);

    getch();
    delwin(winHalf);
    clear();

    char query_string[100];

    if(d_flag==0) snprintf(query_string, sizeof(query_string), "7 %s", q7_N);
    if(d_flag==1) snprintf(query_string, sizeof(query_string), "7F %s", q7_N);

    ArrayDin* resultado = interpreterQueries(query_string, Users, Reservas, Voos, false);
    //move(y/2, x/2 -strlen("Função interpreterQueries done\n")/2);
    //printw("Função interpreterQueries done\n");

    //getch();
    print_output(resultado, y, x, Users, Reservas, Voos);

    //getch();
    //endwin();

}

//####################################################################################################################
//#################################################-SUBMENU Q8-#######################################################
//####################################################################################################################

void submenu_q8(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){
    cbreak();
    noecho();
    nodelay(stdscr, FALSE);

    int x,y;
    getmaxyx(stdscr, y, x);
    //int yquarter1 = y/4;
    //int xquarter1 = x/4;

    mvprintw(y/8, 2, "Q8: Apresentar a receita total de um hotel entre duas datas (inclusive), a partir do seu identificador.");
    refresh();
    
    WINDOW *winHalf = newwin(y/2, x, y/2, 0); //Janela com caixa metade de baixo
    keypad(stdscr, true);
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    int ywin, xwin;
    getmaxyx(winHalf, ywin, xwin);

    //#############################################-User Deseja Flag?-###########################################################

    int d_flag = deseja_flag(winHalf, ywin, xwin);

    //#############################################-Pedir ID do Hotel-###########################################################
    
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char id[30];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
    wprintw(winHalf, "Por favor escreva o ID correspondente ao hotel desejado");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
    echo();
    wscanw(winHalf,"%s", id);

/*
    //Caso de introduzir data inválida
    while(getHotelExiste(HT, id)!=1){
        wclear(winHalf);

        box(winHalf, 0, 0);
        wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
        wprintw(winHalf, "Por favor escreva o ID correspondente ao hotel desejado");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
        wprintw(winHalf, "--Parece que introduziu um id inválido. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
        scanw("%s", id);
    }
*/
    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ID correspondente ao hotel desejado")/2);
    wprintw(winHalf, "-> %s", id);
    wrefresh(winHalf);

    getch();
    wclear(winHalf);

    //#############################################-Data inicial-###########################################################
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char q8_di[30];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd)")/2);
    wprintw(winHalf, "Por favor escreva a data inicial (no formato aaaa/mm/dd)");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd)")/2);
    echo();
    wscanw(winHalf,"%s", q8_di);

/*
    //Caso de introduzir data inválida
    while(dia_validation(q8_di)!=1){
        wclear(winHalf);

        box(winHalf, 0, 0);
        wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd)")/2);
        wprintw(winHalf, "Por favor escreva a data inicial (no formato aaaa/mm/dd)");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd)")/2);
        wprintw(winHalf, "--Parece que introduziu uma data inválida. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd)")/2);
        scanw("%s", id);
    }
*/
    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva a data inicial (no formato aaaa/mm/dd)")/2);
    wprintw(winHalf, "-> %s", q8_di);
    wrefresh(winHalf);

    getch();
    wclear(winHalf);

    //#############################################-Data final-###########################################################
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char q8_df[30];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd)")/2);
    wprintw(winHalf, "Por favor escreva a data final (no formato aaaa/mm/dd)");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd)")/2);
    echo();
    wscanw(winHalf,"%s", q8_df);

/*
    //Caso de introduzir data inválida
    while(dia_validation(q8_df)!=1){
        wclear(winHalf);

        box(winHalf, 0, 0);
        wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(winHalf);

        wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd)")/2);
        wprintw(winHalf, "Por favor escreva a data final (no formato aaaa/mm/dd)");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd)")/2);
        wprintw(winHalf, "--Parece que introduziu uma data inválida. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd)")/2);
        scanw("%s", id);
    }
*/
    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva a data final (no formato aaaa/mm/dd)")/2);
    wprintw(winHalf, "-> %s", q8_df);
    wrefresh(winHalf);

    getch();
    delwin(winHalf);
    clear();

    char query_string[100];

    if(d_flag==0) snprintf(query_string, sizeof(query_string), "8 %s %s %s", id, q8_di, q8_df);
    if(d_flag==1) snprintf(query_string, sizeof(query_string), "8F %s %s %s", id, q8_di, q8_df);

    ArrayDin* resultado = interpreterQueries(query_string, Users, Reservas, Voos, false);
    //move(y/2, x/2 -strlen("Função interpreterQueries done\n")/2);
    //printw("Função interpreterQueries done\n");

    //getch();
    print_output(resultado, y, x, Users, Reservas, Voos);

    //getch();
    //endwin();
}

//####################################################################################################################
//#################################################-SUBMENU Q9-#######################################################
//####################################################################################################################

void submenu_q9(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){
    cbreak();
    noecho();
    nodelay(stdscr, FALSE);

    int x,y;
    getmaxyx(stdscr, y, x);
    //int yquarter1 = y/4;
    //int xquarter1 = x/4;

    mvprintw(y/8, 2, "Q9: Listar todos os utilizadores cujo nome começa com o prefixo passado por argumento.");
    refresh();
    
    WINDOW *winHalf = newwin(y/2, x, y/2, 0); //Janela com caixa metade de baixo
    keypad(stdscr, true);
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    int ywin, xwin;
    getmaxyx(winHalf, ywin, xwin);

    //#############################################-User Deseja Flag?-###########################################################

    int d_flag = deseja_flag(winHalf, ywin, xwin);

    //#############################################-Prefixo-###########################################################
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char q9_pref[20];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o prefixo desejado")/2);
    wprintw(winHalf, "Por favor escreva o prefixo desejado");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o prefixo desejado")/2);
    echo();
    wscanw(winHalf,"%s", q9_pref);

    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o prefixo desejado")/2);
    wprintw(winHalf, "-> %s", q9_pref);
    wrefresh(winHalf);

    getch();
    delwin(winHalf);
    clear();

    char query_string[100];

    if(d_flag==0) snprintf(query_string, sizeof(query_string), "9 %s", q9_pref);
    if(d_flag==1) snprintf(query_string, sizeof(query_string), "9F %s", q9_pref);

    ArrayDin* resultado = interpreterQueries(query_string, Users, Reservas, Voos, false);
    //move(y/2, x/2 -strlen("Função interpreterQueries done\n")/2);
    //printw("Função interpreterQueries done\n");

    //getch();
    print_output(resultado, y, x, Users, Reservas, Voos);

    //getch();
    //endwin();
}

//####################################################################################################################
//#################################################-SUBMENU Q10-#######################################################
//####################################################################################################################

void submenu_q10(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos){
    cbreak();
    noecho();
    nodelay(stdscr, FALSE);

    int x,y;
    getmaxyx(stdscr, y, x);
    //int yquarter1 = y/4;
    //int xquarter1 = x/4;

    mvprintw(y/8, 2, "Q10: Apresentar várias métricas gerais da aplicação.");
    refresh();
    
    WINDOW *winHalf = newwin(y/2, x, y/2, 0); //Janela com caixa metade de baixo
    keypad(stdscr, true);
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    int ywin, xwin;
    getmaxyx(winHalf, ywin, xwin);

    //#############################################-User Deseja Flag?-###########################################################

    int d_flag = deseja_flag(winHalf, ywin, xwin);

    //#############################################-Ano-###########################################################
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char q10_ano[5];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o ano desejado ou pressione Enter")/2);
    wprintw(winHalf, "Por favor escreva o ano desejado ou pressione Enter");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ano desejado ou pressione Enter")/2);
    echo();
    wscanw(winHalf,"%s", q10_ano);

    noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o ano desejado ou pressione Enter")/2);
    wprintw(winHalf, "-> %s", q10_ano);
    wrefresh(winHalf);

    getch();
    wclear(winHalf);

    //#############################################-Mês-###########################################################
    box(winHalf, 0, 0);
    wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(winHalf);

    char q10_mes[3];

    wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o mês desejado ou pressione Enter")/2);
    wprintw(winHalf, "Por favor escreva o mês desejado ou pressione Enter");
    wrefresh(winHalf);

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o mês desejado ou pressione Enter")/2);
    echo();
    wscanw(winHalf,"%s", q10_mes);

    /*
    //Caso de introduzir mês inválida
    while(q10_mes>12 || q10_mes<1){
        wclear(winHalf);

        box(winHalf, 0, 0);
        wborder(winHalf, '|', '|', '-', '-', '+', '+', '+', '+');
        wrefresh(winHalf);

         wmove(winHalf, ywin/3-1, xwin/2 - strlen("Por favor escreva o mês desejado ou pressione Enter")/2);
        wprintw(winHalf, "Por favor escreva o mês desejado ou pressione Enter");
        wrefresh(winHalf);

        wmove(winHalf, ywin/3, xwin/2 - strlen("Por favor escreva o mês desejado ou pressione Enter")/2);
        wprintw(winHalf, "--Parece que introduziu um mês inválido. Por favor tente de novo--");
        wmove(winHalf, ywin/3+1, xwin/2 - strlen("Por favor escreva o mês desejado ou pressione Enter")/2);
        scanw("%d", &q10_mes);
    }
    */
   noecho();

    wmove(winHalf, ywin/3+2, xwin/2 - strlen("Por favor escreva o mês desejado ou pressione Enter")/2);
    wprintw(winHalf, "-> %s", q10_mes);
    wrefresh(winHalf);

    getch();
    delwin(winHalf);
    clear();

    char query_string[100];

    if(d_flag==0){
        if(q10_ano!='\0'){
            if(q10_mes!='\0') snprintf(query_string, sizeof(query_string), "10 %s %s", q10_ano, q10_mes);
            else snprintf(query_string, sizeof(query_string), "10 %s", q10_ano);
        }
        else snprintf(query_string, sizeof(query_string), "10");
    } 
    if(d_flag==1){
        if(q10_ano!='\0'){
            if(q10_mes!='\0') snprintf(query_string, sizeof(query_string), "10F %s %s", q10_ano, q10_mes);
            else snprintf(query_string, sizeof(query_string), "10F %s", q10_ano);
        }
        else snprintf(query_string, sizeof(query_string), "10F");
    }

    ArrayDin* resultado = interpreterQueries(query_string, Users, Reservas, Voos, false);
    //move(y/2, x/2 -strlen("Função interpreterQueries done\n")/2);
    //printw("Função interpreterQueries done\n");

    //getch();
    print_output(resultado, y, x, Users, Reservas, Voos);

    //getch();
    //endwin();
}
