#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "listaBeale.h"
#include "codifica.h"


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

void cria_vetor_de_lista(FILE *arquivo, nodo_l_t **vetorASCII)
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

                primeiraLetra = false;
            }
        }
        else if (c == ' ')
        {
            primeiraLetra = true;
            posicao++;
        }
    }
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
    FILE *MensagemOriginal;
    MensagemOriginal = fopen("MensagemOriginal.txt", "r");

    if (!MensagemOriginal)
    {
        perror("Arquivo com a mensagem não existe\n");
        return 1;
    }

    Cria_arq_msg_codificada(MensagemOriginal, vetorASCII);

    /*novo arquivo com a mensagem codificada para ser usado na decodificação*/
    FILE* mensagem_decodificada = fopen("MensagemDecodificada.txt", "w");
    FILE* mensagem_codificada = fopen("MensagemCodificada.txt", "r");
    //12 32 23 
    if(!mensagem_codificada){
        printf("Arquivo com a mensagem codificada não encontrado.\n");
        return 1;
    }

    /*Decodifica a mensagem*/
    int char_codificado;
    
    while(fscanf(mensagem_codificada, "%d", &char_codificado) != EOF){
        if(char_codificado == -1)
        {
            fprintf(mensagem_decodificada, "%c" , 32);
        }else{
        char busca = buscar_numero_lista(char_codificado, vetorASCII);
        fprintf(mensagem_decodificada, "%c" , busca);
        }
    }

    /*nao sei se vou usar, mas coloca em uma variavel o numerod e palavras do livro*/
    /*int num_palavras = NumPalavrasLivro(livro);*/
    /*printf("O arquivo tem %d palavras.\n", num_palavras);*/

    fclose(mensagem_decodificada);
    fclose(mensagem_codificada);
    fclose(MensagemOriginal);
    fclose(livro);
    return 0;
}