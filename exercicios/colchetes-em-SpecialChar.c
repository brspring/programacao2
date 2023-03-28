#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 100

int main() {
    char str[SIZE+1];
    int i, j;
    char temp;

    printf("Digite uma string: ");
    fgets(str, sizeof(str), stdin);

    // Adiciona colchetes ao redor do caractere 'A'
    for (i = 0; str[i] != '\0'; i++) {
        temp = str[i];
        if (!isalnum(str[i]) && str[i] != ' ' && str[i] != '\0' && str[i] != '\n') {
            for (j = strlen(str); j > i; j--) {
                str[j+2] = str[j];
            }
            str[i] = '[';
            str[i+1] = temp;
            str[i+2] = ']';
            i += 2;
        }
    }

    printf("String resultante: %s\n", str);

    return 0;
}
