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

    /*nao sei se vou usar, mas coloca em uma variavel o numerod e palavras do livro*/
    int num_palavras = NumPalavrasLivro(livro);
    printf("O arquivo tem %d palavras.\n", num_palavras);

    /* vetor que cada indice eh um caracter da tabela ASCII e ele eh do tipo lista para ter uma lista com a frequencia de cada letra*/
    nodo_l_t *vetorASCII[128] = {NULL};

    char c;
    int posicao = 0;

    bool primeiraLetra = true; /* variável para verificar se é a primeira letra de uma palavra*/

    while ((c = fgetc(livro)) != EOF)
    {
        c = tolower(c);
        /*if que identifica se é um alpha numerico ou caracter especial*/
        if (isalnum(c) || c == '!' || c == '@' || c == '(' || c == '$' || c == '#' || c == '-' || c == '&' || c == '%' || c == '=' || c == '"' )
        {
            int index_pos = c;

            /*printf("%d ", index_pos);*/

            if (primeiraLetra)
            {
                if (vetorASCII[index_pos] == NULL)
                {
                    vetorASCII[index_pos] = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                    vetorASCII[index_pos]->elemento = posicao;
                    vetorASCII[index_pos]->prox = NULL;
                }
                //sem esse else ele guarda uma posicao de cada letra, ver dps
                else
                {
                    nodo_l_t *atual = vetorASCII[index_pos];
                    while (atual->prox != NULL)
                    {
                        atual = atual->prox;
                    }
                    atual->prox = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                    atual->prox->elemento = posicao;
                    atual->prox->prox = NULL;
                }

                primeiraLetra = false; /* atualiza para que a próxima letra não seja a primeira de uma palavra*/
            }
        }
        else if (c == ' ')
        {
            primeiraLetra = true; /* atualiza para que a próxima letra seja a primeira de uma palavra */
            posicao++;            /* atualiza a posição */
        }
    }

    rewind(livro);
    /* for que printa a tabela de chaves*/
    for (int i = 0; i < 128; i++)
    {
        if (vetorASCII[i] != NULL)
        {   
            printf("%c: ", i);
            nodo_l_t *atual = vetorASCII[i];
            while (atual != NULL)
            {
                printf("%d ", atual->elemento);
                atual = atual->prox;
            }
            printf("\n");
        }
    }

    fclose(livro);
    return 0;
}