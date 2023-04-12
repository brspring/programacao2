#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 1024

int main()
{
    char line[LINESIZE+1];
    FILE *file = NULL;
    
    file = fopen("mapa.txt", "r");
    if (file == NULL){
        printf("Error opening file\n");
        exit(1);
    }

    fgets (line, LINESIZE, file);
    
    int a, b;
    char *token;

    token = strtok(line, " ");
    a = atoi(token);

    token = strtok(NULL, " ");
    b = atoi(token);

    char **matriz;

    matriz = (char **) malloc(a * sizeof(char *));
    for (int k = 0; k < a; k++)
        matriz[k] = (char *) malloc(b * sizeof(char));

    char c;
    int i, j;

    while (fread(&c, sizeof(char), 1, file)){
        for (i = 0; i < a; i++){ /* roda todas as linhas */
            for (j = 0; j < b; j++){ /* roda as colunas */
                matriz[i][j] = c;
                fread(&c, sizeof(char), 1, file);
            }
            fread(&c, sizeof(char), 1, file);
        }
    }

    for (i = 0; i < a; i++){
        for (j = 0; j < b; j++){
            printf("%c", matriz[i][j]);
        }
        printf("\n");
    }
}