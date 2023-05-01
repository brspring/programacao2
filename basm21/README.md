# Cifras-de-Beale
Criptografia e Descriptografia com uma variante da Cifra de livro.

NOME: Bruno Aziz Spring Machado   GRR: GRR20211279

## Estrutura

O meu projeto está separado em tres arquivos principais, onde o ’main.c’ se trata do principal dos três, os secundários
sao: ’listaBeale.c’ e ’codifica decodifica.c’. E, por fim, o ’Makefile’ que serve para a execução do programa.

A estrutura do meu programa  é um vetor de listas (uma matriz, basicamente), onde o indice do vetor  é o numero na
tabela ASCII dos caracteres, que é inicializado da seguinte forma:

  nodo_l_t *vetorASCII[128] = {NULL};
  
- Como é possível observar, o vetor é do tipo "nodo_l_t", que é a seguinte estrutura: 
```c
  struct nodo_l {
    int elemento;       
    int posicao;
    struct nodo_l* prox; 
   };
  typedef struct nodo_l nodo_l_t;
```
Essa é a estrutura da lista utilizada, que possui um elemento, a posição e um ponteiro para o próximo.

## Função Main

Neste arquivo, faço a inclusão dos arquivos de cabeçalho dos meus outros dois programas para uso de suas funções. Neste arquivo principal, eu trato os três modelos de execução solicitados:

    ./beale -e -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves
    ./beale -d -i MensagemCodificada -c ArquivoDeChaves -o MensagemDecodificada
    ./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada

Para a verificação de qual comando será utilizado, é usado o argc, com isso vemos a quantidade de argumentos passados. Pode ser notado nas execuções acima que o primeiro possui 10 e os outros dois possuem 8 argumentos. Para verificar essa diferença dos dois com 8, eu verifico se o quinto argumento é -c ou -b, faço isso da seguinte forma: 

```c
/*Codifica e cria o arquivo de chaves.*/
    if (argc == 10)
    {
      //codigo da codificação e criação do arquivo de chaves
    }
```
```c
/*decodifica com um arquivo de chaves*/
    if(argc == 8 && (strcmp(argv[4], "-c") == 0) ){
      //codigo da decodificacao com arquivo de chaves
    }
```
```c
  /*Decodifica somente com o livro.*/
    if(argc == 8 && (strcmp(argv[4], "-b") == 0) ){
      //codigo com a decodificação com o arquivo de chaves
    }
```
