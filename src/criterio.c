#include "erros.h"
#include "cabecalho.h"
#include "utils.h"
#include "criterio.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MSG_VAZIO "NADA CONSTA"
#define LIXO '$'

/* Compara strings tratando NULL como valor válido */
bool _comparaCampoVar(char *campo1, char *campo2) {
    // Ambos NULL são considerados iguais
    if (campo1 == NULL && campo2 == NULL) {
        return true;
    }
    // Apenas um NULL são considerados diferentes
    if (campo1 == NULL || campo2 == NULL) {
        return false;
    }
    // Ambos não NULL, compara conteúdo
    return strcmp(campo1, campo2) == 0;
}

CRITERIO *CriarCriterioVazio(void) {
  CRITERIO *criterio = (CRITERIO*) malloc(sizeof(CRITERIO));
  if(criterio == NULL) DispararErro(ErroAlocacaoMemoria());

  // Inicialmente não há criterios
  criterio->temIdAttack = 0;
  criterio->temYear = 0;
  criterio->temFinancialLoss = 0;
  criterio->temCountry = 0;
  criterio->temAttackType = 0;
  criterio->temTargetIndustry = 0;
  criterio->temDefenseMechanism = 0;
  
  criterio->criterios = CriarRegistroVazio();
  
  return criterio;
}


CRITERIO *DefinirCriterio(){
    // Quantidade de campos em cada busca(filtros).
    int quantCampos;
    scanf("%d", &quantCampos);

    // Registro a ser usado como filtro
    CRITERIO *c = CriarCriterioVazio();

    // Ler campos inseridos pelo usuário
    for(int i = 0; i < quantCampos; i++){
        char *campo = LerString();
        // Switch case do campo
        if(!strcmp(campo, "idAttack")){
            scanf(" %d", &(c->criterios->idAttack));
            c->temIdAttack = 1;
        } else if(!strcmp(campo, "year")) {
            c->criterios->year = LerCampoInteiro();
            c->temYear = 1;
        } else if(!strcmp(campo, "financialLoss")) {
            c->criterios->financialLoss = LerCampoFloat();
            c->temFinancialLoss = 1;
        } else if(!strcmp(campo, "country")) {
            c->criterios->country = LerStringComAspas();
            c->temCountry = 1;
        } else if(!strcmp(campo, "attackType")) {
            c->criterios->attackType = LerStringComAspas();
            c->temAttackType = 1;
        } else if(!strcmp(campo, "targetIndustry")) {
            c->criterios->targetIndustry = LerStringComAspas();
            c->temTargetIndustry = 1;
        } else if(!strcmp(campo, "defenseMechanism")) {
            c->criterios->defenseMechanism = LerStringComAspas();
            c->temDefenseMechanism = 1;
        } else {
            printf("Erro ao ler campo\n");
        }

        free(campo);
        campo = NULL;
    }
    return c;
}

// Verifica se o registro é valido dado critério
bool SelecionarPorCriterio(CRITERIO* crit, REGISTRO* reg) {
    if(reg == NULL || reg->removido == REMOVIDO) return false;
    // Se não há critério e registro não removido e não nulo
    // o registro é valido.
    if(crit == NULL) return true;

    /* Um registro é valido, se ele tem os criterios passados*/
    // Verifica idAttack
    if (crit->temIdAttack) {
      if (crit->criterios->idAttack != reg->idAttack) {
        return false;
      }
    }

    // Verifica year
    if (crit->temYear) {
      if (crit->criterios->year != reg->year) {
        return false;
      }
    }

    // Verifica financialLoss
    if (crit->temFinancialLoss) {
      if (crit->criterios->financialLoss != reg->financialLoss) {
        return false;
      }
    }
    
    // Verifica se há campo variavel e se não é igual.
    // Campo variável country
    if (crit->temCountry && 
      !_comparaCampoVar(crit->criterios->country, reg->country)) {
      return false;
    }
    // Campo variável attackType
    if (crit->temAttackType && 
      !_comparaCampoVar(crit->criterios->attackType, reg->attackType)) {
      return false;
    }
    // Campo variável targetIndustry
    if (crit->temTargetIndustry && 
      !_comparaCampoVar(crit->criterios->targetIndustry, reg->targetIndustry)) {
      return false;
    }
    // Campo variável defenseMechanism
    if (crit->temDefenseMechanism && 
      !_comparaCampoVar(crit->criterios->defenseMechanism, reg->defenseMechanism)) {
      return false;
    }

    // Se todas as verificações passaram
    return true;
}

void ApagarCriterio(CRITERIO **criterio){
  if (criterio == NULL) DispararErro(ErroPonteiroInvalido());

  if (*criterio != NULL) {
    ApagarRegistro(&(*criterio)->criterios);
    (*criterio)->criterios = NULL;

    free((*criterio));
    (*criterio) = NULL;
  }

  return;
}
