# Arquivador Vina++
Arquivador que faz operações com arquivos e seus metadados.

NOME: Bruno Aziz Spring Machado   GRR: GRR20211279
## Ideia principal

Eu pensei no arquivador da seguinte maneira:

| OFFSET | conteudo dos arquivos | Diretório com os Metadados |

onde o OFFSET é o tamanho do conteudo dos arquivos.

Algo importante de pontuar é que meus arquivos possuem "posicao" e "indice". Posição é o primeiro byte deles no arquivador e Indice é o indice na lista, o primeiro sendo 0.

## Estrutura

O meu projeto está separado em dois arquivos principais, onde o ’main.c’ se trata do principal, o secundário
é: ’archive.c’. E, por fim, o ’Makefile’ que serve para a execução do programa.

Os metadados são uma lista de FiloInfo, ou seja:

```c
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

```c
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

```c
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

## Arquivo 'archive.c'
Basicamente nesse aquivo eu tenho tudo o que eu preciso para mexer no meu diretorio, eu até poderia separar em remove.c, move.c, etc. Porém quando fiz isso não lidei muito bem com a quantidade alta de arquivos, preferi tudo junto mesmo.

Esse arquivo possui uma grande quantidade de funções, onde 90% delas mexem na lista de structs. Também por esse fato evitei não fragmentar esse arquivo em mais partes, pois dava bastante dor de cabeça com a manipulação da lista.

#Principais Funções

## Insere 
Eu não comecei pela função de remover, porém pela sequência lógica, ela é a que mais faz sentido de se mostrar primeiro.

```c
    void inserir_arq(const char *nome_arquivo, dir_t *diretorio, FILE *arquivador, long long *offset)
    {
        struct stat file_stat;
        FileInfo_t arquivo;
        strncpy(arquivo.nome, nome_arquivo, sizeof(arquivo.nome));
        arquivo.tam = file_stat.st_size;
        arquivo.st_dev = file_stat.st_dev;
        arquivo.permissoes = file_stat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
        arquivo.ult_modif = file_stat.st_mtime;
        arquivo.UserID = file_stat.st_uid;
        arquivo.GroupID = file_stat.st_gid;
        
        if (diretorio->qntd == 0)
        {
            arquivo.posicao = sizeof(long long) + *offset + 1;
        }
        else
        {
            nodo_t *ultimoArquivo = diretorio->ult;
            arquivo.posicao = ultimoArquivo->arquivo.posicao + ultimoArquivo->arquivo.tam;
        }
    
        int posicao = arquivo.posicao;
        int block = arquivo.posicao + arquivo.tam - 1;
        adiciona_arq_lista(diretorio, &arquivo);
    
        ler_conteudo(nome_arquivo, arquivador, posicao, block);
    
        atualizar_posicoes_arq(diretorio);
        printa_metadados_lista(diretorio, arquivador);
        *offset += arquivo.tam;
    }
