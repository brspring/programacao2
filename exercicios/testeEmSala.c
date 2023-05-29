#include <stdio.h>
#include <stdlib.h>

#define BUFFER_MAX 4

int main(){
    FILE *input = fopen("teste1", "rb");

    if(input == NULL){
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }   

    FILE* output = fopen("teste2", "wb+");

    if(output == NULL){
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    char buffer[BUFFER_MAX];
    int check_read, check_write;

    while(!feof(input))
    {
        check_read = fread(&buffer, sizeof(char), 4, input);
        for(int i=0; i<check_read; i++)
        {
            
        }
        check_write = fwrite(&buffer, sizeof(char), check_read, output);

        if (check_read != check_write)
        {
            printf("Erro ao ler/escrever o arquivo\n");
            exit(1);
        }

    }
    return 0;
}