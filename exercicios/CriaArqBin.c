#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARQUIVO "numeros.dat"
int main()
{
    FILE *arq;
    int i, ret;

    // abre o arquivo em modo "append"
    arq = fopen(ARQUIVO, "wb");
    if (!arq)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    srand(time(NULL));

    double *LLints;
    int numVal = 100;
    LLints = malloc(sizeof(double)*numVal);

    // gera NUMVAL valores aleatórios reais
    for (i = 0; i < numVal; i++)
        LLints[i] = rand()*100;

    // escreve os valores gerados no final do arquivo
    ret = fwrite(LLints, sizeof(double), numVal, arq);
    if (ret)
        printf("Gravou %d valores com sucesso!\n", ret);
    else
        printf("Erro ao gravar...\n");

    // fecha o arquivo
    fclose(arq);
    return (0);
}