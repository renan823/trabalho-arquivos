#ifndef REGISTRO_H_
    #define REGISTRO_H_

    #define SEPARADOR '|'
    #define REMOVIDO '1'
    typedef struct registro REGISTRO;

    #include "cabecalho.h"
    #include "criterio.h"

    #include <stdbool.h>

    /*
    Registro do arquivo de dados.

    - Gerenciamento de registros logicamente removidos:

    removido: 1 registro logicamente removido, 0 registro não marcado como removido(char - 1 byte)
    tamanhoRegistro: nº bytes do registro(inteiro - 4 bytes)
    prox: byte offset do próximo registro removido(long int - 8 bytes)

    - Campos de tamanho fixo:

    idAttack: código identificador do ataque(inteiro - 4 bytes)
    year: ano em que o ataque ocorreu(inteiro - 4 bytes)
    financialLoss: prejuízo causado pelo ataque(float - 4 bytes)

    - Campos de tamanho variável:

    country: país onde ocorreu o ataque(string)
    attackType: tipo de ameaça a segunrança cibernética(string)
    targetIndustry: setor da indústria que sofreu o ataque(string)
    defenseMechanism: estratégia de defesa cibernética empregada para resolver o problema(string)
    */

    struct registro {
        // Gerenciamento
        char removido;
        int tamanhoRegistro;
        long int prox;
        // Tamanho fixo
        int idAttack;
        int year;
        float financialLoss;
        // Tamanho variável
        char *country; 
        char *attackType; 
        char *targetIndustry; 
        char *defenseMechanism; 
    };

    REGISTRO *CriarRegistroVazio(void);
    void ExibirRegistro(REGISTRO *reg);
    void ApagarRegistro(REGISTRO **reg);
    void PreencherRegistro(REGISTRO **reg, char *buffer);
    void EscreverRegistro(FILE **arquivo, REGISTRO *reg);
    REGISTRO *LerRegistro(FILE *arquivo);
    void RemoverRegistro(FILE *arquivo, CABECALHO *c, REGISTRO *reg);
    REGISTRO *AtualizarRegistro(REGISTRO *regAtual, CRITERIO *novosDados);

#endif