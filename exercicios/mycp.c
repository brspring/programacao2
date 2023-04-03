#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


#define LINESIZE 1024

int main(int argc, char **argv){
    FILE *arq, *arq2;
    char line[LINESIZE+1];
    char *auxLine;

    arq = fopen (argv[1], "r");
    arq2 = fopen (argv[2], "w+");

    if ( ! arq )
    {
      perror ("Erro ao abrir arquivo x") ;
      exit (1) ; // encerra o programa com status 1
    }

    while(!feof(argv[1]))
    {
        fgets();
        fputs();
    }

    fclose(argv[1], argv[2]);
    return 0;
}