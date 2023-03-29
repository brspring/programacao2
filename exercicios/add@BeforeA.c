#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

int main(){
    char str[SIZE+1];
    int i, j, temp;

    fgets(str, strlen(str), stdin);

    for(i=0; i < strlen(str); i++){
        if(str[i] == 65 || str[i] == 97){
            for(j = strlen(str); j > i ; j--){
                str[j] = str[j+1];
            }
            str[i+1] = str[i];
            str[i] = '@';
        }
    }
    temp = strlen(str);
    str[temp+1] = '\0';

    puts(str);

    return 0;
}