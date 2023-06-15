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

dir_t cria_teste(FILE *arquivador, dir_t diretorio)
{

    FileInfo_t arquivo1;
    strcpy(arquivo1.nome, "a.txt");
    arquivo1.tam_inic = 8;
    arquivo1.tam = 8;
    arquivo1.st_dev = 0;
    arquivo1.permissoes = 777;
    arquivo1.ult_modif = time(NULL);
    arquivo1.UserID = getuid();
    arquivo1.GroupID = getgid();

    FileInfo_t arquivo2;
    strcpy(arquivo2.nome, "b.txt");
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

    long long offset = calcula_offset(arquivador, diretorio);

    fwrite(&offset, sizeof(long long), 1, arquivador);

    char buffer[8];

    memset(buffer, 'a', 8);
    fwrite(buffer, sizeof(char), 8, arquivador);

    memset(buffer, 'b', 4);
    fwrite(buffer, sizeof(char), 4, arquivador);

    fwrite(&arquivo1, sizeof(FileInfo_t), 1, arquivador);
    fwrite(&arquivo2, sizeof(FileInfo_t), 1, arquivador);

    printf("tamanho do offset: %lld\n", offset);
    rewind(arquivador);
    fclose(arquivador);

    return diretorio;
}

int remove_bytes(FILE *arch, const unsigned int b_init, const unsigned int b_final)
{
    char *buffer[1024];
    unsigned int tam = tamanho(arch);
    unsigned int read = b_final;
    unsigned int write = b_init - 1;
    unsigned int rt;

    if (b_init > b_final) return 1;
	if (b_final > tam) return 2;

    if (b_init <= 0){
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

int main()
{

    /*  FILE *arquivador = fopen("backup.vpp", "wb+");
      if (arquivador == NULL)
      {
          printf("Erro ao abrir o arquivo de arquivador\n");
          return 1;
      }

      dir_t diretorio;
      diretorio.qntd = 0;
      diretorio.head = NULL;
      diretorio.ult = NULL;

      diretorio = cria_teste(arquivador, diretorio);
      long long offset = calcula_offset(arquivador, diretorio);
      remove_bytes(arquivador, sizeof(offset), offset);

      liberarDiretorio(&diretorio);*/

    FILE *new = fopen("test.txt", "wb+");
    if (new == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    char buffer[9] = {'z', 'z', 'z', 'z', 'a', 'a', 'a', 'a', 'a'};
    fwrite(buffer, 9, 1, new);
    
    int a, tam;
    
    tam = tamanho(new);

    a = remove_bytes(new, 1, 2);

    //printf("tamanho: %d\n", tam);
    if (a = 0)
        printf("sucesso\n");
    else
        printf("erro\n");

    fclose(new);
    return 0;
}