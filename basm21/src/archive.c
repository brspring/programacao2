#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <time.h>
#include "archive.h"
#include "archiver.h"

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
    fseek(arquivador, offset + 8, SEEK_SET);
    nodo_t *atual = diretorio->head;
    while (atual != NULL)
    {
        adiciona_metadados(atual->arquivo, arquivador);
        atual = atual->prox;
    }
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

    /*remove os metadados do membro*/
    b_metadados_init = sizeof(long long) + offset;
    removerNo(diretorio, removal);
    fseek(arquivador, b_metadados_init, SEEK_SET);
    printa_metadados_lista(diretorio, arquivador);

    /*remove o conteudo do membro*/
    rt = remove_bytes(arquivador, b_arq_init, b_arq_final);
    if (rt)
    {
        return 1;
    }

    printa_metadados_lista(diretorio, arquivador);
    return 0;
}

void atualizar_posicoes_arq(dir_t *diretorio) {
    int posicao = 9;
    nodo_t *nodo = diretorio->head;

    while (nodo != NULL) {
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

    unsigned int b_arq_init, b_arq_final, rt, rt2, b_metadados_init, b_metadados_tam, b_metadados_final;
    int destino;

    /*move do metadados na lista*/
    if(arquivo_destino->arquivo.indice < arquivo_movido->arquivo.indice){
        moverNoParaIndice(diretorio, arquivo_movido, arquivo_destino->arquivo.indice + 1);
    } else
        moverNoParaIndice(diretorio, arquivo_movido, arquivo_destino->arquivo.indice);
    
    /*move o conteudo do membro*/
    
    destino = arquivo_destino->arquivo.posicao + arquivo_destino->arquivo.tam;
    b_arq_init = arquivo_movido->arquivo.posicao;
    b_arq_final = arquivo_movido->arquivo.posicao + arquivo_movido->arquivo.tam - 1;

    if(destino > b_arq_final){
        destino = arquivo_destino->arquivo.posicao + arquivo_destino->arquivo.tam - arquivo_movido->arquivo.tam;
    }

    if(destino == b_arq_init)
        return 1;

    rt = move_bytes(arquivador, b_arq_init, b_arq_final, destino);
    if (rt)
    {
        return 2;
    }

    printa_metadados_lista(diretorio, arquivador);
    atualizar_posicoes_arq(diretorio);
}


int main()
{

    FILE *arquivador = fopen("backup.vpp", "wb+");
    struct stat f_data;
    long long offset = 0;
    if (arquivador == NULL)
    {
        perror("Erro ao abrir o arquivo de arquivador\n");
        return 1;
    }

    /*---------------------------------TESTE------------------------------------*/
    dir_t diretorio;
    diretorio.qntd = 0;
    diretorio.head = NULL;
    diretorio.ult = NULL;

    FileInfo_t arquivo1;
    strcpy(arquivo1.nome, "a.txt");
    arquivo1.posicao = sizeof(long long) + offset + 1;
    arquivo1.indice = 0;
    arquivo1.tam_inic = 8;
    arquivo1.tam = 8;
    arquivo1.st_dev = 0;
    arquivo1.permissoes = 777;
    arquivo1.ult_modif = time(NULL);
    arquivo1.UserID = getuid();
    arquivo1.GroupID = getgid();

    adiciona_arq_lista(&diretorio, &arquivo1);
    offset = calcula_offset(arquivador, diretorio);

    FileInfo_t arquivo2;
    strcpy(arquivo2.nome, "b.txt");
    arquivo2.posicao = sizeof(long long) + offset + 1;
    arquivo2.indice = 1;
    arquivo2.tam_inic = 4;
    arquivo2.tam = 4;
    arquivo2.st_dev = 0;
    arquivo2.permissoes = 777;
    arquivo2.ult_modif = time(NULL);
    arquivo2.UserID = getuid();
    arquivo2.GroupID = getgid();

    adiciona_arq_lista(&diretorio, &arquivo2);
    offset = calcula_offset(arquivador, diretorio);

    FileInfo_t arquivo3;
    strcpy(arquivo3.nome, "c.txt");
    arquivo3.posicao = sizeof(long long) + offset + 1;
    arquivo3.indice = 2;
    arquivo3.tam_inic = 8;
    arquivo3.tam = 8;
    arquivo3.st_dev = 0;
    arquivo3.permissoes = 777;
    arquivo3.ult_modif = time(NULL);
    arquivo3.UserID = getuid();
    arquivo3.GroupID = getgid();

    adiciona_arq_lista(&diretorio, &arquivo3);
    offset = calcula_offset(arquivador, diretorio);

    fwrite(&offset, sizeof(long long), 1, arquivador);

    char buffer[8];

    memset(buffer, 'a', 8);
    fwrite(buffer, sizeof(char), 8, arquivador);

    memset(buffer, 'b', 4);
    fwrite(buffer, sizeof(char), 4, arquivador);

    memset(buffer, 'c', 8);
    fwrite(buffer, sizeof(char), 8, arquivador);

    /*-------------------------------------------------------------------*/
    const char *name = "a.txt";
    const char *name2 = "b.txt"; 
    move_membro(arquivador, &diretorio, name, name2);

    print_lista(&diretorio);
    fclose(arquivador);
    liberarDiretorio(&diretorio);
}