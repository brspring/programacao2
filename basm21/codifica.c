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

void CriaArquivoDeChaves(nodo_l_t **vetorASCII)
{
    FILE *ArquivoDeChaves = fopen("ArquivoDeChaves.txt", "w");

    if (!ArquivoDeChaves)
    {
        perror("O arquivo de chaves não foi criado\n");
    }

    for (int i = 0; i < 128; i++)
    {
        if (vetorASCII[i] != NULL)
        {
            fprintf(ArquivoDeChaves, "%c: ", i);
            nodo_l_t *atual = vetorASCII[i];
            while (atual != NULL)
            {
                fprintf(ArquivoDeChaves, "%d ", atual->elemento);
                atual = atual->prox;
            }
            fprintf(ArquivoDeChaves, "\n");
        }
    }
    printf("arquivo craido!\n");
    fclose(ArquivoDeChaves);
}
/*
buffer[100];
posicao = 0;
while (fscanf(livro, "%99s", buffer) == 1)
    {
    for (int i = 0; buffer[i]; i++)
    {
        char c = tolower(buffer[i]);
        if (isalnum(c) || c == '!' || c == '@' || c == '(' || c == '$' || c == '#' || c == '-' || c == '&' || c == '%' || c == '=' || c == '"')
        {
            int index_pos = c;

            if (i == 0 && c != '\n' && c != ' ')
            {
                if (vetorASCII[index_pos] == NULL)
                {
                    vetorASCII[index_pos] = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                    vetorASCII[index_pos]->elemento = posicao;
                    vetorASCII[index_pos]->prox = NULL;
                }
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
            }
        }
    }
*/

void cria_vetor_de_lista(FILE *arquivo, nodo_l_t **vetorASCII) {
    char c;
    int posicao = 0;

    bool primeiraLetra = true;

    while ((c = fgetc(arquivo)) != EOF) {
        c = tolower(c);
        if (isalnum(c) || c == '!' || c == '@' || c == '(' || c == '$' || c == '#' || c == '-' || c == '&' || c == '%' || c == '=' || c == '"') {
            int index_pos = c;
            if (primeiraLetra && c != '\n' && c != ' ') {
                if (vetorASCII[index_pos] == NULL) {
                    vetorASCII[index_pos] = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                    vetorASCII[index_pos]->elemento = posicao;
                    vetorASCII[index_pos]->prox = NULL;
                } else {
                    nodo_l_t *atual = vetorASCII[index_pos];
                    while (atual->prox != NULL) {
                        atual = atual->prox;
                    }
                    atual->prox = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                    atual->prox->elemento = posicao;
                    atual->prox->prox = NULL;
                }

                primeiraLetra = false;
            }
        } else if (c == ' ') {
            primeiraLetra = true;
            posicao++;
        }
    }
}

void Cria_arq_msg_codificada(FILE *arquivo, nodo_l_t **vetorASCII)
{
    FILE* MensagemCodificada;
    char chr;    
    MensagemCodificada = fopen("MensagemCodificada.txt", "w+");

    while ((chr = fgetc(arquivo)) != EOF)
    {
        chr = tolower(chr);
        int index_pos = chr;
        
        if(chr == ' ')
        {
            fprintf(MensagemCodificada, "%d ", -1);
        }

        if (isalnum(chr) && (chr != ' ')) 
        {
            int index_pos = chr;
            nodo_l_t *teste = vetorASCII[index_pos];
            fprintf(MensagemCodificada, "%d ", teste->elemento);
        }
    }
    fclose(MensagemCodificada);
}

int main(int argc, char *argv[])
{
    FILE *livro;
    livro = fopen("livro.txt", "r");
    if (livro == NULL)
    {
        perror("Erro em abrir o arquivo!\n");
        return 1;
    }

    /* vetor que cada indice eh um caracter da tabela ASCII e ele eh do tipo lista para ter uma lista com a frequencia de cada letra*/
    nodo_l_t *vetorASCII[128] = {NULL};

    cria_vetor_de_lista(livro, vetorASCII);
    CriaArquivoDeChaves(vetorASCII);

    /*preciso voltar o ponteiro para o inicio para contar o numero de palavras do arquivo*/
    rewind(livro);

    /*codificar uma mensagem*/
    FILE* MensagemOriginal;
    MensagemOriginal = fopen("MensagemOriginal.txt", "r");

    if(!MensagemOriginal){
        perror("Arquivo com a mensagem não existe\n");
    }
    
    Cria_arq_msg_codificada(MensagemOriginal, vetorASCII);

    /*nao sei se vou usar, mas coloca em uma variavel o numerod e palavras do livro*/
    int num_palavras = NumPalavrasLivro(livro);
    printf("O arquivo tem %d palavras.\n", num_palavras);

    fclose(MensagemOriginal);
    fclose(livro);
    return 0;
}