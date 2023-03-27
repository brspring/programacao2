#include <stdio.h>
#include <string.h>
#define SIZE 100

int main(){
    char str[SIZE+1]; //+1 pois tem o /0 no final da string

    printf("Digite a string que quer imprimir o contrário.\n");

    fgets (str, 100, stdin);

    for(int i = strlen(str)-1; i >= 0; i--){
        putchar(str[i]); 
    }
    printf("\n");
}
