#include "arvoreb.h"

#include "indice.h"
#include "erros.h"
#include "utils.h"
#include "cabecalho.h"
#include "SQL.h"
#include "criterio.h"
#include "fila.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct _arvb
{
    FILE *arq_arvb;
    CABECALHO_ARVOREB *c_arvb;
};

typedef struct _no NO;
struct _no {
    int tipoNo; // 4 bytes
    int nroChaves; // 4 bytes
    // Espaço extra para alocar nó em caso de split.
    int chaves[MAX_CHAVES + 1];
    long int offsets[MAX_CHAVES + 1];
    int filhos[MAX_FILHOS + 1];
};

// Protótipos
// Funções do nó
NO *_CriarNo(int tipo);
void _ApagarNo(NO **no);
NO *_LerNo(ARVB *arvb, int rrn);
void _EscreverNo(ARVB *arvb, NO *no, int rrn);
void _ImprimirNo(NO *no);

// Funções internas da árvore
NO *_InserirArvoreNo(ARVB *arvb, int rrn, int chave, long int offset);
NO *_Split(ARVB *arvb, NO* no, int rrn, int tipo);
long int _BuscarArvoreB(ARVB *arvb, int rrn, int chave);
void _BuscaEmProfundidade(ARVB *arvb, int rrn, 
                        FILE *arquivoDados, CRITERIO *crit,
                        int *vis, FILA *indices);
    

ARVB *CriarArvoreB(FILE *arquivo) {
    ARVB *arvb = (ARVB*) malloc(sizeof(ARVB));
    if(arvb == NULL) DispararErro(ErroAlocacaoMemoria());

    // Verificar se existe cabeçalho
    // Retorna NULL caso contrário.
    arvb->c_arvb = LerCabecalhoIndice(&arquivo);
    arvb->arq_arvb = arquivo;

    // Se não houver cabeçalho, cria-se um.
    if(arvb->c_arvb == NULL) {
        arvb->c_arvb = CriarCabecalhoIndicePadrao();
    }

    // Caso árvore vazia, criar primeiro nó.
    if(arvb->c_arvb->nroNos == 0) {
        NO *raiz = _CriarNo(FOLHA);
        if(raiz == NULL) DispararErro(ErroAlocacaoMemoria());

        arvb->c_arvb->noRaiz = 0;
        arvb->c_arvb->proxRRN++;
        arvb->c_arvb->nroNos++;
        EscreverCabecalhoIndice(&arquivo, arvb->c_arvb);
        _EscreverNo(arvb, raiz, 0);
        _ApagarNo(&raiz);
    }

    return arvb;
}

int NroNosArvoreB(ARVB *arvb) {
    if (arvb == NULL) DispararErro(ErroPonteiroInvalido());

    return (arvb->c_arvb->nroNos);
}

int NoRaizArvoreB(ARVB *arvb) {
    if (arvb == NULL) DispararErro(ErroPonteiroInvalido());

    return (arvb->c_arvb->noRaiz);
}

void InserirArvoreB(ARVB *arvb, int chave, long int offset) {
    if(arvb == NULL) DispararErro(ErroPonteiroInvalido()); 

    NO *promovido = _InserirArvoreNo(arvb, arvb->c_arvb->noRaiz, chave, offset);
    if(promovido != NULL) {
        promovido->nroChaves = 1;
        promovido->tipoNo = RAIZ;
        arvb->c_arvb->noRaiz = arvb->c_arvb->proxRRN;
        arvb->c_arvb->proxRRN++;
        arvb->c_arvb->nroNos++;

        // Escreve nova raiz
        _EscreverNo(arvb, promovido, arvb->c_arvb->noRaiz);
        _ApagarNo(&promovido);
    }

    return;
}

long int BuscarArvoreB(ARVB *arvb, int chave) {
    if(arvb == NULL) DispararErro(ErroPonteiroInvalido()); 

    return _BuscarArvoreB(arvb, arvb->c_arvb->noRaiz, chave);
}

long int _BuscarArvoreB(ARVB *arvb, int rrn, int chave){
    long int offset = -1;

    if(rrn != -1) {
        NO *no = _LerNo(arvb, rrn);
        int i = 0;
        while(i < no->nroChaves && chave >= no->chaves[i]) i++;

        if(i > 0 && chave == no->chaves[i - 1]) {
            offset = no->offsets[i - 1];
        } else {
            offset = _BuscarArvoreB(arvb, no->filhos[i], chave);
        }

        _ApagarNo(&no);
    }

    return offset;
}


