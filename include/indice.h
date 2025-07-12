#ifndef INDICE_H_
    #define INDICE_H_

    #define FOLHA -1
    #define RAIZ 0
    #define INTERMEDIARIO 1
    #define ORDEM 6
    #define MAX_CHAVES (ORDEM - 1)
    #define MIN_CHAVES (ORDEM/2 - 1)
    #define MAX_FILHOS ORDEM
    // Tipo do nó(int), nro chaves(int), chaves(int), 
    // filhos(int), offsets(long int).
    #define TAM_REG_IND (8 + MAX_CHAVES*4 + MAX_FILHOS*4 + MAX_CHAVES*8) 


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