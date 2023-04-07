/*Escreva um programa em C que leia um arquivo de texto com números reais (um número por linha) e informe a média dos valores lidos.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_LINE_LENGTH 1024

int main()
{
    FILE *arq;
    float temp, media = 0;
    float valor;
    char line[MAX_LINE_LENGTH];
    int contador;

    arq = fopen("arquivoTexto.txt", "r");

    if (!arq)
    {
        perror("Erro em abrir o arquivo!\n");
        return 1;
    }

    fgets(line, MAX_LINE_LENGTH, arq);
    media = atof(line);
    contador = 1;

    while (fgets(line, MAX_LINE_LENGTH, arq) != NULL)
    {
        if (isspace(*line)) /*ignora linhas vazias*/
        {
            continue;
        }
        
        valor = atof(line);
        media = media + valor;
        contador++;
    }

    media = media / contador;  

    printf("A media é: %f\n", media);

    fclose(arq);
    return 0;
}