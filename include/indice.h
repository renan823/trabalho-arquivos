#ifndef INDICE_H_
    #define INDICE_H_

    #define FOLHA -1
    #define RAIZ 0
    #define INTERMEDIARIO 1
    #define TAM_REG_IND 44 // Tamanho do cabeçalho / registro do índice
    #define MAX_CHAVES 2
    #define MAX_PTRS 3

    #include <stdio.h>

    typedef struct cabecalho_indice CABECALHO_INDICE;
    struct cabecalho_indice {
        char status; // 1 byte
        int noRaiz; // 4 bytes
        int proxRRN; // 4 bytes
        int nroNos; // 4 bytes
        char lixo[31]; // 31 bytes
    };

    typedef struct no_indice NO_INDICE;
    struct no_indice {
        int tipoNo; // 4 bytes
        int nroChaves; // 4 bytes
        int P1; // 4 bytes
        int C1; // 4 bytes
        long int Pr1; // 8 bytes
        int P2; // 4 bytes
        int C2; // 4 bytes
        long int Pr2; // 8 bytes
        int P3; // 4 bytes
    };

    CABECALHO_INDICE *CriarCabecalhoIndicePadrão(void);
    void ApagarCabecalhoIndice(CABECALHO_INDICE **c);
    CABECALHO_INDICE *LerCabecalhoIndice(FILE **arquivo);
    void EscreverCabecalhoIndice(FILE **arquivo, CABECALHO_INDICE *c);

    NO_INDICE *CriarNoIndice(int tipo);
    void ApagarNoIndice(NO_INDICE **no);
    NO_INDICE *LerNoIndice(FILE **arquivo, int rrn);
    void EscreverNoIndice(FILE **arquivo, NO_INDICE *no, int rrn);

    void AdicionarChave(FILE **arquivo, int chave, long offset);
    long RemoverChave(FILE **arquivo, int chave);
    void AtualizarChave(FILE **arquivo, int chave, long offset);
    long BuscarChave(FILE **arquivo, int chave);
#endif