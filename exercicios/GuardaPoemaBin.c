#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    FILE *arq_in = fopen("poema.bin", "rb");

    if (!arq_in)
    {
        perror("Erro em abrir o arquivo de entrada!\n");
        return 1;
    }

    struct stat var;
    stat("poema.bin", &var);
    int filesize = var.st_size;

    FILE *arq_out = fopen("poema_saida.txt", "w");

    if (!arq_out)
    {
        perror("Erro em criar o arquivo de saída!\n");
        return 1;
    }

    char c;

    for (int posicao = 0; posicao < filesize; posicao += 5)
    {
        fread(&c, sizeof(char), 1, arq_in);
        fwrite(&c, sizeof(char), 1, arq_out);
    }

    fclose(arq_in);
    fclose(arq_out);
    return 0;
}
