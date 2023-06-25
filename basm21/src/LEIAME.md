# Arquivador Vina++
Arquivador que faz operações com arquivos e seus metadados.

NOME: Bruno Aziz Spring Machado   GRR: GRR20211279

## Estrutura

O meu projeto está separado em dois arquivos principais, onde o ’main.c’ se trata do principal, o secundário
é: ’archive.c’. E, por fim, o ’Makefile’ que serve para a execução do programa.

O meu programa esta estruturado da seguinte forma:
| OFFSET | conteudo dos arquivos | Diretório com os Metadados |

Os metadados são uma lista de FiloInfo, ou seja:

```
    typedef struct FileInfo
    {
        char nome[256];
        char path[1024];
        int posicao;
        int indice;
        size_t tam_inic;
        size_t tam;

        dev_t st_dev;
        mode_t permissoes;
        time_t ult_modif;
        uid_t UserID;
        gid_t GroupID;
    } FileInfo_t;

typedef struct nodo_l nodo_t;
```
que guarda os metadados indicados de cada arquivo.

E o diretório é uma lista duplamente encadeada de FileInfo, que contém todos os arquivos presentes no arquivador.

A lista está representada na seguinte estrutura:

```
    struct nodo_l
{
    FileInfo_t arquivo;
    nodo_t *prox;
    nodo_t *ant;
};

typedef struct dir
{
    unsigned int qntd;
    nodo_t *head;
    nodo_t *ult;
} dir_t;

```
A escolha da estrutura foi feita bem no inicio do projeto, por não saber de minhas necessidades futuras, escolhi faze-la duplamente encadeada.

## Arquivo Main

Neste arquivo, faço a inclusão dos arquivos de cabeçalho dos meus outros dois programas para uso de suas funções. Trato os modelos de execução conforme a flag que for passada, flag essa que representará a operação a ser feita

Eu verifico a flag que foi passada com o "getopt" em conjunto com um switch, o switch verifica se pelo menos um argumento foi passado. 

Primeira mente na minha função principal eu abro o arquivador com fopen, no modo rb+ se caso ele existir, se ele não existir, é criado com wb+, o seguinte trecho de código trata isso:

```
    FILE *arquivador = fopen("backup.vpp", "rb+");
    if (arquivador == NULL)
    {
        arquivador = fopen("backup.vpp", "wb+");
        if (arquivador == NULL)
        {
            perror("Erro ao criar o arquivo de backup");
            return 1;
        }
    }
```
    - 'r' (REMOVE): A opção remove, como o nome já diz, remove um arquivo e seus metadados do arquivador.

    - 'm' (MOVE): A flag -m indica que você quer mover um arquivo dentro do seu arquivador. 

    - 'i' (INSERE): A flag -i indica que será feita uma inserção de um arquivo no diretório.

    - 'c' (LISTA): A opção -c lista os metadados do arquivo, como mencionado anteriormente, esses metadados estão em uma lista, essa opção imprime a lista no terminal.

    - 'x': Quando a opção 'x' é passada, o programa extrai o ou os arquivos do arquivador, você pode extrair todos ou apenas um específico.

No geral, esse programa permite realizar operações básicas de manipulação de um arquivo de backup, como inserir, remover, listar e extrair arquivos
### IMPORTANTE(não se esqueça de remover o arquivo os os arquivos).

## Arquivo codifica-decodifica