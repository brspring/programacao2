#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    FILE *arq = fopen("integers.bin", "rb");

    if (!arq)
    {
        perror("Erro para abrir o arquivo!\n");
        return 1;
    }

    /*tamanho do arquivo*/
    struct stat var;
    stat("integers.bin", &var);
    int filesize = var.st_size;
    filesize = filesize / sizeof(int);

    // Alocar memória para armazenar os números
    int *numeros = malloc(filesize * sizeof(int));
    if (numeros == NULL)
    {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    int numIntegersToRead = 10;
    // Ler os números do arquivo binário e armazená-los no array
    fread(numeros, sizeof(int), filesize, arq);

    // Exibir os primeiros 10 números
    printf("Primeiros 10 números:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", numeros[i]);
    }
    printf("Ultimos numeros: \n");
    fseek(arq, -numIntegersToRead * sizeof(int), SEEK_END);
    fread(numeros, sizeof(int), 10, arq);
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", numeros[i]);
    }

    free(numeros);
    fclose(arq);
    return 0;
}