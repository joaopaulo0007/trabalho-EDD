#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define NUM_LETRAS 26

typedef struct no
{
    // char da trie
    char name;
    // indicador de se a trie representa ou nao o fim da palavra
    bool fim_da_palavra;
    // lista de apontadores para as 26 letras do alfabeto
    struct no *filhos[NUM_LETRAS];
    // apontador para o no pai
    struct no *pai;
} trie;
// funcao para inicializar a função
void inicializar(trie *t)
{
    // alocação de memoria da trie
    trie *tr = (trie *)malloc(sizeof(trie));
    tr = t;
    // definindo o primeiro caractere como \0, esse primeiro caractere nao e considerado na hora de procurar uma palavra
    tr->name = '\0';
    // definição do indicador de fim de palavra como falso
    tr->fim_da_palavra = false;
    // laço de repetição fazendo os nos filhos apontarem para NULL
    for (int i = 0; i < NUM_LETRAS; i++)
    {
        tr->filhos[i] = NULL;
    }
    // definindo o no pai como NULL
    tr->pai = NULL;
}
// função que dado um char e uma trie, retorna o apontador para o no filho representado por aquela letra
trie *procurar_no(char c, trie *T)
{
    // calculo da posição do no filho lista
    int index = c - 'a';
    return T->filhos[index];
}
// função que dado um char e uma trie diz se o filho em questão e NULL ou nao
bool existe_no(char v, trie *principal)
{
    trie *valor = procurar_no(v, principal);
    return valor != NULL;
}
// função que dado  um char e uma trie insere uma trie no nó filho correspondente
void insere(char v, trie *t)
{
    // calculo da posição em que a trie sera alocada
    int index = v - 'a';
    // alocação de memoria para a trie que sera inserida
    t->filhos[index] = (trie *)malloc(sizeof(trie));
    // definindo a letra dessa nova trie como a letra dada na função
    t->filhos[index]->name = v;
    // definindo o fim da palavra como falso
    t->filhos[index]->fim_da_palavra = false;
    // laço fazendo os nos filhos dessa nova trie como NULL
    for (int i = 0; i < NUM_LETRAS; i++)
    {
        t->filhos[index]->filhos[i] = NULL;
    }
    // definindo a trie dada na função como o no pai da trie criada
    t->filhos[index]->pai = t;
}
// função que dada uma palavra e uma trie, insere todos os caracteres dessa palavra na trie
void inserir_palavra(char *palavra, trie *t)
{
    trie *p = t;
    // laço que para cada letra da palavra analisa se ja existe um no para aquela letra e caso nao exista, insere aquela letra usando a função insere
    for (int i = 0; i < strlen(palavra); i++)
    {
        char letra = palavra[i];
        if (!existe_no(letra, p))
        {
            insere(letra, p);
        }
        // muda o no trie para o no filho criado
        p = procurar_no(letra, p);
    }
    // apos o fim da palavra, define o fim como verdadeiro
    p->fim_da_palavra = true;
    printf("palavra %s inserida com sucesso\n", palavra);
}
// função que dada uma palavra verifica se a palavra esta ou nao inserida na trie
bool procurar_palavra(char *palavra, trie *t)
{
    trie *p = t;
    // laço para chegar no fim da palavra
    for (int i = 0; i < strlen(palavra); i++)
    {
        // letra daquela posição da palavra
        char letra = palavra[i];
        // analise se existe o filho para aquela letra existe ou nao
        if (!existe_no(letra, p))
        {
            printf("palavra %s nao encontrada\n", palavra);
            // caso nao exista retorna falso
            return false;
        }
        else
        {
            // caso exista muda o no da trie para o no filho representado por aquela letra
            p = procurar_no(letra, p);
        }
    }
    // apos passar pelo laço, analisa se o no que representa a ultima letra da palavra esta com o indicador de fim da palavra esta como verdadeiro ou nao, retornando o valor indicado nele
    if (p->fim_da_palavra == true)
    {
        printf("palavra %s encontrada\n", palavra);
        return true;
    }
    else
    {
        printf("palavra %s nao encontrada\n", palavra);
        return false;
    }
}
// função que dada uma trie analisa se todos os seus nos filhos são NULL ou nao
bool existe_filhos(trie *t)
{
    for (int i = 0; i < NUM_LETRAS; i++)
    {
        if (t->filhos[i] != NULL)
        {
            return true;
        }
    }
    return false;
}
// função que dada uma palavra e uma trie remove a palavra se ela estiver nela, caso nao esteja, retorna como falso
bool remover_palavra(char *palavra, trie *t)
{
    trie *p = t;
    char letra = '\0';
    // laço para chegar no nó que representa o fim da palavra
    for (int i = 0; i < strlen(palavra); i++)
    {

        letra = palavra[i];
        // analisa se existe ou nao aquele no, caso nao existe retorna como falso
        if (!existe_no(letra, p))
        {
            printf("palavra %s nao esta presente na trie\n", palavra);
            return false;
        }
        p = procurar_no(letra, p);
    }
    // analisa se o fim da palavra esta como verdadeiro ou nao, e retorna o valor que representa ele
    if (p->fim_da_palavra)
    {
        // passa o fim palavra para falso
        p->fim_da_palavra = false;
        // laço que enquanto o nao pai for diferente de NULL o fim da palvra for diferente de verdadeiro e nao existe nenhum filho, vai removendo os caracteres
        while (p->pai != NULL && !p->fim_da_palavra && !existe_filhos(p))
        {
            int index = p->name - 'a';
            p->pai->filhos[index] = NULL;
            free(p);
            p = p->pai;
        }
        printf("palavra %s removida com sucesso\n", palavra);
        return true;
    }
    else
        printf("palavra %s nao esta presente na trie\n", palavra);
    return false;
}
// função para imprimir todas as palvras existentes na trie dada a trie uma palavra e um numero representando o nivel em q ta a trie
void imprimir_palavras(trie *t, char *aux, int nivel)
{
    // analise se aquele e o ultimo no
    if (t == NULL)
    {
        return;
    }
    // analise se esse e o fim da palavra e se for, escreve a palavra
    if (t->fim_da_palavra)
    {

        aux[nivel] = '\0';
        printf("%s\n", aux);
    }
    // laço para fazer recursao com cada no filho da trie
    for (int i = 0; i < NUM_LETRAS; i++)
    {
        if (t->filhos[i] != NULL)
        {
            // coloca a letra no char auxiliar naquela posição
            aux[nivel] = t->filhos[i]->name;
            // chamada recursiva da funçao aumentando em 1 o nivel para nao haver sobreposiçao dos caracteres
            imprimir_palavras(t->filhos[i], aux, nivel + 1);
        }
    }
}

