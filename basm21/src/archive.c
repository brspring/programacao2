#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <time.h>
#include "archive.h"

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

int tamanho(FILE *archive)
{
    rewind(archive);
    struct stat f_data;

    fstat(fileno(archive), &f_data);
    return f_data.st_size;
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

int acrescenta_bytes_fim_arq(FILE *arch, const char *buffer, const unsigned int tam_bytes)
{
    unsigned int rt;

    if (tam_bytes <= 0)
        return 1;

    fseek(arch, 0, SEEK_END);
    rt = fwrite(buffer, 1, tam_bytes, arch);

    if (tam_bytes != rt)
        return 2;

    rewind(arch);
    return 0;
}

int remove_bytes(FILE *arch, const unsigned int b_init, const unsigned int b_final)
{
    char *buffer[1024];
    unsigned int tam = tamanho(arch);
    unsigned int read = b_final;
    unsigned int write = b_init - 1;
    unsigned int rt;

    if (b_init > b_final)
        return 1;
    if (b_final > tam)
        return 2;

    if (b_init <= 0)
    {
        perror("insira um valor maior que zero\n");
        return 1;
    }

    if (read == tam)
    {
        ftruncate(fileno(arch), b_init - 1);
        return 0;
    }

    while (read < tam)
    {
        fseek(arch, read, SEEK_SET);
        if (tam - read > 1024)
            rt = fread(buffer, 1, 1024, arch);
        else
            rt = fread(buffer, 1, tam - read, arch);
        fseek(arch, write, SEEK_SET);
        fwrite(buffer, 1, rt, arch);
        read += rt;
        write += rt;
    }

    rewind(arch);
    ftruncate(fileno(arch), tam - (b_final - b_init + 1));
    return 0;
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
    // printf("rt: %d\n", rt);
    if (rt)
    {
        return 1;
    }

    removerNo(diretorio, removal);

    return 0;
}

int copia_bytes(FILE *arch, char *buffer, const unsigned int b_init, const unsigned int b_final)
{
    unsigned int tam = tamanho(arch);
    unsigned int rt;

    if (b_init > b_final)
        return 1;
    if (b_final > tam)
        return 2;
    if (b_init < 1)
        return 3;

    fseek(arch, b_init - 1, SEEK_SET);
    rt = fread(buffer, 1, b_final - b_init + 1, arch);

    if (rt != b_final - b_init + 1)
        return 4;

    rewind(arch);
    return 0;
}

int move(FILE *arch, const unsigned int b_init, const unsigned int b_final, const unsigned int b_destino)
{
    char buffer[1024];
    unsigned int tam = tamanho(arch);
    printf("tam: %d\n", tam);
    unsigned int rt;
    unsigned int block, read, write;

    if (b_init > b_final)
        return 1;
    if (b_final > tam)
        return 2;
    if (b_init < 1)
        return 3;

    block = b_final - b_init + 1;
    read = b_init - 1;
    write = b_final;

    /*escreve no fim o block*/
    while (block > 0)
    {
        fseek(arch, read, SEEK_SET);

        if (block > 1024)
            rt = fread(buffer, 1, 1024, arch);
        else
            rt = fread(buffer, 1, block, arch);

        fseek(arch, 0, SEEK_END);
        fwrite(buffer, 1, rt, arch);
        read += rt;
        block -= rt;
    }

    /*abre um espaço com o tamanho de block para tras do destino*/
    if (b_destino < b_init)
    {
        block = b_init - b_destino;
        read = b_init - 1;
        write = b_final;
        while (block > 0)
        {
            if (block > 1024)
            {
                fseek(arch, read - 1024, SEEK_SET);
                rt = fread(buffer, 1, 1024, arch);
            }
            else
            {
                fseek(arch, read - block, SEEK_SET);
                rt = fread(buffer, 1, block, arch);
            }
            fseek(arch, write - rt, SEEK_SET);
            fwrite(buffer, 1, rt, arch);
            block -= rt;
            read -= rt;
            write -= rt;
        }
    }

    /*abre um espaço com o  tamanho de block para frente do destino*/
    if (b_destino > b_final)
    {
        block = b_destino - b_init;
        read = b_final;
        write = b_init - 1;

        while (block > 0)
        {
            if (block >= 1024)
            {
                fseek(arch, read, SEEK_SET);
                rt = fread(buffer, 1, 1024, arch);
            }
            else
            {
                fseek(arch, read, SEEK_SET);
                rt = fread(buffer, 1, block, arch);
            }
            fseek(arch, write, SEEK_SET);
            fwrite(buffer, 1, rt, arch);
            read += rt;
            write += rt;
            block -= rt;
        }
    }

    /*escreve o block no destino*/
    block = b_final - b_init + 1; 
    read =  tam;
    write = b_destino - 1;

    while (block > 0)
    {
        fseek(arch, read, SEEK_SET);
        if (block >= 1024)
        {
            rt = fread(buffer, 1, 1024, arch);
        }
        else
        {
            rt = fread(buffer, 1, block, arch);
        }

        printf("write: %d\n", write);
        fseek(arch, write - block, SEEK_SET);
        fwrite(buffer, 1, rt, arch);
        block -= rt;
        read -= rt;
        write -= rt;
    }
    rewind(arch);
    ftruncate(fileno(arch), tam);
    return 0;
}

int main()
{
    FILE *arquivo_copia = fopen("arquivo_copia.txt", "r+");

    move(arquivo_copia, 6, 8, 15);

    fclose(arquivo_copia);
}