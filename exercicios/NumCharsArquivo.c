/*Escreva um programa em C para informar o número de caracteres presentes em um arquivo de texto.*/

#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE* arq;
    char c;
    int cont;

    arq = fopen("arquivoTexto.txt", "r");

    if (! arq){
        perror("Erro ao abrir arquivo\n");
        exit(1) ; 
    }

    c = fgetc(arq); /*o C comeca armazenando o primeiro caracter do arquivo*/

    if(c == EOF) /*verifica se o arquivo possui algum caracter*/
    {
        printf("Arquivo nao possui nenhum caracter!\n"); 
        exit(1);
    }

    while(c != EOF){
        cont++;
        c = fgetc(arq);
    }

    printf("O arquivo possui %d caracters\n", cont);

    fclose(arq);
    return 0;
}   

