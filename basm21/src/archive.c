#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <time.h>
#include "archive.h"
#include "archiver.h"


#define MAX_SIZE 2014

void adiciona_arq_lista(dir_t *diretorio, FileInfo_t *arquivo)
{
    nodo_t *novoNodo = malloc(sizeof(nodo_t));
    novoNodo->arquivo = *arquivo;
    novoNodo->prox = NULL;

    if (diretorio->head == NULL)
    {
        diretorio->head = novoNodo;
        diretorio->ult = novoNodo;
    }
    else
    {
        diretorio->ult->prox = novoNodo;
        diretorio->ult = novoNodo;
    }

    diretorio->qntd++;
}

void print_lista(dir_t *diretorio)
{
    printf("|-------------------|\n");
    printf("v Lista de arquivos v\n");
    printf("--------------------\n");
    nodo_t *atual = diretorio->head;
    while (atual != NULL)
    {
        printf("Nome: %s\n", atual->arquivo.nome);
        printf("Posição: %u\n", atual->arquivo.posicao);
        printf("Índice: %u\n", atual->arquivo.indice);
        printf("Tamanho: %lu\n", atual->arquivo.tam);
        printf("Permissões: %o\n", atual->arquivo.permissoes);
        printf("Última modificação: %ld\n", atual->arquivo.ult_modif);
        printf("UserID: %d\n", atual->arquivo.UserID);
        printf("GroupID: %d\n", atual->arquivo.GroupID);
        printf("--------------------\n");
        atual = atual->prox;
    }
}

void liberarDiretorio(dir_t *diretorio)
{
    nodo_t *atual = diretorio->head;
    while (atual != NULL)
    {
        nodo_t *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    diretorio->head = NULL;
    diretorio->ult = NULL;
    diretorio->qntd = 0;
}

void removerNo(dir_t *diretorio, nodo_t *no)
{
    // Atualiza o ponteiro diretorio->ult se o nó removido for o último nó
    if (no == diretorio->ult)
    {
        nodo_t *atual = diretorio->head;
        while (atual->prox != diretorio->ult)
        {
            atual = atual->prox;
        }
        diretorio->ult = atual;
    }

    // Remover o nó correspondente do diretório
    nodo_t *atual = diretorio->head;
    if (atual == no)
    {
        diretorio->head = atual->prox;
    }
    else
    {
        while (atual->prox != no)
        {
            atual = atual->prox;
        }
        atual->prox = no->prox;
    }

    diretorio->qntd--;

    free(no);
}

nodo_t *buscarArquivoPorNome(dir_t *diretorio, const char *nome)
{
    nodo_t *atual = diretorio->head;
    while (atual != NULL)
    {
        if (strcmp(atual->arquivo.nome, nome) == 0)
        {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void adiciona_metadados(FileInfo_t arquivo, FILE *arquivador)
{
    fwrite(&arquivo, sizeof(FileInfo_t), 1, arquivador);
}

void printa_metadados_lista(dir_t *diretorio, FILE *arquivador)
{
    nodo_t *atual = diretorio->head;
    while (atual != NULL)
    {
        adiciona_metadados(atual->arquivo, arquivador);
        atual = atual->prox;
    }
}

long long calcula_offset(FILE *arquivador, dir_t diretorio)
{
    long long offset = 0;
    nodo_t *agr = diretorio.head;
    while (agr != NULL)
    {
        offset += agr->arquivo.tam;
        agr = agr->prox;
    }
    return offset;
}

int remove_member(const char *name, dir_t *diretorio, FILE *arquivador)
{
    nodo_t *removal = buscarArquivoPorNome(diretorio, name);
    unsigned int b_arq_init, b_arq_final, rt, rt2, b_metadados_init, b_metadados_tam, b_metadados_final;
    int indice;
    long long offset = 0;

    if (removal == NULL)
    {
        return 1;
    }

    indice = removal->arquivo.indice;
    /*calculo os valores para remover o conteudo do membro*/
    b_arq_init = removal->arquivo.posicao;
    b_arq_final = removal->arquivo.posicao + removal->arquivo.tam - 1;

    /*calcula o offset antes de tudo para remover os metadados da forma certa*/
    offset = calcula_offset(arquivador, *diretorio);

    /*calcula os valores para remover os metadados do membro*/
    b_metadados_tam = sizeof(FileInfo_t);
    b_metadados_init = sizeof(long long) + offset + 1 + (indice * b_metadados_tam);
    b_metadados_final = b_metadados_init + b_metadados_tam - 1;
    printf("----------bytes------------\n");
    printf("b_arq_init: %d\n", b_arq_init);
    printf("b_arq_final: %d\n", b_arq_final);
    printf("b_metadados_init: %d\n", b_metadados_init);
    printf("b_metadados_final: %d\n", b_metadados_final);
    printf("b_metadados_tam: %d\n", b_metadados_tam);
    printf("---------------------------\n");

    /*remove os bytes dos metadados do membro*/
    rt2 = remove_bytes(arquivador, b_metadados_init, b_metadados_final);
    if (rt2 != 0)
    {
        return 3;
    }

    /*remove o conteudo do membro*/
    rt = remove_bytes(arquivador, b_arq_init, b_arq_final);
    if (rt)
    {
        return 1;
    }

    removerNo(diretorio, removal);

    return 0;
}

int main()
{
    FILE *arquivo_copia = fopen("arquivo_copia.txt", "r+");

    move_bytes(arquivo_copia, 1, 3, 6);

    fclose(arquivo_copia);
}