#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "liblista.h"

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

int main(int argc, char *argv[])
{
    FILE *livro;
    livro = fopen("livro.txt", "r+");
    if (livro == NULL)
    {
        perror("Erro em abrir o arquivo!\n");
        return 1;
    }

    // nao sei se vou usar, mas coloca em uma variavel o numerod e palavras do livro
    int num_palavras = NumPalavrasLivro(livro);
    printf("O arquivo tem %d palavras.\n", num_palavras);

    // vetor que cada indice eh um caracter da tabela ASCII e ele eh do tipo lista para ter uma lista com a frequencia de cada letra
    nodo_l_t *vetorASCII[128] = {NULL};

    char c;

    int posicao = 0;
    bool primeiraLetra = true; // variável para verificar se é a primeira letra de uma palavra
    while ((c = fgetc(livro)) != EOF)
    {
        c = tolower(c);
        if (isalpha(c))
        {
            int index_pos = c;

            if (primeiraLetra)
            {
                if (vetorASCII[index_pos] == NULL)
                {
                    vetorASCII[index_pos] = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                    vetorASCII[index_pos]->elemento = posicao;
                    vetorASCII[index_pos]->prox = NULL;
                }
                else
                {
                    nodo_l_t *current = vetorASCII[index_pos];
                    while (current->prox != NULL)
                    {
                        current = current->prox;
                    }
                    current->prox = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                    current->prox->elemento = posicao;
                    current->prox->prox = NULL;
                }

                primeiraLetra = false; // atualiza para que a próxima letra não seja a primeira de uma palavra
            }
        }
        else if (c == ' ' || c == '\n')
        {
            primeiraLetra = true; // atualiza para que a próxima letra seja a primeira de uma palavra
            posicao++;            // atualiza a posição
        }
    }

    rewind(livro);

    for (int i = 0; i < 128; i++)
    {
        if (vetorASCII[i] != NULL)
        {   
            //printf("Lista para letra '%c' criada com sucesso!\n", i);
            printf("Letra '%c': ", i);
            nodo_l_t *current = vetorASCII[i];
            while (current != NULL)
            {
                printf("%d ", current->elemento);
                current = current->prox;
            }
            printf("\n");
        }
    }

    fclose(livro);
    return 0;
}