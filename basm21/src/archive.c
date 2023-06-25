#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#include <unistd.h>
#include <time.h>
#include "archive.h"
#include "archiver.h"

#define MAX_SIZE 2014

void inicia_dir(dir_t *diretorio)
{
    diretorio->qntd = 0;
    diretorio->head = NULL;
    diretorio->ult = NULL;
}

long long le_offset(FILE *arquivador)
{
    long long offset;
    fread(&offset, sizeof(long long), 1, arquivador);
    return offset;
}

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

void atualizarIndices(dir_t *diretorio)
{
    nodo_t *atual = diretorio->head;
    int indice = 0;

    while (atual != NULL)
    {
        atual->arquivo.indice = indice;
        atual = atual->prox;
        indice++;
    }
}

void exclui_todos_metadados_arquivador(FILE *arquivador)
{
    long long offset = le_offset(arquivador);

    fseek(arquivador, 0, SEEK_SET);
    fwrite(&offset, sizeof(long long), 1, arquivador);

    ftruncate(fileno(arquivador), offset + sizeof(long long));
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

void moverNoParaIndice(dir_t *diretorio, nodo_t *no, int indice)
{
    if (no == NULL || diretorio == NULL || diretorio->qntd == 0)
    {
        printf("Erro: Nó ou lista inválidos.\n");
        return;
    }

    if (indice < 0 || indice >= diretorio->qntd)
    {
        printf("Erro: Índice fora dos limites da lista.\n");
        return;
    }

    if (indice == no->arquivo.indice)
    {
        printf("O nó já está na posição correta.\n");
        return;
    }

    FileInfo_t info = no->arquivo;
    nodo_t *atual = diretorio->head;

    if (indice == 0)
    {
        removerNo(diretorio, no);
        nodo_t *novoNodo = malloc(sizeof(nodo_t));
        novoNodo->arquivo = info;
        novoNodo->prox = atual;
        novoNodo->ant = NULL;

        if (atual != NULL)
        {
            atual->ant = novoNodo;
        }

        diretorio->head = novoNodo;
    }
    else if (indice < no->arquivo.indice) // Move para trás
    {
        int i = indice - 1;
        removerNo(diretorio, no);
        while (i >= indice && atual != NULL) // Correção aqui
        {
            atual = atual->ant;
            i--;
        }

        if (atual == NULL)
        {
            // adiciona_arq_lista(diretorio, &info);
        }

        nodo_t *novoNodo = malloc(sizeof(nodo_t));
        novoNodo->arquivo = info;
        novoNodo->prox = atual->prox;
        novoNodo->ant = atual;
        novoNodo->ant = atual;
        novoNodo->prox = atual->prox;

        if (atual->prox != NULL && atual != NULL)
            atual->prox->ant = novoNodo;

        if (atual != NULL)
            atual->prox = novoNodo;
    }
    else // Move para frente
    {
        removerNo(diretorio, no);
        int i = 0;
        while (i < indice && atual != NULL)
        {
            atual = atual->prox;
            i++;
        }

        if (atual == NULL)
        {
            adiciona_arq_lista(diretorio, &info);
        }
        else
        {
            nodo_t *novoNodo = malloc(sizeof(nodo_t));
            novoNodo->arquivo = info;
            novoNodo->prox = atual->prox;
            novoNodo->ant = atual;
            if (atual->prox != NULL)
            {
                atual->prox->ant = novoNodo;
            }
            atual->prox = novoNodo;
        }
    }

    diretorio->qntd++;
    atualizarIndices(diretorio);
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
    int offset = calcula_offset(arquivador, *diretorio);
    fseek(arquivador, offset + 9, SEEK_SET);
    nodo_t *atual = diretorio->head;
    while (atual != NULL)
    {
        adiciona_metadados(atual->arquivo, arquivador);
        atual = atual->prox;
    }
}

int remove_member(const char *name, dir_t *diretorio, FILE *arquivador)
{
    nodo_t *removal = buscarArquivoPorNome(diretorio, name);
    long long offset = calcula_offset(arquivador, *diretorio);
    printf("offset: %lld\n", offset);

    if (removal == NULL)
    {
        return 1;
    }
    unsigned int b_metadado_fim, b_metadado_init;

    if (removal->prox == NULL)
    {
        b_metadado_init = 9 + offset + (removal->arquivo.indice * sizeof(FileInfo_t));
        b_metadado_fim = b_metadado_init + sizeof(FileInfo_t);
    }
    else
    {
        b_metadado_init = 9 + offset + (removal->arquivo.indice * sizeof(FileInfo_t)) + 1;
        b_metadado_fim = b_metadado_init + sizeof(FileInfo_t);
    }

    int retorno = remove_bytes(arquivador, b_metadado_init, b_metadado_fim);

    if (retorno != 0)
        printf("retorno: %d\n", retorno);

    unsigned int b_arq_init = removal->arquivo.posicao;
    unsigned int b_arq_final = removal->arquivo.posicao + removal->arquivo.tam - 1;

    /* Remove o conteúdo do membro */
    int rt = remove_bytes(arquivador, b_arq_init + 1, b_arq_final + 1);
    if (rt)
        return 1;

    return 0;
}

void atualizar_posicoes_arq(dir_t *diretorio)
{
    int posicao = 9;
    nodo_t *nodo = diretorio->head;

    while (nodo != NULL)
    {
        nodo->arquivo.posicao = posicao;
        posicao += nodo->arquivo.tam;
        nodo = nodo->prox;
    }
}

int move_membro(FILE *arquivador, dir_t *diretorio, const char *name, const char *name2)
{
    nodo_t *arquivo_movido = buscarArquivoPorNome(diretorio, name);
    nodo_t *arquivo_destino = buscarArquivoPorNome(diretorio, name2);
    rewind(arquivador);

    if (arquivo_movido == NULL || arquivo_destino == NULL)
    {
        return 1;
    }

    unsigned int b_arq_init, b_arq_final, rt;
    int destino;

    /*move do metadados na lista*/
    if (arquivo_destino->arquivo.indice < arquivo_movido->arquivo.indice)
    {
        moverNoParaIndice(diretorio, arquivo_movido, arquivo_destino->arquivo.indice + 1);
    }
    else
        moverNoParaIndice(diretorio, arquivo_movido, arquivo_destino->arquivo.indice);

    /*move o conteudo do membro*/

    destino = arquivo_destino->arquivo.posicao + arquivo_destino->arquivo.tam;
    b_arq_init = arquivo_movido->arquivo.posicao;
    b_arq_final = arquivo_movido->arquivo.posicao + arquivo_movido->arquivo.tam - 1;

    if (destino > b_arq_final)
    {
        destino = arquivo_destino->arquivo.posicao + arquivo_destino->arquivo.tam - arquivo_movido->arquivo.tam;
    }

    if (destino == b_arq_init)
        return 1;

    rt = move_bytes(arquivador, b_arq_init, b_arq_final, destino);
    if (rt)
        return 2;

    printa_metadados_lista(diretorio, arquivador);
    atualizar_posicoes_arq(diretorio);
    return 0;
}

void ler_conteudo(const char *nome_arquivo, FILE *backup, int posicao, int block)
{
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    if (backup == NULL)
    {
        perror("Erro ao abrir o arquivo de backup");
        fclose(arquivo);
        return;
    }

    char buffer[1024];
    size_t bytes_lidos;

    if (block > sizeof(buffer))
    {
        while (block > 0 && (bytes_lidos = fread(buffer, 1, sizeof(buffer), arquivo)) > 0)
        {
            fseek(backup, posicao, SEEK_SET);
            fwrite(buffer, 1, bytes_lidos, backup);
            posicao += bytes_lidos;
            block -= bytes_lidos;
        }
    }
    else
    {
        bytes_lidos = fread(buffer, 1, block, arquivo);
        fseek(backup, posicao, SEEK_SET);
        fwrite(buffer, 1, bytes_lidos, backup);
    }

    fclose(arquivo);
}

void inserir_arq(const char *nome_arquivo, dir_t *diretorio, FILE *arquivador, long long *offset)
{
    struct stat file_stat;
    if (stat(nome_arquivo, &file_stat) == -1)
    {
        perror("Erro ao obter as informações do arquivo");
        return;
    }

    FileInfo_t arquivo;
    strncpy(arquivo.nome, nome_arquivo, sizeof(arquivo.nome));
    arquivo.tam = file_stat.st_size;
    arquivo.st_dev = file_stat.st_dev;
    arquivo.permissoes = file_stat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    arquivo.ult_modif = file_stat.st_mtime;
    arquivo.UserID = file_stat.st_uid;
    arquivo.GroupID = file_stat.st_gid;
    if (diretorio->qntd == 0)
    {
        arquivo.posicao = sizeof(long long) + *offset + 1;
    }
    else
    {
        nodo_t *ultimoArquivo = diretorio->ult;
        arquivo.posicao = ultimoArquivo->arquivo.posicao + ultimoArquivo->arquivo.tam;
    }

    int posicao = arquivo.posicao;
    int block = arquivo.posicao + arquivo.tam - 1;
    adiciona_arq_lista(diretorio, &arquivo);

    ler_conteudo(nome_arquivo, arquivador, posicao, block);

    atualizar_posicoes_arq(diretorio);
    printa_metadados_lista(diretorio, arquivador);
    *offset += arquivo.tam;
}

void carregar_metadados_lista(dir_t *diretorio, FILE *arquivador)
{
    rewind(arquivador);
    long long offset;
    fread(&offset, sizeof(long long), 1, arquivador);
    fseek(arquivador, offset + 9, SEEK_SET);

    FileInfo_t arquivo;
    while (fread(&arquivo, sizeof(FileInfo_t), 1, arquivador) == 1)
    {
        adiciona_arq_lista(diretorio, &arquivo);
    }
}

int copiar_arquivo_do_arquivador(const char *nome_arquivo, FILE *arquivador)
{
    DIR *dir = opendir(".");
    if (dir == NULL)
    {
        perror("Erro ao abrir o diretório atual");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        struct stat st;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", ".", entry->d_name);
        if (stat(path, &st) == 0 && S_ISREG(st.st_mode))
        {
            if (strcmp(entry->d_name, nome_arquivo) == 0)
            {
                char nome_copia[256];
                snprintf(nome_copia, sizeof(nome_copia), "copia_%s", nome_arquivo);

                FILE *arquivo_original = fopen(nome_arquivo, "rb");
                if (arquivo_original == NULL)
                {
                    perror("Erro ao abrir o arquivo original");
                    closedir(dir);
                    return 1;
                }

                FILE *arquivo_copia = fopen(nome_copia, "wb");
                if (arquivo_copia == NULL)
                {
                    perror("Erro ao criar o arquivo de cópia");
                    fclose(arquivo_original);
                    closedir(dir);
                    return 2;
                }

                char buffer[1024];
                size_t bytes_lidos;
                while ((bytes_lidos = fread(buffer, 1, sizeof(buffer), arquivo_original)) > 0)
                {
                    fwrite(buffer, 1, bytes_lidos, arquivo_copia);
                }

                printf("Arquivo copiado com sucesso: %s\n", nome_copia);
                return 0;
                break;
            }
        }
    }
    return 0;
    closedir(dir);
}