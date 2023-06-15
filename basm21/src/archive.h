#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stddef.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>

typedef struct FileInfo {
    char nome[256];
    char path[1024];
    int posicao;
    size_t tam_inic;
    size_t tam;

    dev_t  st_dev;
    mode_t permissoes;
    time_t ult_modif;
    uid_t UserID;
    gid_t  GroupID;
} FileInfo_t;

typedef struct nodo_l nodo_t;  

typedef struct nodo_l {
    FileInfo_t arquivo;
    nodo_t* prox; 
    nodo_t* ant;
} nodo_l;

typedef struct dir {
    unsigned int qntd;
    nodo_t *head;
    nodo_t *ult;
} dir_t;

void adicionarArquivo(dir_t *diretorio, FileInfo_t *arquivo);

void liberarDiretorio(dir_t *diretorio);

int tamanho(FILE* archive);

nodo_t *buscarArquivoPorNome(dir_t *diretorio, const char *nome);

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