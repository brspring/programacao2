#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 100 // tamanho máximo de uma palavra

int main() {
    char filename[MAX_WORD_LEN]; // nome do arquivo a ser lido
    char word[MAX_WORD_LEN]; // palavra sendo lida do arquivo
    char *new_text = NULL; // novo texto com as palavras modificadas
    char *p = NULL; // ponteiro para percorrer o texto
    int text_size = 0; // tamanho do texto original
    int new_text_size = 0; // tamanho do novo texto
    int found_vida = 0; // flag indicando se a palavra "vida" foi encontrada
    FILE *fp; // ponteiro para o arquivo

    printf("Digite o nome do arquivo: ");
    scanf("%s", filename);

    fp = fopen(filename, "rb"); // abre o arquivo para leitura em modo binário

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // lê o texto original
    while (fscanf(fp, "%s", word) == 1) {
        text_size += strlen(word) + 1; // conta o tamanho da palavra + espaço
    }

    // aloca memória para o novo texto
    new_text = (char *) malloc(text_size + 1); // +1 para o caractere nulo

    // volta ao início do arquivo
    fseek(fp, 0, SEEK_SET);

    // percorre o texto original, inserindo a palavra "boa" antes de "vida"
    while (fscanf(fp, "%s", word) == 1) {
        if (strcmp(word, "vida") == 0) {
            strcat(new_text, "boa "); // insere a palavra "boa"
            new_text_size += 4; // tamanho de "boa "
            found_vida = 1;
        }

        strcat(new_text, word); // adiciona a palavra lida
        new_text_size += strlen(word);

        if (!found_vida) {
            strcat(new_text, " "); // adiciona um espaço
            new_text_size += 1;
        }

        found_vida = 0;
    }

    fclose(fp); // fecha o arquivo

    printf("Texto original:\n");
    printf("%s\n", new_text);

    free(new_text); // libera a memória alocada

    return 0;
}