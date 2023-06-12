#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "archive.h"

void liberarLista(nodo_t *head)
{
    nodo_t *atual = head;
    while (atual != NULL)
    {
        nodo_t *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

int main()
{
    /*

    INCLUINDO VARIOS ARQUIVOS NA MAO 


    FileInfo_t arquivo1;
    arquivo1.name[0] = 'a';
    arquivo1.name[1] = 0;
    arquivo1.tam_inic = 8;
    arquivo1.tam = 8;
    arquivo1.st_dev = 0;
    arquivo1.permissoes = 777;
    arquivo1.ult_modif = time(NULL);
    arquivo1.UserID = getuid();
    arquivo1.GroupID = getgid();

    FileInfo_t arquivo2;
    arquivo2.name[0] = 'b';
    arquivo2.name[1] = 0;
    arquivo2.posicao = 0;
    arquivo2.tam_inic = 4;
    arquivo2.tam = 4;
    arquivo2.st_dev = 0;
    arquivo2.permissoes = 777;
    arquivo2.ult_modif = time(NULL);
    arquivo2.UserID = getuid();
    arquivo2.GroupID = getgid();

    FILE *arquivador = fopen("backup.vpp", "wb+");
    long long offset =  12;

    if (arquivador == NULL)
    {
        printf("Erro ao abrir o arquivo de arquivador\n");
        return 1;
    }

    fwrite(&offset, sizeof(long long), 1, arquivador);
    char buffer[8];

    memset(buffer, 'a', 8);
    fwrite(buffer, sizeof(char), 8, arquivador);

    memset(buffer, 'b', 4);
    fwrite(buffer, sizeof(char), 4, arquivador);

    fwrite(&arquivo1, sizeof(FileInfo_t), 1, arquivador);
    fwrite(&arquivo2, sizeof(FileInfo_t), 1, arquivador);

    fclose(arquivador);

    falta colocar o arquivador */

    
    return 0;
}