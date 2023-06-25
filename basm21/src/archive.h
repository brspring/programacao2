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
};

typedef struct dir
{
    unsigned int qntd;
    nodo_t *head;
    nodo_t *ult;
} dir_t;

void inicia_dir(dir_t *diretorio);

long long le_offset(FILE *arquivador);

void adiciona_arq_lista(dir_t *diretorio, FileInfo_t *arquivo);

void carregar_metadados_lista(dir_t *diretorio, FILE *arquivador);

long long calcula_offset(FILE *arquivador, dir_t diretorio);

void print_lista(dir_t *diretorio);

void liberarDiretorio(dir_t *diretorio);

void atualizarIndices(dir_t *diretorio);

void removerNo(dir_t *diretorio, nodo_t *no);

void moverNoParaIndice(dir_t *diretorio, nodo_t *no, int indice);

nodo_t *buscarArquivoPorNome(dir_t *diretorio, const char *nome);

void adiciona_metadados(FileInfo_t arquivo, FILE *arquivador);

void printa_metadados_lista(dir_t *diretorio, FILE *arquivador);

int remove_member(const char *name, dir_t *diretorio, FILE *arquivador);

void atualizar_posicoes_arq(dir_t *diretorio);

int move_membro(FILE *arquivador, dir_t *diretorio, const char *name, const char *name2);

void ler_conteudo(const char *nome_arquivo, FILE *backup, int posicao, int block);

void inserir_arq(const char *nome_arquivo, dir_t *diretorio, FILE *arquivador, long long *offset);

int copiar_arquivo_do_arquivador(const char *nome_arquivo, FILE *arquivador);

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

/*TESTE DA FUNCAO DE MOVER NO PARA INDICE

    c
const char *name = "a.txt";
    const char *name2 = "b.txt";

    nodo_t *noParaMover = buscarArquivoPorNome(&diretorio, name);
    printf("noParaMover: %s\n", noParaMover->arquivo.nome);

    if (noParaMover != NULL)
    {
        moverNoParaIndice(&diretorio, noParaMover, 1);
    }*/

/*
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
    */
/*-------------------------------------------------------------------*/
/*void extrai_arquivo_para_diretorio_atual(const char *nome_arquivo, const char *arquivador_nome,  dir_t *diretorio)
{
    FILE *arquivador = fopen(arquivador_nome, "rb");
    if (arquivador == NULL) {
        perror("Erro ao abrir o arquivo arquivador");
        return;
    }

    nodo_t *nodo = diretorio->head;
    while (nodo != NULL) {
        if (strcmp(nodo->arquivo.nome, nome_arquivo) == 0) {
            char caminho_destino[1024];
            
            FILE *arquivo_destino = fopen(nome_arquivo, "wb");
            if (arquivo_destino == NULL) {
                perror("Erro ao abrir o arquivo de destino");
                fclose(arquivador);
                return;
            }

            // Copia o conteúdo do arquivo do arquivador para o destino
            fseek(arquivador, nodo->arquivo.posicao, SEEK_SET);
            char buffer[1024];
            size_t bytes_lidos;

            while ((bytes_lidos = fread(buffer, 1, sizeof(buffer), arquivador)) > 0) {
                fwrite(buffer, 1, bytes_lidos, arquivo_destino);
            }

            fclose(arquivo_destino);
            printf("Arquivo copiado do arquivador para o diretório atual: %s\n", caminho_destino);
            break;
        }

        nodo = nodo->prox;
    }

    fclose(arquivador);
}*/

/*
EXTRACT

copiar_arquivo_do_arquivador("b.txt", arquivador);
*/

