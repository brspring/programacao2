#Cifras-de-Beale
Criptografia e Descriptografia com uma variante da Cifra de livro.

NOME: Bruno Aziz Spring Machado   GRR: GRR20211279

O meu projeto está separado em tres arquivos principais, onde o ’main.c’ se trata do principal dos três, os secundários
sao: ’listaBeale.c’ e ’codifica decodifica.c’. E, por fim, o ’Makefile’ que serve para a execução do programa.

A estrutura do meu programa  é um vetor de listas (uma matriz, basicamente), onde o indice do vetor  é o numero na
tabela ASCII dos caracteres, que é inicializado da seguinte forma:

  nodo_l_t *vetorASCII[128] = {NULL};
  
Como é possível observar, o vetor é do tipo "nodo_l_t", que é a seguinte estrutura: 

  struct nodo_l {
    int elemento;       
    int posicao;
    struct nodo_l* prox; 
   };
  typedef struct nodo_l nodo_l_t;


