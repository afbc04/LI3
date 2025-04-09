#ifndef QUERIES
#define QUERIES

#include "arraydin.h"
#include "hashtable.h"
#include <stdbool.h>

#include "users.h"
#include "reservas.h"
#include "voos.h"

ArrayDin* interpreterQueries(char* query,User_Dados* Users,Reservas_Dados* Reservas,Voos_Dados* Voos, bool batch);

#endif