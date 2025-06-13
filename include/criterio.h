#ifndef CRITERIO_H_
    #define CRITERIO_H_

    #include "cabecalho.h"
    #include "registro.h"

    #include <stdbool.h>

    typedef struct criterio CRITERIO;

    struct criterio {
      int temIdAttack;
      int temYear;
      int temFinancialLoss;
      int temCountry; 
      int temAttackType; 
      int temTargetIndustry; 
      int temDefenseMechanism;
      REGISTRO *criterios;
    };

    CRITERIO *CriarCriterioVazio(void);
    void ApagarCriterio(CRITERIO **criterio);
    bool SelecionarPorCriterio(CRITERIO* crit, REGISTRO* reg);
    CRITERIO *DefinirCriterio();

#endif