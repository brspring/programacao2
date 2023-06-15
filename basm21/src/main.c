#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv) {
    char *destinoNome;
    char *vpp;
    int i, option;

    if(argc == 1)
        printf("Necessário mais argumentos!\n");

    while((option = getopt(argc, argv, "r:m:")) != -1) {
        switch(option) {
        case 'r':
            
            break;
        case 'm':

            break;
        /*case 'r':

            break;
        case '':*/
        default:
            printf("Opção inválida!\n");
            break;
        }
    }
}