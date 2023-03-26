#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define SIZE 100

int main(){
    char str[SIZE+1];
    int i,j;

    printf("Enter the string you want to remove the special character: \n");
    fgets(str, SIZE, stdin);

    i = 0;
    while (str[i] != '\0')                
    {
        //a funcao isalnum faz o mesmo que esse if !((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') || str[i] == ' '
        if (!isalnum(str[i]) && str[i] != ' '){ 
            j = i;
            while (str[j] != '\0'){
                str[j] = str[j+1];  //move todos os caracteres apos o caracter especial
                                    //para a esquerda sobreescrevendo ele 
                j++;
            }
        } else {                    
        i++ ;
        }
    }

    printf("String modificada: %s", str);
}