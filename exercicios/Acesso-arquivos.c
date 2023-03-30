/* Escreva um programa em C para informar o número de caracteres presentes em um arquivo de texto.
*/

#include<stdio.h>
#include<stdlib.h>

int main()
{
    FILE* arq;
    char c;
    int cont = 0;

    arq = fopen("arquivoTexto.txt", "r");

    if (!arq)
    {
        perror("Erro ao abrir arquivo arquivoTexto.txt");
        exit(1);
    }

    c = fgetc (arq) ;		// tenta ler um caractere   
     
    while (c != EOF)		// não é o fim do arquivo
    {		               
        c = fgetc (arq) ;   // tenta ler o próximo
        cont++;
    }

    printf("O arquivo possui %d caracters\n", cont);

    fclose(arq);
    exit(0);

}
