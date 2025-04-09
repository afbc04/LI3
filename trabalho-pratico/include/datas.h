#ifndef DATAS
#define DATAS

//Getters
short getDataAno(const char* string);
short getDataMes(const char* string);
short getDataDia(const char* string);
short getDataHora(const char* string);
short getDataMinuto(const char* string);
short getDataSegundo(const char* string);

int calculaIdade(const char* pessoa_, const char* referencia_);
int dia_validation(char* data_aux);
int hora_validation(char* hora_aux);
int data_time_validation(char* dado_);
short comparaDatas(const char* a_, const char* b_);
int calculaDiasDatas(const char* data1_, const char* data2_);
int calculaAtraso(const char* a_, const char* b_);
int entreDatas(const char* inicio, const char* fim, const char* inicio_ref, const char* fim_ref);
int getNoitesDatas(const char* inicio, const char* fim, const char* inicio_ref, const char* fim_ref);

//Setters


//int dataIncluida(Data* inicio, Data* fim, Data* inicio_ref, Data* fim_ref, bool entre_datas);

#endif
