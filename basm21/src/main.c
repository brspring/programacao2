#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <getopt.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <time.h>
#include "archive.h"
#include "archiver.h"

int main(int argc, char **argv)
{
    int i, rt, option;
    long long offset;
    struct stat file_stat;

    if (argc == 1)
    {
        printf("Necessário mais argumentos!\n");
        return 0;
    }

    FILE *arquivador = fopen("backup.vpp", "rb+");
    if (arquivador == NULL)
    {
        arquivador = fopen("backup.vpp", "wb");
        if (arquivador == NULL)
        {
            perror("Erro ao criar o arquivo de backup");
            return 1;
        }
    }

    dir_t diretorio;
    inicia_dir(&diretorio);

    while ((option = getopt(argc, argv, "r:m:i:c::x:h::")) != -1)
    {
        switch (option)
        {
        case 'r': // REMOVE
            carregar_metadados_lista(&diretorio, arquivador);
            atualizarIndices(&diretorio);
            const char *nomeMembro = argv[3];
            nodo_t *remove = buscarArquivoPorNome(&diretorio, nomeMembro);

            offset = calcula_offset(arquivador, diretorio);

            int resultado = remove_member(nomeMembro, &diretorio, arquivador);

            if (resultado != 0)
                printf("Erro ao remover o membro.\n");

            removerNo(&diretorio, remove);
            printa_metadados_lista(&diretorio, arquivador);

            long long offset2 = calcula_offset(arquivador, diretorio);
            printf("novo offset apos remocao: %lld\n", offset2);

            rewind(arquivador);
            fwrite(&offset2, sizeof(long long), 1, arquivador);
            rewind(arquivador);

            break;
        case 'm':
            const char *name = argv[4];
            const char *name2 = argv[2];
            carregar_metadados_lista(&diretorio, arquivador);

            int retorno;
            retorno = move_membro(arquivador, &diretorio, name, name2);

            if (retorno != 0)
            {
                printf("Erro ao mover o membro.\n");
            }
            rewind(arquivador);
            break;
        case 'i': // INSERE
            carregar_metadados_lista(&diretorio, arquivador);
            for (i = optind; i < argc; i++)
            {
                nodo_t *arquivoExistente = buscarArquivoPorNome(&diretorio, argv[i]);

                if (arquivoExistente != NULL)
                {
                    printf("Arquivo %s será substituído na lista e add ao final.\n", argv[i]);
                    nodo_t *remove = buscarArquivoPorNome(&diretorio, argv[i]);

                    int resultado = remove_member(argv[i], &diretorio, arquivador);

                    if (resultado != 0)
                        printf("Erro ao remover o membro.\n");

                    removerNo(&diretorio, remove);
                    printa_metadados_lista(&diretorio, arquivador);

                    long long offset2 = calcula_offset(arquivador, diretorio);

                    rewind(arquivador);
                    fwrite(&offset2, sizeof(long long), 1, arquivador);
                    rewind(arquivador);
                }

                inserir_arq(argv[i], &diretorio, arquivador, &offset);
                printf("Arquivo %s inserido\n", argv[i]);
            }

            offset = calcula_offset(arquivador, diretorio);
            fseek(arquivador, 0, SEEK_SET);
            fwrite(&offset, sizeof(long long), 1, arquivador);

            rewind(arquivador);
            atualizarIndices(&diretorio);
            atualizar_posicoes_arq(&diretorio);
            break;
        case 'c': // LISTA
            rewind(arquivador);
            carregar_metadados_lista(&diretorio, arquivador);
            printa_metadados_lista(&diretorio, arquivador);
            print_lista(&diretorio);
            break;
        case 'x':
            carregar_metadados_lista(&diretorio, arquivador);
            if (strcmp(argv[2], "backup.vpp") == 0)
            {
                if (argv[3] != NULL)
                {
                    rt = copiar_arquivo_do_arquivador(argv[3], arquivador);
                    if (rt != 0)
                        printf("erro\n");
                }
                else
                {
                    nodo_t *agr = diretorio.head;
                    while (agr != NULL)
                    {
                        printf("Extraindo arquivo %s\n", agr->arquivo.nome);
                        rt = copiar_arquivo_do_arquivador(agr->arquivo.nome, arquivador);
                        agr = agr->prox;
                    }
                }
            }
            rewind(arquivador);
            break;
        case 'h':
            printf("Opções:\n");
            printf("-r <nome arquivador> <nome do membro>\n");
            printf("-m <membro destino> <nome arquivador> <membro movido>\n");
            printf("-i <nome arquivador> <nome do membro>\n");
            printf("-c : lista os membros do arquivo e seus metadados\n");
            printf("-x <nome arquivador> <nome do arquivo> : extrai o membro do arquivo\n");
            printf("\n");
            printf("Em caso de erro, tente remover o arquivo backup.vpp e insira novos arquivos.\n");
            break;
        default:
            printf("Opção inválida!\n");
            break;
        }
    }

    atualizarIndices(&diretorio);
    atualizar_posicoes_arq(&diretorio);
    rewind(arquivador);
    printa_metadados_lista(&diretorio, arquivador);
    liberarDiretorio(&diretorio);
    fclose(arquivador);

    return 0;
}