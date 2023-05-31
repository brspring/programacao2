/*Considere um arquivo de entrada fornecido. Você precisa ler o arquivo em buffers de 4 bytes e aplicar operações específicas em cada posição do buffer. Implemente um programa em C que realize as seguintes ações para cada buffer lido:

    Para o primeiro byte do buffer, converta-o para um caractere e exiba-o na tela.
    Para o segundo byte do buffer, verifique se é um número par. Se for par, imprima o valor na tela.
    Para o terceiro byte do buffer, verifique se é uma letra maiúscula. Se for uma letra maiúscula, imprima na tela.

O programa deve percorrer todo o arquivo, lendo os buffers e aplicando as operações em cada posição correspondente.

Observações:

    O arquivo de entrada deve ser fornecido como argumento na linha de comando ao executar o programa.
    Considere que o arquivo de entrada está no formato binário e possui tamanho suficiente para a leitura dos buffers.*/

#include <stdio.h>

int main() {
    FILE *file;
    unsigned char buffer[4];
    int count = 1;

    // Abre o arquivo de entrada
    file = fopen("arquivoTexto.txt", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    while (fread(buffer, sizeof(unsigned char), 4, file)) {
        printf("Byte numero: %d\n", count);
        // Para o primeiro byte do buffer, converte para um caractere e exibe na tela
        char firstByte = (char)buffer[0];
        printf("Primeiro byte: %c\n", firstByte);

        // Para o segundo byte do buffer, verifica se é um número par e imprime na tela se for par
        int secondByte = buffer[1];
        if (secondByte % 2 == 0) {
            printf("Segundo byte (número par): %d\n", secondByte);
        }

        // Para o terceiro byte do buffer, verifica se é uma letra maiúscula e imprime na tela se for maiúscula
        char thirdByte = (char)buffer[2];
        if (thirdByte >= 'A' && thirdByte <= 'Z') {
            printf("Terceiro byte (letra maiúscula): %c\n", thirdByte);
        }
        printf("-----------------------------------------\n");
        count ++;
    }

    // Verifica se houve algum elemento restante no último buffer lido
    if (count > 0) {
        char firstByte = (char)buffer[0];
        printf("Primeiro byte: %c\n", firstByte);
    }

    fclose(file);

    return 0;
}
