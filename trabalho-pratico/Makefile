CFLAGS=-Wall -Iinclude -fno-stack-protector -g -Werror=discarded-qualifiers

inicio:programa-principal programa-testes

#exemplo
programa-principal: src/programa-principal.o src/datas.o src/batch.o src/queries.o src/modo_interativo.o src/arraydin.o src/hashtable.o src/ficheiros.o src/users.o src/reservas.o src/voos.o
	@gcc $(LDFLAGS) $(CFLAGS) $^ -o programa-principal -lncurses

#exemplo
programa-testes: src/programa-testes.o src/datas.o src/batch.o src/queries.o src/arraydin.o src/hashtable.o src/ficheiros.o src/users.o src/reservas.o src/voos.o src/testes.o
	@gcc $(LDFLAGS) $(CFLAGS) $^ -o programa-testes

src/programa-principal.o: src/programa-principal.c
src/programa-testes.o : src/programa-testes.c
src/users.o: src/users.c
src/reservas.o: src/reservas.c
src/ficheiros.o: src/ficheiros.c
src/voos.o: src/voos.c
src/datas.o: src/datas.c
src/testes.o: src/testes.c
src/batch.o: src/batch.c

src/queries.o: src/queries.c
src/modo_interativo.o: src/modo_interativo.c

src/arraydin.o: src/arraydin.c
src/hashtable.o: src/hashtable.c

clean:
	rm -f programa-principal programa-testes src/*.o include/*.o Resultados/*.txt Resultados/users_errors.csv Resultados/reservations_errors.csv Resultados/flights_errors.csv Resultados/passengers_errors.csv

