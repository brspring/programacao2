#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stddef.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>

typedef struct FileInfo
{
    char nome[256];
    char path[1024];
    int posicao;
    int indice;
    size_t tam_inic;
    size_t tam;

    dev_t st_dev;
    mode_t permissoes;
    time_t ult_modif;
    uid_t UserID;
    gid_t GroupID;
} FileInfo_t;

typedef struct nodo_l nodo_t;

struct nodo_l
{
    FileInfo_t arquivo;
    nodo_t *prox;
    nodo_t *ant;
} nodo_l;

typedef struct dir
{
    unsigned int qntd;
    nodo_t *head;
    nodo_t *ult;
} dir_t;

void adiciona_arq_lista(dir_t *diretorio, FileInfo_t *arquivo);

void print_lista(dir_t *diretorio);

void liberarDiretorio(dir_t *diretorio);

void removerNo(dir_t *diretorio, nodo_t *no);

nodo_t *buscarArquivoPorNome(dir_t *diretorio, const char *nome);

void adiciona_metadados(FileInfo_t arquivo, FILE *arquivador);

void printa_metadados_lista(dir_t *diretorio, FILE *arquivador);

int tamanho(FILE *archive);

long long calcula_offset(FILE *arquivador, dir_t diretorio);

int acrescenta_bytes_fim_arq(FILE *arch, const char *buffer, const unsigned int tam_bytes);

int remove_bytes(FILE *arch, const unsigned int b_init, const unsigned int b_final);

int remove_member(const char *name, dir_t *diretorio, FILE *arquivador);

int copia_bytes(FILE *arch, char *buffer, const unsigned int b_init, const unsigned int b_final);

#endif

/*
    TESTE DA FUNCAO BUSCA

    const char* nomeBuscado = "a.txt";
    nodo_t* nodoEncontrado = buscarArquivoPorNome(&diretorio, nomeBuscado);

    if (nodoEncontrado != NULL)
    {
        printf("Arquivo encontrado: %s\n", nodoEncontrado->arquivo.nome);
        // Faça o que for necessário com as informações do arquivo encontrado
    }else
    {
        printf("Arquivo não encontrado.\n");
    }*/

/*
    TESTE DA FUNCAO DE REMOVER BYTES

    FILE *new = fopen("test.txt", "wb+");
    if (new == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    char buffer[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
    fwrite(buffer, 1, 9, new);

    int a, tam;

    tam = tamanho(new);

    a = remove_bytes(new, 1, 2);

    printf("tamanho: %d\n", tam);
    if (a == 0)
        printf("sucesso\n");
    else
        printf("erro\n");

    fclose(new);
    return 0;*/


 /*
    
    TESTE DA FUNCAO DE REMOVER MEMBRO

    const char *nomeMembro = "d.txt";
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

    //escreve o novo valor do offset no arquivador
    long long offset2 = calcula_offset(arquivador, diretorio);
    rewind(arquivador);
    fwrite(&offset2, sizeof(long long), 1, arquivador);

    //novo tamanho do arquivo em bytes
    int tamanhoNovo = tamanho(arquivador);
    printf("tamanhoNovo: %d\n", tamanhoNovo);
    
*/