#include "registro.h"

#include "cabecalho.h"

#include <stdlib.h>
#include <stdio.h>

#define MSG_VAZIO "NADA CONSTA"

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

/*
Exibe os campos do regsitro, cuidando de valores nulos/vazios.
*/
void ExibirRegistro(REGISTRO *reg, CABECALHO *c) {
    if (reg == NULL || c == NULL) {
        return;
    }

    // Printar todos os campos
    printf("%s: %d\n", c->descreveIdentificador, reg->idAttack);
    
    if (reg->year == 0) {
        printf("%s: %s\n", c->descreveYear, MSG_VAZIO);
    } else {
        printf("%s: %d\n", c->descreveYear, reg->year);
    }

    printf("%s: %s\n", c->descreveDefense, reg->country == NULL ? MSG_VAZIO : reg->country);
    printf("%s: %s\n", c->descreveTargetIndustry, reg->targetIndustry == NULL ? MSG_VAZIO : reg->targetIndustry);
    printf("%s: %s\n", c->descreveType, reg->attackType == NULL ? MSG_VAZIO : reg->attackType);
    
    if (reg->financialLoss == 0) {
        printf("%s: %s\n", c->descreveFinancialLoss, MSG_VAZIO);
    } else {
        printf("%s: %.2f\n", c->descreveFinancialLoss, reg->financialLoss);
    }

    printf("%s: %s\n", c->descreveDefense, reg->defenseMechanism == NULL ? MSG_VAZIO : reg->defenseMechanism);

    // Pular linha
    printf("\n");
}