#ifndef _listaBeale_t_H
#define _Beale_t_H

struct nodo_l {
    int elemento;       /* lista de numeros inteiros */
    int posicao;
    struct nodo_l* prox;  /* ponteiro para o proximo   */
};
typedef struct nodo_l nodo_l_t;

int NumPalavrasLivro(FILE *livro);

nodo_l_t *elemento_aleatorio(nodo_l_t *topo, int tamanho);

int tamanho_lista(nodo_l_t *head);

char buscar_numero_lista(int num, nodo_l_t **vetorASCII);

void libera_vetor_ASCII(nodo_l_t **vetorASCII);

#endif