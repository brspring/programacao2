#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include "listaBeale.h"
#include "codifica_decodifica.h"

#define TAM_MAX_LINHA 100
#define TAM_ASCII 128

int NumPalavrasLivro(FILE *livro)
{
    char *string = malloc(sizeof(char) * TAM_MAX_LINHA);
    int cont = 1;

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
    rewind(livro);
    free(string);
    return cont;
}

/*nodo_l_t *buscar_numero(int num, nodo_l_t **vetorASCII) {
    for (char i = 0; i < 128; i++) {
        nodo_l_t *atual = vetorASCII[i];
        while (atual != NULL) {
            if (atual->elemento == num) {
                return i;
            }
            atual = atual->prox;
        }
    }
    return NULL;
}*/


nodo_l_t *elemento_aleatorio(nodo_l_t *topo, int tamanho)
{
    /* gera um número aleatório entre 0 e o tamanho da lista - 1*/
    int indice_aleatorio = rand() % tamanho - 1;

    // percorre a lista até chegar no elemento de índice correspondente
    nodo_l_t *atual = topo;
    for (int i = 0; i < indice_aleatorio; i++)
    {
        atual = atual->prox;
    }

    return atual;
}

int tamanho_lista(nodo_l_t *head)
{
    int tamanho = 0;
    nodo_l_t *atual = head;

    while (atual != NULL)
    {
        tamanho++;
        atual = atual->prox;
    }

    return tamanho;
}

char buscar_numero_lista(int num, nodo_l_t **vetorASCII) {
    for (int i = 0; i < 128; i++) {
        nodo_l_t *atual = vetorASCII[i];
        while (atual != NULL) {
            if (atual->elemento == num) {
                return i;
            }
            atual = atual->prox;
        }
    }
    return -2;
}