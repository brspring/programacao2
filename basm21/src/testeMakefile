parametrosCompilacao=-std=c99 -Wall -Wshadow
nomePrograma=vina++

all: $(nomePrograma)

$(nomePrograma): archiver.o archive.o main.o
	gcc -o $(nomePrograma) archiver.o archive.o main.o $(parametrosCompilacao)

archiver.o: archiver.c
	gcc -c archiver.c $(parametrosCompilacao)

archive.o: archive.c
	gcc -c archive.c $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)
