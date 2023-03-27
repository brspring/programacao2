#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100

char *rmvSpecialChar(char str[]) {
    int i, j;
    char *new_str = (char *) malloc(sizeof(char) * (strlen(str) + 1)); // alocando memoria para a nova string
    if (new_str == NULL) {
        printf("Erro ao alocar memoria!\n");
    }
    
    i = 0;
    j = 0;
    while (str[i] != '\0') {
        if (isalnum(str[i])) { // se for um caractere alfanumerico
            new_str[j] = tolower(str[i]); // converte para minusculas e adiciona a nova string
            j++;
        }
        i++;
    }
    new_str[j] = '\0'; // adiciona o terminador de string a nova string
    
    return new_str;
}

int main() {
    char str[SIZE+1];
    int temp;
    fgets(str, SIZE, stdin);

    char *new_str = rmvSpecialChar(str);
    char backwards_string[strlen(new_str)+1];
    printf("%s\n", new_str);

    int i = 0;
    int j = strlen(new_str) - 1;
    
    while (i < strlen(new_str) && j >= 0) {
        backwards_string[i] = new_str[j];
        i++;
        j--;
    }

    backwards_string[i] = '\0';

    printf("%s\n", backwards_string);

    temp = strcmp(new_str, backwards_string);
    //printf("%p", strcmp);
    if (temp == 0){
        printf("Eh um palindromo!\n");
    }else{ 
        printf("Nao eh um palindromo\n");
    }
    free(new_str); // liberando a memoria alocada
    
    return 0;
}