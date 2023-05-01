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

## Arquivo Main

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

## Arquivo codifica-decodifica

Este arquivo possui todas as funções que uso na codificação e na decodificação, as mais relevantes são:

- a funcao 'cria_vetor_de_lista_com_livro', que popula  o vetor de listas conforme le o arquivo de chaves e acha as letras, usando o ':' como separador.

```c
void cria_vetor_de_lista_com_arq_de_chaves(nodo_l_t **vetorASCII)
{
    FILE *arquivo_de_chaves;
    arquivo_de_chaves = fopen("ArquivoDeChaves.txt", "r");

    if (arquivo_de_chaves == NULL)
    {
        perror("Erro em abrir o arquivo de chaves!\n");
        exit(1);
    }

    char linha[10000];

    while (fgets(linha, 10000, arquivo_de_chaves) != NULL)
    {
        char *separador = strtok(linha, ": \n"); /* quebra a linha  com base nos : \n"*/

        if (separador != NULL)
        {
            int indice = separador[0]; /*a primeira letra que é o indice do vetor*/
            indice = tolower(indice);
            separador = strtok(NULL, ": \n"); /*vai para o indice*/

            while (separador != NULL) /*vai por separadores*/
            {
                int posicao;
                if (sscanf(separador, "%d", &posicao) == 1) /* verifica se o separador pode ser inteiro*/
                {
                    if (vetorASCII[indice] == NULL) /*se a lista da letra n existe*/
                    {
                        vetorASCII[indice] = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                        vetorASCII[indice]->elemento = posicao;
                        vetorASCII[indice]->prox = NULL;
                    }
                    else /*se ja existe adiciona*/
                    {
                        nodo_l_t *atual = vetorASCII[indice];
                        while (atual->prox != NULL)
                        {
                            atual = atual->prox;
                        }
                        atual->prox = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                        atual->prox->elemento = posicao;
                        atual->prox->prox = NULL;
                    }
                }
                separador = strtok(NULL, ": \n"); /* avanca para o próximo separador*/
            }
        }
    }
    
    rewind(arquivo_de_chaves);
}
```
- a funcao 'cria_vetor_de_lista_com_livro', que faz o mesmo que a anterior, porém agora lendo direto do livro passado.
```c
  void cria_vetor_de_lista_com_livro(FILE *arquivo, nodo_l_t **vetorASCII)
{
    char c;
    int posicao = 0;

    bool primeiraLetra = true;

    while ((c = fgetc(arquivo)) != EOF)
    {
        c = tolower(c);
        if (isalnum(c) || c == '!' || c == '@' || c == '(' || c == '$' || c == '#' || c == '-' || c == '&' || c == '%' || c == '=' || c == '"')
        {
            int index_pos = c;
            if (primeiraLetra && c != '\n' && c != ' ')
            {
                if (vetorASCII[index_pos] == NULL)
                {
                    vetorASCII[index_pos] = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                    vetorASCII[index_pos]->elemento = posicao;
                    vetorASCII[index_pos]->prox = NULL;
                }
                else /*cria as outras posicoes das listas*/
                {
                    nodo_l_t *atual = vetorASCII[index_pos];
                    while (atual->prox != NULL)
                    {
                        atual = atual->prox;
                    }
                    atual->prox = (nodo_l_t *)malloc(sizeof(nodo_l_t));
                    atual->prox->elemento = posicao;
                    atual->prox->prox = NULL;
                }

                primeiraLetra = false;
            }
        }
        else if (c == ' ')
        {
            primeiraLetra = true;
            posicao++;
        }
    }
    rewind(arquivo);
}
```
- a funcao 'libera_vetor_de_lista' que eu usei para dar os free's no vetor.
```c
  void libera_vetor_de_lista(nodo_l_t **vetorASCII)
  {
    nodo_l_t *atual, *prox;

    for (int i = 0; i < 256; i++)
    {
        if (vetorASCII[i] != NULL)
        {
            atual = vetorASCII[i];
            while (atual != NULL)
            {
                prox = atual->prox;
                free(atual);
                atual = prox;
            }
        }
    }
    free(vetorASCII);
  }
```
## Arquivo listaBeale

Este arquivo eu coloquei todas as operaçes exclusivamente da lista, como o numero de palavras, a funcao que pega um numero aleatorio da lista para a codificação.
- a seguinte função eu utilizei para pegar um numero aleatorio da lista, para a codificação ficar boa.
```C
    nodo_l_t *elemento_aleatorio(nodo_l_t *topo, int tamanho)
{
    /* gera um número aleatório entre 0 e o tamanho da lista - 1*/
    int indice_aleatorio = rand() % tamanho - 1;

    // percorre a lista até chegar no elemento de índice correspondente
    nodo_l_t *atual = topo;
    for (int i = 0; i < indice_aleatorio; i++)
    {
        atual = atual->prox;
    }

    return atual;
}
```

