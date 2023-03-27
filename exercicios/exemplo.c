#include <stdio.h>

int main ()
{
  // quest�o in�til
  printf ("Qu�o saud�vel voc� est�, G�nter?\n") ;
  return (0) ;
}

/* pedia para converter o texto para UTF-8, eu usei
os comando file e iconv para isso da seguinte forma:
- iconv -f ISO-8859-15 -t UTF-8 < exemplo.c  > convertido.c
- e depois "file convertido.c" que ve se converteu
*/