#ifndef INDICE_H_
    #define INDICE_H_

    #define FOLHA -1
    #define RAIZ 0
    #define INTERMEDIARIO 1
    #define TAM_REG_IND 44 // Tamanho do cabeçalho / registro do índice
    #define MAX_CHAVES 2
    #define MAX_FILHOS 3
    #define PTR_ESQ 1
    #define PTR_MED 2
    #define PTR_DIR 3

    #include <stdio.h>
    #include "cabecalho.h"

    // Struct auxiliar para as promoções de chave / offset / rrn
    typedef struct promovido PROMOVIDO;
    struct promovido {
        int chave;
        int offset;
        int rrn;
    };

    typedef struct _no NO;
    struct _no {
        int tipoNo; // 4 bytes
        int nroChaves; // 4 bytes
        int chaves[MAX_CHAVES + 1];
        long int offsets[MAX_CHAVES + 1];
        int filhos[MAX_FILHOS + 1];
    };

    typedef struct _arvb ARVB;

    struct _arvb
    {
        FILE *arq_arvb;
        CABECALHO_ARVOREB *c_arvb;
    };

    ARVB *CriarArvoreB(FILE *arquivo);
    void ApagarArvoreB(ARVB **arvb);
    void InserirArvoreB(ARVB *arvb, int chave, long int offset);

    void AdicionarChaveIndice(FILE **arquivo, int chave, long offset);
    long RemoverChaveIndice(FILE **arquivo, int chave);
    long BuscarChaveIndice(FILE **arquivo, int chave);

    void AbrirIndice(FILE **arquivo, char *nome);
#endif