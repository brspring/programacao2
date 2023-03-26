#include <stdio.h>
#include <string.h>
#define SIZE 100

int main(){
    char str[SIZE+1];
    int cont = 0;

    printf("Type the string you want to know the size\n");

    scanf("%s", str);   
    
    while (str[cont] != '\0')
    {
        cont++;
    }

    printf("Sua string tem %d de tamanho\n", cont);

    return 0;
}
