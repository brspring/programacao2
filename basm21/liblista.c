#include <stdio.h>
#include <stdlib.h>
#include "liblista.h"

lista_t* lista_cria () {
    lista_t *lista = malloc(sizeof(lista_t));

    if(lista == NULL)
        return NULL;

    lista->tamanho = 0;
    lista->ini = NULL;

    return lista;
}


int lista_vazia (lista_t* l) {

    if(l->tamanho == 0)
        return 1;

    return 0;

}

int lista_tamanho (lista_t* l) {
    return l->tamanho;

}

lista_t* lista_destroi (lista_t* l) {

    if(l->tamanho > 0) {
        nodo_l_t *auxiliar = l->ini;
        nodo_l_t *nodoFree = l->ini;
        /*verifica do inicio da lista*/
        while(nodoFree != NULL) {
            auxiliar = nodoFree->prox;
            free(nodoFree);
            nodoFree = auxiliar;
        }
    }

    free(l);

    return NULL;

}

int lista_insere_inicio (lista_t* l, int elemento) {
    nodo_l_t* no;
    
    if( ! (no = malloc(sizeof(nodo_l_t))))
        return 0;
    no->elemento = elemento;
    no->prox = NULL;
    /*se a fila ja tier um elemento, entra na condicao*/
    if (! (lista_vazia(l)))
        no->prox = l->ini;
    /*coloca o elemento na lista*/ 
    l->ini = no;
    l->tamanho++;

    return 1;
} 

int lista_insere_fim (lista_t* l, int elemento) {
    nodo_l_t* no;
    nodo_l_t* aux;

    if (lista_vazia(l)) {
        /*caso a lista estiver vazia insere no inicio*/
        lista_insere_inicio(l, elemento);
        return 1;
    }
    
    if (! (no = malloc(sizeof(nodo_l_t))))
        return 0;

    no->elemento = elemento;
    no->prox = NULL;

    l->tamanho++;
    aux = l->ini;

    /*fazer o aux percorrer ate o fim e inserir la*/
    while (aux->prox != NULL){
        aux = aux->prox;
    }
    aux->prox = no;   

    return 1;
    

}

void lista_imprime (lista_t* l) {
    nodo_l_t *aux = l->ini;

    if(l->tamanho == 0)
        return;

    while(aux != NULL) {
        printf("%d ", aux->elemento);
        aux = aux->prox;
    }
    
    printf("\n");

}


int lista_insere_ordenado (lista_t* l, int elemento) {
    nodo_l_t* no;
    nodo_l_t* aux;

    aux = l->ini;
    /*condicao para ver se a lista esta vazia ou o elemento é menor que o que ja pertence a lista*/
    if (lista_vazia(l) || (elemento <= l->ini->elemento)) {
        lista_insere_inicio(l, elemento);
        return 1;
    }
    /*verifica a alocacao*/
    if (! (no = malloc(sizeof(nodo_l_t))))
        return 0;
    no->elemento = elemento;

    aux = l->ini;
    
    /*verifica ae achar um elemento maior e insere ele antes do maior q ele*/
    while ((aux->prox != NULL) && (elemento > aux->prox->elemento))
        aux = aux->prox;

    no->prox = aux->prox;
    /*coloco o elemento inserido como proximo do elemento anterior menor que ele*/
    aux->prox = no;   
    l->tamanho = l->tamanho+1;
    
    return 1;

}

int lista_retira_inicio (lista_t* l, int* elemento) {
    nodo_l_t *temp = l->ini;

    if(l->tamanho == 0)
        return 0;

    /*para remover o elemento o inicio recebe seu prox*/
    l->ini = l->ini->prox;
    *elemento = temp->elemento;
    /*limpa o elemento que sera removido*/
    free(temp);
    temp = NULL;

    l->tamanho = l->tamanho-1;

    return 1;

}

int lista_pertence (lista_t* l, int elemento) {
    nodo_l_t* aux = l->ini; 

    if(l->tamanho == 0)
        return 0;

    /*percorre a lista atras do elemento buscado*/
    while (aux->prox){
        if (aux->elemento == elemento)
            return 1;
        aux = aux->prox;
    }


    return 0;
}

int lista_retira_fim (lista_t* l, int* elemento) {
    nodo_l_t* aux = l->ini;

    if (l->tamanho == 0)
        return 0;
    
    if (aux->prox){
        nodo_l_t* ultimo;
        
        /*chega ate o penultimo elemento*/
        while (aux->prox->prox){
            aux = aux->prox;
        }
        /*retira o ultimo elemento e coloca o penultimo como ultimo*/
        ultimo = aux->prox;
        *elemento = ultimo->elemento;
        aux->prox = NULL;
        aux = ultimo;

    } else {
        /*condicao caso so exista um elemento na lista*/
        *elemento = aux->elemento;
        l->ini = NULL;
    }

    free(aux);
    l->tamanho = l->tamanho-1;

    return 1;
    

}

int lista_retira_elemento (lista_t *l, int *elemento) {
    nodo_l_t* aux = l->ini;
    nodo_l_t* nodo_elem;

    if (l->tamanho == 0)
        return 0;

    /*caso o elemento digitado seja igual ao inicio*/
    if(l->ini->elemento == *elemento)
        return (lista_retira_inicio(l, elemento));
    
    while (aux->prox) {
        if (aux->prox->elemento == *elemento) {
        /*caso o elemento digitado esteja no fim da lista: */
            if (! (aux->prox->prox))
                return (lista_retira_fim(l, elemento));
            nodo_elem = aux->prox;
            aux->prox = aux->prox->prox;
            aux = aux->prox;
                
            free(nodo_elem);
            l->tamanho--;
                
            return 1;
        }
        aux = aux->prox;
    }
    return 0;
    

}
