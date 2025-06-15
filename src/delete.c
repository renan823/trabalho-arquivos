#include "delete.h"
#include "erros.h"
#include "buscar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void DELETE(FILE *arquivo, CRITERIO *crit) {
    // Se arquivo nulo, encerra execução.
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    CABECALHO *c = LerCabecalho(&arquivo);
    if(c->status == INCONSISTENTE) {
        ApagarCabecalho(&c);
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    // Buscar registro sem filtro(todos os não removidos)
    REGISTRO *registroBuscado = NULL;
    while (c->nroRegArq != 0 &&
        (registroBuscado = SELECT_WHERE(arquivo, crit)) != NULL) {
        fseek(arquivo, -(registroBuscado->tamanhoRegistro + 5), SEEK_CUR);
        RemoverRegistro(arquivo, c, registroBuscado);
        ApagarRegistro(&registroBuscado);
    }    

    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);

    return;
}
