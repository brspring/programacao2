parametrosCompilacao=-std=c99 -Wall -Wshadow
nomePrograma=vina++

all: $(nomePrograma)

$(nomePrograma): src/archive.o src/main.o
	gcc -o $(nomePrograma) src/archive.o src/main.o $(parametrosCompilacao)

src/archive.o: src/archive.c src/archive.h
	gcc -c src/archive.c -o src/archive.o $(parametrosCompilacao)

src/main.o: src/main.c
	gcc -c src/main.c -o src/main.o $(parametrosCompilacao)

clean:
	rm -f src/*.o src/$(nomePrograma)