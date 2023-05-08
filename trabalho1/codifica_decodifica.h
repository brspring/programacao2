#ifndef _codifica_decodifica_t_H
#define _Codifica_t_H

void cria_arquivo_de_chaves(nodo_l_t **vetorASCII);

void cria_vetor_de_lista_com_livro(FILE *arquivo, nodo_l_t **vetorASCII);

void Cria_arq_msg_codificada(FILE *arquivo, nodo_l_t **vetorASCII);

void cria_arq_msg_decodificada(nodo_l_t **vetorASCII);

void cria_vetor_de_lista_com_arq_de_chaves(nodo_l_t **vetorASCII);

#endif