#include "fila.h"

#include<stdio.h>
#include<stdlib.h>

typedef struct _no NO;

struct _no
{
    NO* proximo;
    int chave;
};

struct fila_
{
    // Ponteiro para frente e fim da fila.
    NO* frente; // Aponta para o nó mais antigo da estrutura.
    NO* fim; // Aponta para o ultimo nó inserido.
    int tamanho;
};

// Função cria fila, retorna nulo como erro ao alocar.
FILA* fila_criar(void)
{
    FILA *fila = (FILA*) malloc(sizeof(FILA));
    // Caso fila tenha sido alcoada, inicializa-se os campos.
    if(fila != NULL)
    {
        // Inicialmente, não há fim nem frente.
        fila->frente = NULL;
        fila->fim = fila->frente;
        fila->tamanho = 0;
    }
    return fila;
}
    
// Retorna verdadeiro caso memória cheia ou fila nula.
bool fila_cheia(FILA *fila)
{
    if(fila != NULL)
    {
        // Fila está cheia somente se a memória encheu.
        NO* verificar_memoria = (NO*) malloc(sizeof(NO));
        // Caso ocorra alocação dinâmica.
        if(verificar_memoria != NULL)
        {
            // A memória não está cheia.
            // Desalocar memória usada para verificar.
            free(verificar_memoria); 
            verificar_memoria = NULL;
            return false;
        }
        // Se memoria cheia, não houve alocação, retorna true no final.
    }
    // Se não existir fila, retorna-se verdadeiro.
    // Tal opção evita erros, pois fila cheia evita ações com a fila.  
    return true;
}

// Retorna verdadeiro para fila vazia ou nula.
bool fila_vazia(FILA *fila)
{
    // Fila está vazia se não há itens na frente.
    if(fila != NULL)
        return (fila->frente == NULL);

    // Dado que a fila não existi, ela está vazia.
    return true;
}

// Retorna falso caso item nulo, fila nula ou fila cheia.
bool fila_inserir(FILA *fila, int chave)
{
    if(fila != NULL)
    {
        NO *novo_fim = (NO*) malloc(sizeof(NO));

        if(novo_fim != NULL)
        {    
            // Se fila vazia(define-se o inicio).
            if(fila->frente == NULL)
                fila->frente = novo_fim;  
            else 
                fila->fim->proximo = novo_fim; // Garante ligação da fila.
            
            // Inicializa nó
            novo_fim->chave = chave; 
            novo_fim->proximo = NULL;

            // Define novo fim da fila.
            fila->fim = novo_fim;

            fila->tamanho++;
            // Retorna nulo caso ocorra inserção
            return true;
        }
        // Se novo_fim for nulo, a memória está cheia.
    }
    // Caso de item ou fila nula.
    return false;
} 

// Remove frente da fila, retorna nulo caso contrário.
int fila_remover(FILA *fila)
{
    if(!fila_vazia(fila))
    {
        // Guardar frente a ser retirada.
        NO *frente = fila->frente;
        int chave = frente->chave;

        // Atualizar frente e tamanho da fila.
        fila->frente = fila->frente->proximo;
        fila->tamanho--;
        
        // Desalocação da antiga frente da fila.
        frente->proximo = NULL;
        free(frente); 
        frente = NULL;
        
        // Se fila ficou vazia, ajustar fim para nulo.
        if(fila->frente == NULL)
            fila->fim = NULL;
        
        // Retorna item da frente.
        return chave;
    }
    // Retorna nulo em caso de fila vazia ou nula.
    return -1;
}

// Retorna -1 caso fila não exista.
int fila_tamanho(FILA *fila)
{
    if(fila != NULL)
        return (fila->tamanho);

    // Simboliza fila nula.
    return (-1);
}
    
// Retorna item da frente da fila.(nulo caso não exista)
int fila_frente(FILA *fila)
{
    if(fila != NULL) 
        return (fila->frente->chave);
    
    return -1;
}

void fila_apagar(FILA **fila)
{
    // Se fila existir.
    if(fila != NULL && *fila != NULL)
    {
        // Esvazia-se a fila apagando os seus dados e itens.
        while((*fila)->frente != NULL) fila_remover(*fila);

        // Apagar estrutura de controle da fila.
        free(*fila); 
        *fila = NULL;
    }
}

// Inverte fila recursivamente
void fila_inverter_no(NO *no)
{
    // Encontra penúltimo da fila.
    if(no->proximo->proximo != NULL) 
        fila_inverter_no(no->proximo);
    // Inverte os ponteiros
    no->proximo->proximo = no;
    no->proximo = NULL;
}

// Retorna falso caso a fila não existir.
bool fila_inverter(FILA *fila)
{
    if(fila != NULL && fila->tamanho > 1)
    {
        // Função recursiva que inverterá os ponteiros.
        fila_inverter_no(fila->frente);
        // Antiga frente será novo fim e vice-versa.
        NO *aux = fila->frente;
        fila->frente = fila->fim;
        fila->fim = aux; 
        return true;
    }
    return false;
}

void fila_imprimir(FILA *fila)
{
    if(fila != NULL && fila->frente != NULL)
    {
        // Variável auxiliar imprimir fila.
        NO *elemento = fila->frente;

        // Percorre fila imprimindo valores.
        while(elemento != NULL)
        {
            printf("%d ", elemento->chave);
            elemento = elemento->proximo;
        }
        printf("\n");
    }
}