NO *_InserirArvoreNo(ARVB *arvb, 
                    int rrn, 
                    int chave, 
                    long int offset) 
{
    NO *no = _LerNo(arvb, rrn);
    NO *promovido = NULL;

    // Caso base: nó folha
    if(no->tipoNo == FOLHA) {
        // inserindo em uma folha
        int i = no->nroChaves - 1;
        for(; i >= 0; i--) {
            if(chave >= no->chaves[i]) break;
        
            // desloca chaves em x para dar espaço para k
            no->chaves[i + 1] = no->chaves[i];
            no->offsets[i + 1] = no->offsets[i];
        }
        
        no->chaves[i + 1] = chave;  // insere chave k em x
        no->offsets[i + 1] = offset;  
        no->nroChaves = no->nroChaves + 1;     // agora x tem mais 1 chave

        // Split em nó folha
        if(no->nroChaves > MAX_CHAVES) {
            promovido = _Split(arvb, no, rrn, FOLHA);
        }
    } else {
        // Continuar busca pela folha a ser inserida
        int i = 0;
        while(i < no->nroChaves && chave >= no->chaves[i]) i++;
        promovido = _InserirArvoreNo(arvb, no->filhos[i], chave, offset);
        // Caso houver promoção, inseri-lo no nó.
        if(promovido != NULL) {
            // Shiftar em uma casa para direito
            // o vetor dos filhos e o vetor das chaves.
            for (int j = no->nroChaves; j > i; j--) {
                no->filhos[j + 1] = no->filhos[j];
                no->chaves[j] = no->chaves[j - 1];
                no->offsets[j] = no->offsets[j - 1];
            }

            // Inserir novo promovido.
            no->chaves[i] = promovido->chaves[0];
            no->offsets[i] = promovido->offsets[0];
            no->filhos[i] = promovido->filhos[0];
            no->filhos[i + 1] = promovido->filhos[1];
            no->nroChaves = no->nroChaves + 1;
            // Liberar espaço.
            _ApagarNo(&promovido);
            promovido = NULL;
        } 

        // Split em nó intermediário
        if(no->nroChaves > MAX_CHAVES) {
            promovido = _Split(arvb, no, rrn, INTERMEDIARIO);
            no->tipoNo = INTERMEDIARIO;
        }
    }

    _EscreverNo(arvb, no, rrn);
    _ApagarNo(&no);
    no = NULL;

    return promovido;
}

NO *_Split(ARVB *arvb, NO* no, int rrn, int tipo) {
    int meio = MAX_CHAVES/2;
    NO *novoNO = _CriarNo(tipo);
    arvb->c_arvb->proxRRN++;
    arvb->c_arvb->nroNos++;
    // Split passando parte superior para
    // novo nó à direita
    
    novoNO->filhos[0] = no->filhos[meio + 1];
    no->filhos[meio + 1] = -1;
    int k = 1;
    for (int j = meio + 1; j <= MAX_CHAVES; j++, k++) {
        novoNO->chaves[j - (meio + 1)] = no->chaves[j];
        novoNO->offsets[j - (meio + 1)] = no->offsets[j];
        novoNO->filhos[k] = no->filhos[j + 1];
        novoNO->nroChaves++;

        // Limpando no original
        no->chaves[j] = -1;
        no->offsets[j] = -1;
        no->filhos[j + 1] = -1;
        no->nroChaves--;
    }

    NO *promovido = _CriarNo(INTERMEDIARIO);
    // Selecionando promovido.
    promovido->chaves[0] = no->chaves[meio];
    promovido->offsets[0] = no->offsets[meio];
    // Garantindo filho do promovido.
    promovido->filhos[0] = rrn;
    promovido->filhos[1] = (arvb->c_arvb->proxRRN) - 1;
    // Limpando espaço
    no->chaves[meio] = -1;
    no->offsets[meio] = -1;
    no->nroChaves--;

    _EscreverNo(arvb, novoNO, arvb->c_arvb->proxRRN - 1);
    _ApagarNo(&novoNO);

    return promovido;
}

void ApagarArvoreB(ARVB **arvb) {
    if(arvb == NULL || *arvb == NULL) return;

    if((*arvb)->c_arvb) {
        EscreverCabecalhoIndice(&((*arvb)->arq_arvb), (*arvb)->c_arvb);
        ApagarCabecalhoIndice(&((*arvb)->c_arvb));
    }
    (*arvb)->arq_arvb = NULL;

    free(*arvb);
    *arvb = NULL;
}

/*
Cria um nó, passando o tipo
Folha = -1, Raiz = 0, Intermediario = 1
*/
NO *_CriarNo(int tipo) {
    NO *no = (NO*) malloc(sizeof(NO));
    // Inicializar nó.
    if (no != NULL) {
        no->tipoNo = tipo;
        no->nroChaves = 0;

        // Inicializa os espaços + extra
        // 1 extra no offset, chave e filhos.
        for(int i = 0; i <= MAX_CHAVES; i++) {
            no->chaves[i] = -1;
            no->filhos[i] = -1;
            no->offsets[i] = -1;
        }
        no->filhos[MAX_CHAVES + 1] = -1;
    }

    return no;
}

/*
Apaga um nó e liberá memória
*/
void _ApagarNo(NO **no) {
    if (no == NULL || *no == NULL) {
        return;
    }

    free(*no);
    *no = NULL;
}

