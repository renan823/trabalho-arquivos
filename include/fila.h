#ifndef FILA_H
    #define FILA_H

    typedef struct fila_ FILA;

    #include "stdbool.h"

    FILA* fila_criar(void);
    bool fila_cheia(FILA *fila);
    bool fila_vazia(FILA *fila);
    bool fila_inserir(FILA *fila, int chave); 
    int fila_remover(FILA *fila);
    int fila_tamanho(FILA *fila);
    int fila_frente(FILA *fila);
    void fila_imprimir(FILA *fila);
    bool fila_inverter(FILA *fila);
    void fila_apagar(FILA **fila);

#endif