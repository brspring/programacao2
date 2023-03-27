#include <stdio.h>
#include <string.h>
#define SIZE 100

int main(){
    char str[SIZE+1];
    int i,j;

    fgets(str, SIZE, stdin);

    while(str[i] != '\0'){
        j = i;
        if(str[j] == str[j+1]){
           j = i;
            while (str[j] != '\0'){
                str[j] = str[j+1];
                j++;
            }
        } else {                    
        i++ ;
        }
    }


    printf("String modificada: %s\n", str);
}