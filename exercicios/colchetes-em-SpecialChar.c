#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define SIZE 100

int main(){
    char str[SIZE+1];
    int i,j;

    fgets(str, SIZE, stdin);
    i = 0;
    while (str[i] != '\0')                
    {
        if (!isalnum(str[i]) && str[i] != ' '){ 
            j = i;
            while (str[j] != '\0'){
                str[j+1] = str[j+2];
                j++;
            }
        } else {                    
        i++ ;
        }
    }

    printf("String modificada: %s\n", str);
}