#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "archive.h"

void adicionarArquivo(dir_t *diretorio, FileInfo_t *arquivo)
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

void cria_dados_teste(){

}
int main()
{
    dir_t diretorio;
    diretorio.qntd = 0;
    diretorio.head = NULL;
    diretorio.ult = NULL;

    FileInfo_t arquivo1;
    strcpy(arquivo1.name, "a.txt");
    arquivo1.tam_inic = 8;
    arquivo1.tam = 8;
    arquivo1.st_dev = 0;
    arquivo1.permissoes = 777;
    arquivo1.ult_modif = time(NULL);
    arquivo1.UserID = getuid();
    arquivo1.GroupID = getgid();

    FileInfo_t arquivo2;
    strcpy(arquivo2.name, "b.txt");
    arquivo2.posicao = 0;
    arquivo2.tam_inic = 4;
    arquivo2.tam = 4;
    arquivo2.st_dev = 0;
    arquivo2.permissoes = 777;
    arquivo2.ult_modif = time(NULL);
    arquivo2.UserID = getuid();
    arquivo2.GroupID = getgid();
    
    adicionarArquivo(&diretorio, &arquivo1);
    adicionarArquivo(&diretorio, &arquivo2);

    FILE *arquivador = fopen("backup.vpp", "wb+");
    if (arquivador == NULL)
    {
        printf("Erro ao abrir o arquivo de arquivador\n");
        return 1;
    }

    //calcula o tamanho do offset
    long long offset = 0;
    nodo_t *agr = diretorio.head;
    while (agr != NULL)
    {
        offset += agr->arquivo.tam;
        agr = agr->prox;
    }

    fwrite(&offset, sizeof(long long), 1, arquivador);

    char buffer[8];

    memset(buffer, 'a', 8);
    fwrite(buffer, sizeof(char), 8, arquivador);

    memset(buffer, 'b', 4);
    fwrite(buffer, sizeof(char), 4, arquivador);

    fwrite(&arquivo1, sizeof(FileInfo_t), 1, arquivador);
    fwrite(&arquivo2, sizeof(FileInfo_t), 1, arquivador);


    // escreve a lista de metadadados
    /*nodo_t *atual = diretorio.head;
    while (atual != NULL)
    {
        fwrite(&(atual->arquivo), sizeof(FileInfo_t), 1, arquivador);
        atual = atual->prox;
    }*/

    rewind(arquivador);
    fclose(arquivador);
    liberarDiretorio(&diretorio);

    return 0;
}