#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "archive.h"

void incluirArquivo(FILE *arquivador, const FileInfo_t *arquivo) {
    fwrite(arquivo, sizeof(FileInfo_t), 1, arquivador);
}

void liberarLista(nodo_t *head) {
    nodo_t *atual = head;
    while (atual != NULL) {
        nodo_t *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

int main() {
    FileInfo_t arquivo1;
    arquivo1.name = "a.txt";
    arquivo1.nome_original = "a.txt";
    arquivo1.posicao = 0;
    arquivo1.tam_inic = 8;
    arquivo1.tam = 8;
    arquivo1.st_dev = 0;
    arquivo1.permissoes = 777;
    arquivo1.ult_modif = time(NULL);
    arquivo1.UserID = getuid();
    arquivo1.GroupID = getgid();

    FILE *arquivador = fopen("backup.vpp", "wb+");
    if (arquivador == NULL) {
        printf("Erro ao abrir o arquivo de arquivador\n");
        return 1;
    }

    incluirArquivo(arquivador, &arquivo1);

    if (arquivador == NULL) {
        printf("Erro ao abrir o arquivo de arquivador\n");
        return 1;
    }

    archive_t archive;
    archive.qntd = 0;
    archive.head = NULL;
    archive.ult = NULL;

    /* Le os metadados dos arquivos do arquivador e adiciona a lista*/
    while (1) {
        nodo_t *novoNodo = (nodo_t *)malloc(sizeof(nodo_t));

        size_t bytesLidos = fread(&(novoNodo->arquivo), sizeof(FileInfo_t), 1, arquivador);

        if (bytesLidos != 1) {
            free(novoNodo);
            break;
        }

        novoNodo->prox = NULL;

        if (archive.head == NULL) {
            /* Se a lista ta vazia, o novo nodo se torna a cabeca da lista */
            archive.head = novoNodo;
            archive.ult = novoNodo;
        } else {
            /* Se nn adicionamos o novo nodo no final da lista*/
            archive.ult->prox = novoNodo;
            archive.ult = novoNodo;
        }

        archive.qntd++;
    }

    fclose(arquivador);

    nodo_t *nodoAtual = archive.head;
    while (nodoAtual != NULL) {
        printf("Nome do arquivo: %s\n", nodoAtual->arquivo.name);

        nodoAtual = nodoAtual->prox;
    }

    liberarLista(archive.head);

    return 0;
}