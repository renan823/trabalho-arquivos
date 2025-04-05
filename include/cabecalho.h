#ifndef CABECALHO_H_
    #define CABECALHO_H_

    #include <stdio.h>

    /*
    Cabeçalho do arquivo de dados.

    status: 0 arquivo inconsistente, 1 arquivo consistente
    topo: byte offset de um registro removido (-1 caso não haja registro)
    proxByteOffset: armazena o próximo byte offset, iniciado em zero
    nroRegArq: numero de registros não removidos, iniciado em zero
    nroRegRem: numero de registros removidos, iniciado em zero
    descreveIdentificador: descrição do idAttack (por padrão: IDENTIFICADOR DO ATAQUE)
    descreveYear: descrição do year (por padrão: ANO EM QUE O ATAQUE OCORREU)
    descreveFinancialLoss: descrição do financialLoss (por padrão: PREJUIZO CAUSADO PELO ATAQUE)
    codDescreveContry: código da keyword de country (por padrão: 1)
    descreveCountry: descrição do country (por padrão: PAIS ONDE OCORREU O ATAQUE)
    codDescreveType: código da keyword de attackType (por padrão: 2)
    descreveType: descrição de attackType (por padrão: TIPO DE AMEACA A SEGURANCA CIBERNETICA)
    codDescreveTargetIndustry: keyword para targetIndustry (por padrão: 3)
    descreveTargetIndustry: descrição de targetIndustry (por padrão: SETOR DA INDUSTRIA QUE SOFREU O ATAQUE)
    codDescreveDefense: keyword de defenseMechanism (por padrão: 4);
    descreveDefense: descrição de defenseMechanism (por padrão: ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA)
    */

    typedef struct cabecalho CABECALHO;

    struct cabecalho {
        char status;
        long int topo;
        long int proxByteOffset;
        int nroRegArq;
        int nroRegRem;
        char descreveIdentificador[24];
        char descreveYear[28];
        char descreveFinancialLoss[29];
        char codDescreveCountry;
        char descreveCountry[27];
        char codDescreveType;
        char descreveType[39];
        char codDescreveTargetIndustry;
        char descreveTargetIndustry[39];
        char codDescreveDefense;
        char descreveDefense[68];
    };

    CABECALHO *CriarCabecalhoPadrao(void);
    void ApagarCabecalho(CABECALHO **c);

    CABECALHO *LerCabecalho(FILE **arquivo);
    void EscreverCabecalho(FILE **arquivo, CABECALHO *c);
#endif