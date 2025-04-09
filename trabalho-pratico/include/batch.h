#ifndef BATCH
#define BATCH

#include "arraydin.h"
#include "hashtable.h"
#include <stdbool.h>
#include "users.h"
#include "reservas.h"
#include "voos.h"

void lerQueries(const char* caminho,User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos,bool testes);

#endif