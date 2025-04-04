#include "registro.h"

#include <stdlib.h>
#include <stdio.h>

/*
Aloca memória para um registro.
Dispara erro em caso de ponteiro NULL
*/
REGISTRO *CriarRegistroVazio(void) {
    REGISTRO *reg = (REGISTRO*) malloc(sizeof(REGISTRO));

    if (reg == NULL) {
        //erro!
    }

    return reg;
}