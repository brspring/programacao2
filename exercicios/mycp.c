/*Escreva um programa mycp para fazer a cópia de um arquivo em outro: mycp arq1 arq2.
Antes da cópia, arq1 deve existir e arq2 não deve existir. 
Mensagens de erro devem ser geradas caso essas condições não sejam atendidas ou 
o nome dado a arq2 seja inválido. Para acessar os nomes dos arquivos 
na linha de comando use os parâmetros argc e argv (veja aqui).*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 1024

int main(int argc, char **argv) {
    FILE *file1, *file2;
    char line[MAX_LINE_SIZE];

    //Verifica se o número de argumentos é válido
    if (argc != 3) {
        fprintf(stderr, "Erro: número de argumentos inválido\n");
        fprintf(stderr, "Uso: %s <arquivo_origem> <arquivo_destino>\n", argv[0]);
    }

    //Verifica se o arquivo de origem existe
    if ((file1 = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Erro: arquivo %s não encontrado\n", argv[1]);
    }

    //Verifica se o arquivo de destino não existe
    if ((file2 = fopen(argv[2], "r")) != NULL) {
        fprintf(stderr, "Erro: arquivo %s já existe\n", argv[2]);
        fclose(file1);
        fclose(file2);
    }

    //Abre o arquivo de destino para escrita
    if ((file2 = fopen(argv[2], "w")) == NULL) {
        fprintf(stderr, "Erro: não foi possível criar o arquivo %s\n", argv[2]);
        fclose(file1);
    }

    //Copia o conteúdo do arquivo de origem para o arquivo de destino
    while (fgets(line, MAX_LINE_SIZE, file1) != NULL) {
        fputs(line, file2);
    }

    //Fecha os arquivos e exibe uma mensagem de sucesso
    fclose(file1);
    fclose(file2);
    printf("Arquivo %s copiado para %s com sucesso.\n", argv[1], argv[2]);

    return 0;
}