/*
Lê um nó do arquivo de índice.
*/
NO *_LerNo(ARVB *arvb, int rrn) {
    if (arvb == NULL || arvb->arq_arvb == NULL) {
        return NULL;
    }

    if (rrn < 0 || rrn >= arvb->c_arvb->proxRRN) {
        printf("Error: rrn não existente.\n");
        exit(1);
    }

    fseek(arvb->arq_arvb, OffsetNo(rrn), SEEK_SET);

    // Cria um novo nó e preenche os valores
    NO *no = _CriarNo(FOLHA);

    if (no != NULL) {
        fread(&(no->tipoNo), sizeof(int), 1, arvb->arq_arvb);
        fread(&(no->nroChaves), sizeof(int), 1, arvb->arq_arvb);

        // Ler chaves, offsets e ponteiros(filhos)
        for(int i = 0; i < MAX_CHAVES; i++) {
            fread(&(no->filhos[i]), sizeof(int), 1, arvb->arq_arvb);
            fread(&(no->chaves[i]), sizeof(int), 1, arvb->arq_arvb);
            fread(&(no->offsets[i]), sizeof(long), 1, arvb->arq_arvb);
        }
        fread(&(no->filhos[MAX_CHAVES]), sizeof(int), 1, arvb->arq_arvb);
    }
    
    return no;
}

/*
Escreve os dados do nó no arquivo de índice.
*/
void _EscreverNo(ARVB *arvb, NO *no, int rrn) {
    if (arvb == NULL || arvb->arq_arvb == NULL || no == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    long int offset = OffsetNo(rrn);

    // Mover para a posição de inserção do nó
    fseek(arvb->arq_arvb, offset, SEEK_SET);

    fwrite(&(no->tipoNo), sizeof(int), 1, arvb->arq_arvb);
    fwrite(&(no->nroChaves), sizeof(int), 1, arvb->arq_arvb);

    // Escrever chaves, offsets e ponteiros(filhos)
    for(int i = 0; i < MAX_CHAVES; i++) {
        fwrite(&(no->filhos[i]), sizeof(int), 1, arvb->arq_arvb);
        fwrite(&(no->chaves[i]), sizeof(int), 1, arvb->arq_arvb);
        fwrite(&(no->offsets[i]), sizeof(long), 1, arvb->arq_arvb);
    }
    fwrite(&(no->filhos[MAX_CHAVES]), sizeof(int), 1, arvb->arq_arvb);
}

void _ImprimirNo(NO *no) {
    printf("\n\nNó da árvore B:\n");
    printf("Tipo do nó: %d\n", no->tipoNo);
    printf("Número de chaves: %d\n", no->nroChaves);
    for(int i = 0; i < no->nroChaves; i++){
        printf("Filho %d: %d\n", i, no->filhos[i]);
        printf("Chave %d: %d\n", i, no->chaves[i]);
        printf("Offsets %d: %ld\n", i, no->offsets[i]);
    }
    printf("Filho %d: %d\n\n", no->nroChaves, no->filhos[no->nroChaves]);
}

void AtualizarOffsetArvoreB(ARVB *arvb, int chave, long int offset){
    if(arvb == NULL) DispararErro(ErroPonteiroInvalido());
    
    int rrn = arvb->c_arvb->noRaiz; 
    
    while(rrn != -1) {
        // Carregar nó atual na memória
        NO *no = _LerNo(arvb, rrn);
        // Buscar possível posição da chave
        int i = 0;
        while(i < no->nroChaves && chave >= no->chaves[i]) i++;

        // Verificar se a chave foi encontrada
        if(i > 0 && chave == no->chaves[i - 1]) {
            no->offsets[i - 1] = offset;
            _EscreverNo(arvb, no, rrn);
            rrn = -1;
        // Caso contrário, continua-se a busca
        } else {
            rrn = no->filhos[i];
        }
        // Liberar memória
        _ApagarNo(&no);
    }
}

/* 
Executa busca em profundidade em um árvore B,
Guarda os indices dos nós buscados dado um critério
*/
void BuscaEmProfundidade(ARVB *arvb, 
                          int rrn, 
                          FILE *arquivoDados,
                          CRITERIO *crit,
                          int *vis,
                          FILA *indices) 
{
    if (rrn == -1 || vis[rrn]) return;

    vis[rrn] = 1;
    NO *no = _LerNo(arvb, rrn);

    for (int i = 0; i < no->nroChaves; i++) {
        // Visita filho à esquerda da chave i
        if (no->filhos[i] != -1) {
            BuscaEmProfundidade(arvb, no->filhos[i], arquivoDados, crit, vis, indices);
        }

        // Processa a chave i
        if (no->chaves[i] != -1) {
            fseek(arquivoDados, no->offsets[i], SEEK_SET);
            REGISTRO *reg = LerRegistro(arquivoDados);
            if (SelecionarPorCriterio(crit, reg)) {
                fila_inserir(indices, no->chaves[i]);
            }
            ApagarRegistro(&reg);
        }
    }

    // Visita o último filho
    if (no->filhos[no->nroChaves] != -1) {
        BuscaEmProfundidade(arvb, no->filhos[no->nroChaves], arquivoDados, crit, vis, indices);
    }

    _ApagarNo(&no);
}





   

