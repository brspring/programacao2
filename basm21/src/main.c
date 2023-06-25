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
    int i,rt, option;
    long long offset;

    if (argc == 1)
    {
        printf("Necessário mais argumentos!\n");
        return 0;
    }

    FILE *arquivador = fopen("backup.vpp", "rb+");
    if (arquivador == NULL)
    {
        arquivador = fopen("backup.vpp", "wb+");
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

            atualizar_posicoes_arq(&diretorio);
            rewind(arquivador);
            fwrite(&offset2, sizeof(long long), 1, arquivador);

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

            atualizarIndices(&diretorio);
            atualizar_posicoes_arq(&diretorio);
            break;
        case 'i': // INSERE
            carregar_metadados_lista(&diretorio, arquivador);
            
            for (i = optind; i < argc; i++)
            {
                printf("Inserindo arquivo %s\n", argv[i]);
                inserir_arq(argv[i], &diretorio, arquivador, &offset);
            }

            offset = calcula_offset(arquivador, diretorio);
            printf("Offset: %lld\n", offset);
            fseek(arquivador, 0, SEEK_SET);
            fwrite(&offset, sizeof(long long), 1, arquivador);

            atualizarIndices(&diretorio);
            atualizar_posicoes_arq(&diretorio);

            break;
        case 'c': // LISTA
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

    // print_lista(&diretorio);
    printa_metadados_lista(&diretorio, arquivador);
    liberarDiretorio(&diretorio);
    fclose(arquivador);

    return 0;
}