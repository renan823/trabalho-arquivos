#ifndef SQL_H_
    #define SQL_H_

    #include "cabecalho.h"
    #include "registro.h"
    #include "criterio.h"

    #include <stdio.h>
    /* Ler arquivo CSV e escreve em binário. */
    void LerCsvParaBinario(FILE *arquivoEntrada, FILE *arquivoSaida);

    /* Busca */
    void ExibirRegistrosDadoCriterio(FILE *arquivo, CRITERIO *reg);
    REGISTRO *SELECT_WHERE(FILE *arquivo, CRITERIO *filtro);

    /* Remoção */
    void RemoverRegistroDadoCriterio(FILE *arquivo, CRITERIO *crit);
    void DELETE(FILE *arquivo, CABECALHO *c, REGISTRO *reg);

    /* Inserção */
    void InserirRegistro(FILE *arquivo, REGISTRO *reg);
    void INSERT(FILE *arquivo, CABECALHO *c, REGISTRO *reg);

    /* Atualização */
    void AtualizarRegistroDadoCriterio(FILE *arquivo, CRITERIO *crit, CRITERIO *valoresAtualizados);
    long int UPDATE(FILE *arquivo, CABECALHO *c, CRITERIO *valoresAtualizados, REGISTRO *regBuscado);


#endif