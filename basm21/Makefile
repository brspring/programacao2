parametrosCompilacao=-std=c99 -Wall -Wshadow 
nomePrograma=beale

all: $(nomePrograma)

$(nomePrograma): codifica_decodifica.o listaBeale.o main.o
	gcc -o $(nomePrograma) codifica_decodifica.o listaBeale.o main.o $(parametrosCompilacao)

codifica_decodifica.o: codifica_decodifica.c codifica_decodifica.h
	gcc -c codifica_decodifica.c $(parametrosCompilacao)

listaBeale.o: listaBeale.h listaBeale.c
	gcc -c listaBeale.c $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)
