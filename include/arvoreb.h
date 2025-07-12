#ifndef ARVB_H
    #define ARVB_H

    #include <stdio.h>
    #include "stdbool.h"
    #include "fila.h"
    #include "registro.h"
    #include "criterio.h"

    typedef struct _arvb ARVB;
    
    ARVB *CriarArvoreB(FILE *arquivo);
    void ApagarArvoreB(ARVB **arvb);
    void InserirArvoreB(ARVB *arvb, int chave, long int offset);
    long int BuscarArvoreB(ARVB *arvb, int chave);
    void AtualizarOffsetArvoreB(ARVB *arvb, int chave, long int offset);
    int NroNosArvoreB(ARVB *arvb);
    int NoRaizArvoreB(ARVB *arvb);
    void ImprimirArvoreB(ARVB *arvb);
    void RemoverArvoreB(ARVB *arvb, int chave);
    void BuscaEmProfundidade(ARVB *arvb, int rrn, 
                            FILE *arquivoDados, CRITERIO *crit,
                            int *vis, FILA *indices);

#endif