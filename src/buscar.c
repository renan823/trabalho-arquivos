#include "buscar.h"

#include "cabecalho.h"
#include "registro.h"

#include <stdlib.h>
#include <stdio.h>

void BuscarRegistros(FILE *arquivo) {
    CABECALHO *c = LerCabecalho(&arquivo);

    if (c == NULL) {
        // erro
        return;
    }

    // Cabeçalho inconsistente não será lido
    if (c->status == INCONSISTENTE) {
        // erro
        return;
    }

    // Ler dados 
}


void BuscarRegistrosCondicional(FILE *arquivo) {

}