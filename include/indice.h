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
    #include "registro.h"
    #include "fila.h"

    long OffsetNo(int rrn);    
    void CriarArquivoIndice(FILE *arquivoEntrada, FILE *arquivoSaida);
    void ExibirRegistroDadoIndice(FILE *arquivoDados, FILE *arquivoIndices, int indice);
    void InserirRegistroIndice(FILE *arquivoDados, FILE *arquivoIndices, REGISTRO *reg);
    void AtualizarRegistroDadoIndice(FILE *arquivoDados, FILE *arquivoIndices,
                                    int indice, CRITERIO *valoresAtualizados);
    FILA *RertornaIndicesDadoCriterio(FILE *arquivoDados,
                                    FILE *arquivoIndices,
                                    CRITERIO *criterio);
                                       
#endif