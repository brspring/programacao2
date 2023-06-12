#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stddef.h>
#include <sys/stat.h>
#include <time.h>

typedef struct FileInfo {
    char *name;
    char *nome_original;
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
} nodo_l;


typedef struct archive {
    unsigned int qntd;
    nodo_t *head;
    nodo_t *ult;
} archive_t;

#endif