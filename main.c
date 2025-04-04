#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char descreveIdentificador[23];
    char descreveYear[27];
    char descreveFinancialLoss[28];
    char codDescreveCountry;
    char descreveCountry[26];
    char codDescreveType;
    char descreveType[38];
    char codDescreveTargetIndustry;
    char descreveTargetIndustry[38];
    char codDescreveDefense;
    char descreveDefense[67];
};

/*
Função para gerar um cabeçalho de dados padrão.
*/
CABECALHO *CriarCabecalhoPadrao(void) {
    CABECALHO *c = (CABECALHO*) malloc(sizeof(CABECALHO));

    if (c != NULL) {
        c->status = '1';
        c->topo = -1;
        c->proxByteOffset = 0;
        c->nroRegArq = 0;
        c->nroRegRem = 0;
        memcpy(c->descreveIdentificador, "IDENTIFICADOR DO ATAQUE", 23);
        memcpy(c->descreveYear, "ANO EM QUE O ATAQUE OCORREU", 27);
        memcpy(c->descreveFinancialLoss, "PREJUIZO CAUSADO PELO ATAQUE", 28);
        c->codDescreveCountry = '1';
        memcpy(c->descreveCountry, "PAIS ONDE OCORREU O ATAQUE", 26);
        c->codDescreveType = '2';
        memcpy(c->descreveType, "TIPO DE AMEACA A SEGURANCA CIBERNETICA", 38);
        c->codDescreveTargetIndustry = '3';
        memcpy(c->descreveTargetIndustry, "SETOR DA INDUSTRIA QUE SOFREU O ATAQUE", 38);
        c->codDescreveDefense = '4';
        memcpy(c->descreveDefense, "ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA", 67);
    }

    return c;
}

/* 
Escreve o cabeçalho de dados no arquivo informado
*/
void EscreverCabecalho(FILE *arquivo, CABECALHO *c) {
    if (arquivo == NULL || c == NULL) {
        return;
    }

    // Escrever dados
    fwrite(&(c->status), sizeof(char), 1, arquivo);
    fwrite(&(c->topo), sizeof(long int), 1, arquivo);
    fwrite(&(c->proxByteOffset), sizeof(long int), 1, arquivo);
    fwrite(&(c->nroRegArq), sizeof(int), 1, arquivo);
    fwrite(&(c->nroRegRem), sizeof(int), 1, arquivo);
    fwrite(c->descreveIdentificador, sizeof(char), 23, arquivo);
    fwrite(c->descreveYear, sizeof(char), 27, arquivo);
    fwrite(c->descreveFinancialLoss, sizeof(char), 28, arquivo);
    fwrite(&(c->codDescreveCountry), sizeof(char), 1, arquivo);
    fwrite(c->descreveCountry, sizeof(char), 26, arquivo);
    fwrite(&(c->codDescreveType), sizeof(char), 1, arquivo);
    fwrite(c->descreveType, sizeof(char), 38, arquivo);
    fwrite(&(c->codDescreveTargetIndustry), sizeof(char), 1, arquivo);
    fwrite(c->descreveTargetIndustry, sizeof(char), 38, arquivo);
    fwrite(&(c->codDescreveDefense), sizeof(char), 1, arquivo);
    fwrite(c->descreveDefense, sizeof(char), 67, arquivo);
}

int main(void) {
    FILE *arquivo = fopen("teste.bin", "rb");

    char buffer[276];
    fread(buffer, 1, 276, arquivo);
    
    fwrite(buffer, 1, 276, stdout);

    fclose(arquivo);

    return 0;
}