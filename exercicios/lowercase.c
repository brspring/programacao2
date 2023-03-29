#include <stdio.h>
#include <string.h>
#define SIZE 100

int main(){
    char str[SIZE+1];
    int i = 0;

    fgets(str, SIZE, stdin);

    while (str[i] != '\0')                
    {
        if (str[i] >= 'A' && str[i] <= 'Z') 
            str[i] += 32 ;                    
        i++ ;
    }

    for (i = 0; i < strlen(str); i++)
        putchar(str[i]);
    //puts(str) teria o mesmo efeito que esse for
    return 0;
}
