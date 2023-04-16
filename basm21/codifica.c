#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "liblista.h"

#define TAM_MAX_LINHA 100
#define TAM_ASCII 128

int NumPalavrasLivro(FILE *livro)
{
    char *string = malloc(sizeof(char) * TAM_MAX_LINHA);
    int cont = 0;

    while (fgets(string, TAM_MAX_LINHA, livro))
    {
        for (int i = 0; string[i] != '\0'; i++)
        {
            if (string[i] == ' ' || string[i] == '\n' || string[i] == '\t')
            {
                cont++;
            }
        }
    }
    free(string);
    return cont;
}

int main(int argc, char *argv[])
{
    const char *livro_cifra = "livro.txt";
    FILE *livro;
    livro = fopen(livro_cifra, "r");

    if (!livro)
    {
        perror("Erro em abrir o arquivo!\n");
        return 1;
    }

    //nao sei se vou usar, mas coloca em uma variavel o numerod e palavras do livro
    int num_palavras = NumPalavrasLivro(livro);
    printf("O arquivo tem %d palavras.\n", num_palavras);

    //vetor que cada indice eh um caracter da tabela ASCII e ele eh do tipo lista para ter uma lista com a frequencia de cada letra
    nodo_l_t *primeiraLetra[TAM_ASCII] = {NULL};

    int posicao = 0;
    char c;

     while ((c = fgetc(livro)) != EOF) {
        if (isascii(c)) {
            int index_pos = c;
            nodo_l_t* novo_nodo = (nodo_l_t*) malloc(sizeof(nodo_l_t));
            novo_nodo->posicao = posicao;
            novo_nodo->prox = NULL;

            if (primeiraLetra[index_pos] == NULL) {
                primeiraLetra[index_pos] = novo_nodo;
            } else {
                nodo_l_t* current = primeiraLetra[index_pos];
                while (current->prox != NULL) {
                    current = current->prox;
                }
                current->prox = novo_nodo;
            }
        }

        if (c == ' ' || c == '\n') {
            posicao++;
        }
    }

    fclose(livro);
    return 0;
}