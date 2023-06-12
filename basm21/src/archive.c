#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "archive.h"

void saveArchiveToFile(Nodo_t* fileList, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    Nodo_t* current = fileList;
    while (current != NULL) {
        // Escrever nome do arquivo
        size_t nameLength = strlen(current->arquivo.name);
        fwrite(&nameLength, sizeof(size_t), 1, file);
        fwrite(current->arquivo.name, sizeof(char), nameLength, file);

        // Escrever tamanho do arquivo
        fwrite(&current->arquivo.tam, sizeof(size_t), 1, file);

        // Escrever permissões do arquivo
        fwrite(&current->arquivo.permissoes, sizeof(mode_t), 1, file);

        // Escrever data de modificação do arquivo
        fwrite(&current->arquivo.ult_modif, sizeof(time_t), 1, file);

        current = current->prox;
    }

    fclose(file);
}

int main() {
    Nodo_t* fileList = NULL;

    struct stat fileStat;
    if (stat("file.txt", &fileStat) == -1) {
        printf("Erro ao obter informações do arquivo.\n");
        return 1;
    }

    insertNodo(&fileList, "arquivo.txt", fileStat.st_size, fileStat.st_mode, fileStat.st_mtime);

    // Salvar a lista de arquivos em um arquivo .vpp
    saveArchiveToFile(fileList, "arquivo.vpp");

    freeNodoList(&fileList);

    return 0;
}