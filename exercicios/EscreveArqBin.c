#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Abrindo o arquivo binário para leitura e escrita
    FILE *fp = fopen("poema_saida.bin", "rb+");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    
    // Obtendo o tamanho do arquivo
    fseek(fp, 0L, SEEK_END);
    long int size = ftell(fp);
    rewind(fp);
    
    // Lendo o conteúdo do arquivo
    char *buffer = (char *) malloc(size * sizeof(char));
    fread(buffer, sizeof(char), size, fp);
    
    // Encontrando a posição da palavra "vida" no arquivo
    char *pos = strstr(buffer, "vida");
    if (pos == NULL) {
        printf("A palavra 'vida' não foi encontrada no arquivo.\n");
        return 1;
    }
    
    // Inserindo a palavra "boa" antes da palavra "vida"
    int offset = pos - buffer;
    char *new_buffer = (char *) malloc((size + 4) * sizeof(char));
    strncpy(new_buffer, buffer, offset);
    strncpy(new_buffer + offset, "boa ", 4);
    strncpy(new_buffer + offset + 4, buffer + offset, size - offset);
    
    // Escrevendo o novo conteúdo no arquivo
    rewind(fp);
    fwrite(new_buffer, sizeof(char), size + 4, fp);
    
    // Liberando a memória alocada
    free(buffer);
    free(new_buffer);
    
    // Fechando o arquivo
    fclose(fp);
    
    return 0;
}
