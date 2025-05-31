#ifndef BUSCAR_H_
    #define BUSCAR_H_

    #include "registro.h"

    #include <stdio.h>

    void ExibirRegistrosDadoCriterio(FILE *arquivo, REGISTRO *reg);
    REGISTRO *SELECT_WHERE(FILE *arquivo, REGISTRO *filtro);

#endif