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
    reg->removido = '0';
    // Tamanho fixo do registro
    reg->tamanhoRegistro = 20;
    // Inicializar campos fixos com -1.
    reg->prox = -1;
    reg->idAttack = -1;
    reg->year = -1;
    reg->financialLoss = -1;
    // Campos variaveis setar para NULL
    reg->country = NULL; 
    reg->attackType = NULL; 
    reg->targetIndustry = NULL; 
    reg->defenseMechanism = NULL;

    return reg;
}

/*
Exibe os campos do regsitro, cuidando de valores nulos/vazios.
// Função precisa ser ajustada para ler o cabeçalho do arquivo, pois o criar cabelho cria se \0,
ou seja, ao tentar printar como string, printa todo o arquivo.
*/

void ExibirRegistro(REGISTRO *reg) {
    CABECALHO *c = CriarCabecalhoPadrao();
    if (reg == NULL || c == NULL) {
        // erro!
        return;
    }

    // Campo idAttack
    if (reg->idAttack == -1) printf("%s: %s\n", c->descreveIdentificador, MSG_VAZIO);
    else printf("%s: %d\n", c->descreveIdentificador, reg->idAttack);
    // Campo year
    if (reg->year == -1) printf("%s: %s\n", c->descreveYear, MSG_VAZIO);
    else printf("%s: %d\n", c->descreveYear, reg->year);
    // Campo country
    printf("%s: %s\n", c->descreveCountry, 
        reg->country == NULL ? MSG_VAZIO : reg->country);
    // Campo targetIndustry
    printf("%s: %s\n", c->descreveTargetIndustry, 
                        reg->targetIndustry == NULL ? MSG_VAZIO:reg->targetIndustry);
    // Campo attackType
    printf("%s: %s\n", c->descreveType,
                        reg->attackType == NULL ? MSG_VAZIO : reg->attackType);
    // Campo financialLoss
    if (reg->financialLoss == -1) printf("%s: %s\n", c->descreveFinancialLoss, MSG_VAZIO);
    else printf("%s: %.2f\n", c->descreveFinancialLoss, reg->financialLoss);
    // Campo defenseMechanism
    printf("%s: %s\n", c->descreveDefense,
                        reg->defenseMechanism == NULL ? MSG_VAZIO : reg->defenseMechanism);

    ApagarCabecalho(&c);

    // Pular linha
    printf("\n");
}