```
Primeiro adiciono os metadados do arquivo nele, para inserir os metadados na lista na posição correspondente, após isso eu adiciono o arquivo na lista e leio seu conteudo para imprimilo em forma de bytes no arquivador, e a função 'printa_metadados_lista' coloca os metadados em forma de bytes no diretório do arquivador.

## Move

Provavelmente a função/operação mais confusa e complicada de ser feita, mesmo com o move_bytes.

```c
    int move_membro(FILE *arquivador, dir_t *diretorio, const char *name, const char *name2)
{
    nodo_t *arquivo_movido = buscarArquivoPorNome(diretorio, name);
    nodo_t *arquivo_destino = buscarArquivoPorNome(diretorio, name2);
    rewind(arquivador);
    
    unsigned int b_arq_init, b_arq_final, rt;
    int destino;

    /*move do metadados na lista*/
    if (arquivo_destino->arquivo.indice < arquivo_movido->arquivo.indice)
    {
        moverNoParaIndice(diretorio, arquivo_movido, arquivo_destino->arquivo.indice + 1);
    }
    else
        moverNoParaIndice(diretorio, arquivo_movido, arquivo_destino->arquivo.indice);
    
    destino = arquivo_destino->arquivo.posicao + arquivo_destino->arquivo.tam;
    b_arq_init = arquivo_movido->arquivo.posicao;
    b_arq_final = arquivo_movido->arquivo.posicao + arquivo_movido->arquivo.tam - 1;

    if (destino > b_arq_final)
    {
        destino = arquivo_destino->arquivo.posicao + arquivo_destino->arquivo.tam - arquivo_movido->arquivo.tam;
    }
    
    rt = move_bytes(arquivador, b_arq_init, b_arq_final, destino);

    printa_metadados_lista(diretorio, arquivador);
    atualizar_posicoes_arq(diretorio);
}
```
Nessa função, como citado anteriormente, eu movo o arquivo, porém para isso eu preciso mover seu conteúdo e seus metadados na lista também. E após move-lo, é preciso atualizar seu índice e sua posição (que indica o primeiro byte do conteúdo).
O principal desafio dessa função foi a operação com bytes e saber onde o arquivo deve ser inserido após move-lo, é um desafio e sem um papel e um lápis essa função não roda. Para sua implementação foi necessário vários e vários testes.

## Remove

A função remove member me deu alguns trabalhos por conta das operações com bytes e por me confudir quando eu tinha que subtrair 1 ou não.

```c
    int remove_member(const char *name, dir_t *diretorio, FILE *arquivador)
{
    nodo_t *removal = buscarArquivoPorNome(diretorio, name);
    long long offset = calcula_offset(arquivador, *diretorio);
    printf("offset: %lld\n", offset);

    if (removal == NULL)
    {
        return 1;
    }
    unsigned int b_metadado_fim, b_metadado_init;

    if (removal->prox == NULL)
    {
        b_metadado_init = 9 + offset + (removal->arquivo.indice * sizeof(FileInfo_t));
        b_metadado_fim = b_metadado_init + sizeof(FileInfo_t);
    }
    else
    {
        b_metadado_init = 9 + offset + (removal->arquivo.indice * sizeof(FileInfo_t)) + 1;
        b_metadado_fim = b_metadado_init + sizeof(FileInfo_t);
    }

    int retorno = remove_bytes(arquivador, b_metadado_init, b_metadado_fim);

    if (retorno != 0)
        printf("retorno: %d\n", retorno);

    unsigned int b_arq_init = removal->arquivo.posicao;
    unsigned int b_arq_final = removal->arquivo.posicao + removal->arquivo.tam - 1;

    /* Remove o conteúdo do membro */
    int rt = remove_bytes(arquivador, b_arq_init + 1, b_arq_final + 1);
    if (rt)
        return 1;

    return 0;
}

```
Eu primeiro remove os metadados, por ficarem no fim do arquivo e atrapalharem menos na hora da remoção e após isso removo o conteúdo, e assim como na função de mover, é necessário atualizar seu indice e posição após a remoção.

## Extrai
Para extrair um arquivo do meu arquivador, eu usei a seguinte função:
```c
    int copiar_arquivo_do_arquivador(const char *nome_arquivo, FILE *arquivador)
{
    FILE *arquivo_original = fopen(nome_arquivo, "rb");
    char nome_copia[256];
    const char *nome_arquivo_sem_dir = strrchr(nome_arquivo, '/');
    if (nome_arquivo_sem_dir != NULL)
    {
        nome_arquivo_sem_dir++; 
        snprintf(nome_copia, sizeof(nome_copia), "copia_%s", nome_arquivo_sem_dir);
    }
    else
    {
        snprintf(nome_copia, sizeof(nome_copia), "copia_%s", nome_arquivo);
    }

    printf("Criando cópia do arquivo: %s\n", nome_copia);

    FILE *arquivo_copia = fopen(nome_copia, "wb");
    if (arquivo_copia == NULL)
    {
        perror("Erro ao criar o arquivo de cópia");
        fclose(arquivo_original);
        return 2;
    }
    char buffer[1024];
    size_t bytes_lidos;
    long long block = get_size(arquivador);

    if (block > sizeof(buffer))
    {
        while (block > 0 && (bytes_lidos = fread(buffer, 1, sizeof(buffer), arquivo_original)) > 0)
        {
            fwrite(buffer, 1, bytes_lidos, arquivo_copia);
            block -= bytes_lidos;
        }
    }
    else
    {
        bytes_lidos = fread(buffer, 1, block, arquivo_original);
        fwrite(buffer, 1, bytes_lidos, arquivo_copia);
    }

    printf("Arquivo copiado com sucesso: %s\n", nome_copia);

    fclose(arquivo_original);
    fclose(arquivo_copia);

    return 0;
}
```
Onde eu pego o nome do arquivo a ser extraido e crio uma copia, essa copia eu mando para o diretorio 'basm21/' onde o meu programa está sendo executado. E o restante da lógica que era o mais fácil eu deixei na main mesmo.

#Lista

Para manipulação da lista, as principais funções foram:

```c
 adiciona_arq_lista(dir_t *diretorio, FileInfo_t *arquivo);

 carregar_metadados_lista(dir_t *diretorio, FILE *arquivador);

 print_lista(dir_t *diretorio);

 printa_metadados_lista(dir_t *diretorio, FILE *arquivador);
```

A principal delas sendo a 'carregar_metadados_lista'. Essa função lê um arquivador já existente, lê seu diretório e com base nesse diretório lido, acrescenta na lista.

##BUGS conhecidos

o meu extrair nao funciona quando passado com outro arquivador que não seja o backup.vpp






