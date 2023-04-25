#ifndef _codifica_decodifica_t_H
#define _Codifica_t_H

void cria_arquivo_de_chaves(nodo_l_t **vetorASCII);

void cria_vetor_de_lista(FILE *arquivo, nodo_l_t **vetorASCII);

void Cria_msg_codificada(FILE *arquivo, nodo_l_t **vetorASCII);

void descodifica_msg_arq_de_chaves(FILE* arquivo, nodo_l_t **vetorASCII);

#endif