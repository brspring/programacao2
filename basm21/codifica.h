#ifndef _codifica_t_H
#define _Codifica_t_H

void CriaArquivoDeChaves(nodo_l_t **vetorASCII);

void cria_vetor_de_lista(FILE *arquivo, nodo_l_t **vetorASCII);

FILE* Cria_arq_msg_codificada(FILE *arquivo, nodo_l_t **vetorASCII);

int buscar_numero(int num, nodo_l_t **vetorASCII);

#endif