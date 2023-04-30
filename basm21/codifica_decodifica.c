#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <getopt.h>
#include "listaBeale.h"
#include "codifica_decodifica.h"

void cria_vetor_de_lista_com_livro(FILE *arquivo, nodo_l_t **vetorASCII)
{
    char c;
    int posicao = 0;

    bool primeiraLetra = true;

    while ((c = fgetc(arquivo)) != EOF)
    {
        c = tolower(c);
        if (isalnum(c) || c == '!' || c == '@' || c == '(' || c == '$' || c == '#' || c == '-' || c == '&' || c == '%' || c == '=' || c == '"')
        {
            int index_pos = c;
            if (primeiraLetra && c != '\n' && c != ' ')
            {
                if (vetorASCII[index_pos] == NULL)
                {
                    vetorASCII[index_pos] = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                    vetorASCII[index_pos]->elemento = posicao;
                    vetorASCII[index_pos]->prox = NULL;
                }
                else /*cria as outras posicoes das listas*/
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

                primeiraLetra = false;
            }
        }
        else if (c == ' ')
        {
            primeiraLetra = true;
            posicao++;
        }
    }
    rewind(arquivo);
}

void cria_arquivo_de_chaves(nodo_l_t **vetorASCII)
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
    fclose(ArquivoDeChaves);
}

void Cria_arq_msg_codificada(FILE *arquivo, nodo_l_t **vetorASCII)
{
    FILE *MensagemCodificada;
    char chr;
    MensagemCodificada = fopen("MensagemCodificada.txt", "w+");
    srand(time(NULL));

    while ((chr = fgetc(arquivo)) != EOF)
    {
        chr = tolower(chr);

        if (chr == ' ')
        {
            fprintf(MensagemCodificada, "%d ", -1);
        }
        else if (isalnum(chr))
        {
            int index_pos = chr;
            nodo_l_t *lista_atual = vetorASCII[index_pos];

            /* pego o tamanho da lista para pegar uma posição aleatoria da letra*/
            int tamanho_lista_atual = tamanho_lista(lista_atual);

            nodo_l_t *no_elemento_aleatorio = elemento_aleatorio(vetorASCII[index_pos], tamanho_lista_atual);

            fprintf(MensagemCodificada, "%d ", no_elemento_aleatorio->elemento);
        }
    }
    fclose(MensagemCodificada);
}

void cria_arq_msg_decodificada(nodo_l_t **vetorASCII)
{
    /*novo arquivo com a mensagem codificada para ser usado na decodificação*/

    FILE *mensagem_codificada = fopen("MensagemCodificada.txt", "r");
    if (!mensagem_codificada)
    {
        printf("Arquivo com a mensagem codificada não encontrado.\n");
        exit(1);
    }

    FILE *mensagem_decodificada = fopen("MensagemDecodificada.txt", "w");

    /*Decodifica a mensagem*/
    int char_codificado;

    while (fscanf(mensagem_codificada, "%d", &char_codificado) != EOF)
    {
        if (char_codificado == -1)
        {
            fprintf(mensagem_decodificada, "%c", 32);
        }
        else
        {
            char busca = buscar_numero_lista(char_codificado, vetorASCII);
            fprintf(mensagem_decodificada, "%c", busca);
        }
    }
    fclose(mensagem_decodificada);
    fclose(mensagem_codificada);
}

void libera_vetor_de_lista(nodo_l_t **vetorASCII)
{
    nodo_l_t *atual, *prox;

    for (int i = 0; i < 256; i++)
    {
        if (vetorASCII[i] != NULL)
        {
            atual = vetorASCII[i];
            while (atual != NULL)
            {
                prox = atual->prox;
                free(atual);
                atual = prox;
            }
        }
    }
    free(vetorASCII);
}

void cria_vetor_de_lista_com_arq_de_chaves(nodo_l_t **vetorASCII)
{
    FILE *arquivo_de_chaves;
    arquivo_de_chaves = fopen("ArquivoDeChaves.txt", "r");

    if (arquivo_de_chaves == NULL)
    {
        perror("Erro em abrir o arquivo de chaves!\n");
        exit(1);
    }

    char linha[1000];

    while (fgets(linha, 1000, arquivo_de_chaves) != NULL)
    {
        char *separador = strtok(linha, ": \n"); /* quebra a linha  com base nos : \n"*/

        if (separador != NULL)
        {
            int indice = separador[0]; /*a primeira letra que é o indice do vetor*/

            separador = strtok(NULL, ": \n"); /*vai para o indice*/

            while (separador != NULL) /*vai por separadores*/
            {
                int posicao;
                if (sscanf(separador, "%d", &posicao) == 1) /* verifica se o separador pode ser inteiro*/
                {
                    if (vetorASCII[indice] == NULL) /*se a lista da letra n existe*/
                    {
                        vetorASCII[indice] = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                        vetorASCII[indice]->elemento = posicao;
                        vetorASCII[indice]->prox = NULL;
                    }
                    else /*se ja existe adiciona*/
                    {
                        nodo_l_t *atual = vetorASCII[indice];
                        while (atual->prox != NULL)
                        {
                            atual = atual->prox;
                        }
                        atual->prox = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                        atual->prox->elemento = posicao;
                        atual->prox->prox = NULL;
                    }
                }
                separador = strtok(NULL, ": \n"); /* avanca para o próximo separador*/
            }
        }
    }
    
    rewind(arquivo_de_chaves);
}
