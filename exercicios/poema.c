/* arquivo poema.bin contém um poema de uma das mais importantes escritoras brasileiras. Esse arquivo binário contém registros de 5 bytes, sendo um inteiro e um char. O valor inteiro indica em que posição o char deve ser colocado na string.

Por exemplo, se um registro contém (em binário)

     250 a

isto indica que o caracter 'a' deve ser colocado na posição 250 da string.

Uma vez que todos os caracteres foram colocados na string, você deve mostrar a string na tela, a qual deverá conter o poema desta famosa poetisa.

Note que deve alocar dinamicamente a string que será utilizada para armazenar o poema.*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    FILE *arq = fopen("poema.bin", "r+");

    if (!arq)
    {
        perror("Erro em abrir o arquivo!\n");
        return 1;
    }

    struct stat var;
    stat("poema.bin", &var);
    int filesize = var.st_size;
    
/*
    // determinar tamanho do arquivo
    fseek(arq, 0, SEEK_END); //coloca o ponteiro no final do arq
    int filesize = ftell(arq); //retorna a posicao do ponteiro em relacao ao inicio do arquivo
    rewind(arq); //joga o ponteiro para o inicio novamente
*/
    // alocar espaço para a string
    char *poema = malloc(filesize);

    if (!poema)
    {
        perror("Erro em alocar memória!\n");
        return 1;
    }

    // construir a string
    int posicao;
    char c;

    while (fread(&posicao, sizeof(int), 1, arq) == 1)
    {
        fread(&c, sizeof(char), 1, arq);
        poema[posicao] = c;
    }

    poema[filesize] = '\0';

    // imprimir a string
    printf("%s\n", poema);

    // liberar a memória alocada
    free(poema);

    fclose(arq);
    return 0;
}