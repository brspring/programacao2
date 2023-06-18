#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <time.h>
#include "archive.h"

#define MAX_SIZE 1024

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
    printf("Lista de arquivos:\n");
    nodo_t *atual = diretorio->head;
    while (atual != NULL)
    {
        printf("Nome: %s\n", atual->arquivo.nome);
        printf("Posição: %u\n", atual->arquivo.posicao);
        printf("Tamanho: %lu\n", atual->arquivo.tam);
        printf("Permissões: %o\n", atual->arquivo.permissoes);
        printf("Última modificação: %ld\n", atual->arquivo.ult_modif);
        printf("UserID: %d\n", atual->arquivo.UserID);
        printf("GroupID: %d\n", atual->arquivo.GroupID);
        printf("----------------------\n");
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

int remove_bytes(FILE *arch, const unsigned int b_init, const unsigned int b_final)
{
    char *buffer[1024];
    unsigned int tam = tamanho(arch);
    printf("tam na funcao remove bytes: %d\n", tam);
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
    unsigned int b_init, b_final, rt;
    if (removal == NULL)
    {
        return 1;
    }

    b_init = removal->arquivo.posicao;
    b_final = removal->arquivo.posicao + removal->arquivo.tam - 1;

    rt = remove_bytes(arquivador, b_init, b_final);
    printf("rt: %d\n", rt);
    if (rt)
    {
        return 1;
    }

    removerNo(diretorio, removal);
    return 0;
}

int main()
{

    FILE *arquivador = fopen("backup.vpp", "wb+");
    struct stat f_data;

    if (arquivador == NULL)
    {
        printf("Erro ao abrir o arquivo de arquivador\n");
        return 1;
    }

    /*---------------------------------TESTE------------------------------------*/
    dir_t diretorio;
    diretorio.qntd = 0;
    diretorio.head = NULL;
    diretorio.ult = NULL;

    FileInfo_t arquivo1;
    strcpy(arquivo1.nome, "a.txt");
    arquivo1.posicao = sizeof(long long) + 1;
    arquivo1.indice = 1;
    arquivo1.tam_inic = 8;
    arquivo1.tam = 8;
    arquivo1.st_dev = 0;
    arquivo1.permissoes = 777;
    arquivo1.ult_modif = time(NULL);
    arquivo1.UserID = getuid();
    arquivo1.GroupID = getgid();

    FileInfo_t arquivo2;
    strcpy(arquivo2.nome, "b.txt");
    arquivo2.posicao = sizeof(long long) + arquivo1.tam + 1;
    arquivo2.indice = 2;
    arquivo2.tam_inic = 4;
    arquivo2.tam = 4;
    arquivo2.st_dev = 0;
    arquivo2.permissoes = 777;
    arquivo2.ult_modif = time(NULL);
    arquivo2.UserID = getuid();
    arquivo2.GroupID = getgid();

    adiciona_arq_lista(&diretorio, &arquivo1);
    adiciona_arq_lista(&diretorio, &arquivo2);

    long long offset = calcula_offset(arquivador, diretorio);

    fwrite(&offset, sizeof(long long), 1, arquivador);

    char buffer[8];

    memset(buffer, 'a', 8);
    fwrite(buffer, sizeof(char), 8, arquivador);

    memset(buffer, 'b', 4);
    fwrite(buffer, sizeof(char), 4, arquivador);

    adiciona_metadados(arquivo1, arquivador);
    adiciona_metadados(arquivo2, arquivador);
    /*-------------------------------------------------------------------*/

    const char *nomeMembro = "a.txt";
    int tamantigo = tamanho(arquivador);
    printf("tamantigo: %d\n", tamantigo);
    int resultado = remove_member(nomeMembro, &diretorio, arquivador);

    if (resultado == 0)
    {
        printf("Membro removido com sucesso!\n");
    }
    else
    {
        printf("Erro ao remover o membro.\n");
    }
    int tamanhoNovo = tamanho(arquivador);
    print_lista(&diretorio);
    liberarDiretorio(&diretorio);
}