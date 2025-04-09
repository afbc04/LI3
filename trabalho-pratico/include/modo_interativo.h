#ifndef MODO_INTERATIVO_H
#define MODO_INTERATIVO_H

#include "users.h"
#include "reservas.h"
#include "voos.h"

void menu_title();
void main_title(User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos);
char* pede_path();


#endif