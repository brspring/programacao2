#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "listaBeale.h"
#include "codifica_decodifica.h"

int main(int argc, char *argv[])
{
    /*Codifica e cria o arquivo de chaves.*/
    if (argc == 10)
    {
        FILE *livro;
        livro = fopen("livro.txt", "r");
        if (livro == NULL)
        {
            perror("Erro em abrir o arquivo!\n");
            return 1;
        }

        /* vetor que cada indice eh um caracter da tabela ASCII e ele eh do tipo lista para ter uma lista com a frequencia de cada letra*/
        nodo_l_t *vetorASCII[128] = {NULL};

        cria_vetor_de_lista_com_livro(livro, vetorASCII);
        cria_arquivo_de_chaves(vetorASCII);

        /*codificar uma mensagem*/
        FILE *MensagemOriginal;
        MensagemOriginal = fopen("MensagemOriginal.txt", "r");

        if (!MensagemOriginal)
        {
            perror("Arquivo com a mensagem não existe\n");
            return 1;
        }

        Cria_arq_msg_codificada(MensagemOriginal, vetorASCII);


        libera_vetor_ASCII(vetorASCII);
        fclose(MensagemOriginal);
        fclose(livro);
    }

    /*decodifica com um arquivo de chaves*/
    if(argc == 8 && (strcmp(argv[4], "-c") == 0) ){

        FILE *mensagem_codificada = fopen("MensagemCodificada.txt", "r");
        if (!mensagem_codificada)
        {
            printf("Arquivo com a mensagem codificada não encontrado.\n");
            return 1;
        }

        nodo_l_t *vetorASCII[128] = {NULL};

        cria_vetor_de_lista_com_arq_de_chaves(vetorASCII);

        cria_arq_msg_decodificada(vetorASCII);

        /*limpeza*/
        libera_vetor_ASCII(vetorASCII);
        fclose(mensagem_codificada);

    }

    /*Decodifica somente com o livro.*/
    if(argc == 8 && (strcmp(argv[4], "-b") == 0) ){

        FILE *livro;
        livro = fopen("livro.txt", "r");
        if (livro == NULL)
        {
            perror("Erro em abrir o arquivo!\n");
            return 1;
        }
    
        nodo_l_t *vetorASCII[128] = {NULL};

        cria_vetor_de_lista_com_livro(livro, vetorASCII);

        FILE *mensagem_codificada = fopen("MensagemCodificada.txt", "r");
        if (!mensagem_codificada)
        {
            printf("Arquivo com a mensagem codificada não encontrado.\n");
            return 1;
        }

       cria_arq_msg_decodificada(vetorASCII);

        /*nao sei se vou usar, mas diz o numero de palavras do livro*/
        /*int num_palavras = NumPalavrasLivro(livro);
        printf("O arquivo tem %d palavras.\n", num_palavras);*/

        /*limpeza*/
        libera_vetor_ASCII(vetorASCII);
        fclose(mensagem_codificada);
        fclose(livro);
    }
        return 0;
}