// função para liberar o espaço alocado da trie
void dispose_trie(trie *t)
{
    if (t == NULL)
        return;
    else
    {
        for (int i = 0; i < NUM_LETRAS; i++)
        {
            dispose_trie(t->filhos[i]);
        }
        free(t);
    }
}

int main()
{
    char palavra[100];
    trie trie;
    inicializar(&trie);
    inserir_palavra("joao", &trie);
    inserir_palavra("casal", &trie);
    inserir_palavra("casa", &trie);
    inserir_palavra("abacate", &trie);
    inserir_palavra("abacaxi", &trie);
    inserir_palavra("abaco", &trie);
    inserir_palavra("corrida", &trie);
    inserir_palavra("correr", &trie);
    inserir_palavra("caridade", &trie);
    inserir_palavra("explorar", &trie);
    inserir_palavra("examinar", &trie);
    inserir_palavra("conhecer", &trie);
    inserir_palavra("esperanca", &trie);
    procurar_palavra("casa", &trie);
    procurar_palavra("cas", &trie);
    procurar_palavra("abacate", &trie);
    procurar_palavra("correr", &trie);
    procurar_palavra("explorar", &trie);
    procurar_palavra("esperanca", &trie);
    procurar_palavra("corredor", &trie);
    remover_palavra("explorar", &trie);
    remover_palavra("corredor", &trie);
    remover_palavra("joao", &trie);
    remover_palavra("abacaxi", &trie);
    imprimir_palavras(&trie, palavra, 0);
    dispose_trie(&trie);

    return 0;
}
