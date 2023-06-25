#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <time.h>
#include "archive.h"
#include "archiver.h"

int main(int argc, char **argv)
{
    char *destinoNome;
    int i, option;
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

    struct stat f_data;

    dir_t diretorio;
    inicia_dir(&diretorio);

    while ((option = getopt(argc, argv, "r:m:i:c::")) != -1)
    {
        switch (option)
        {
        case 'r': // REMOVE  
            carregar_metadados_lista(&diretorio, arquivador);
            atualizarIndices(&diretorio);
            const char *nomeMembro = argv[2];
            nodo_t *remove = buscarArquivoPorNome(&diretorio, nomeMembro);

            int tamantigo = get_size(arquivador);
            offset = calcula_offset(arquivador, diretorio);
            /* Remove o metadado do arquivo */

            //remover os bytes do metadados
            int b_metadado_init = 9 + offset + (remove->arquivo.indice * sizeof(FileInfo_t)) + 1;
            int b_metadado_fim = b_metadado_init + sizeof(FileInfo_t);
            int rt = remove_bytes(arquivador, b_metadado_init, b_metadado_fim );
            if (rt != 0)
                printf("erro\n");
            
            //remover os bytes do conteudo
            int resultado = remove_member(nomeMembro, &diretorio, arquivador);

            if (resultado != 0)
                printf("Erro ao remover o membro.\n");

            removerNo(&diretorio, remove);
            printa_metadados_lista(&diretorio, arquivador);

            long long offset2 = calcula_offset(arquivador, diretorio);
            printf("novo offset apos remocao: %lld\n", offset2);

            rewind(arquivador);
            fwrite(&offset2, sizeof(long long), 1, arquivador);

            break;
        case 'm':
            // Lógica para tratar a opção '-m'
            break;
        case 'i': // INSERE
            carregar_metadados_lista(&diretorio, arquivador);

            for (i = optind - 1; i < argc; i++)
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