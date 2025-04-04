#include "cabecalho.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Função para gerar um cabeçalho de dados padrão.
*/
CABECALHO *CriarCabecalhoPadrao(void) {
    CABECALHO *c = (CABECALHO*) malloc(sizeof(CABECALHO));

    // Se houver espaço na memória heap
    if (c != NULL) {
        // Inicializar campos do cabecalho
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
Função que escreve o cabeçalho de dados no arquivo informado
*/
void EscreverCabecalho(FILE **arquivo, CABECALHO *c) {
    if (*arquivo == NULL || c == NULL) {
        return;
    }

    // Escrever dados
    fwrite(&(c->status), sizeof(char), 1, *arquivo);
    fwrite(&(c->topo), sizeof(long int), 1, *arquivo);
    fwrite(&(c->proxByteOffset), sizeof(long int), 1, *arquivo);
    fwrite(&(c->nroRegArq), sizeof(int), 1, *arquivo);
    fwrite(&(c->nroRegRem), sizeof(int), 1, *arquivo);
    fwrite(c->descreveIdentificador, sizeof(char), 23, *arquivo);
    fwrite(c->descreveYear, sizeof(char), 27, *arquivo);
    fwrite(c->descreveFinancialLoss, sizeof(char), 28, *arquivo);
    fwrite(&(c->codDescreveCountry), sizeof(char), 1, *arquivo);
    fwrite(c->descreveCountry, sizeof(char), 26, *arquivo);
    fwrite(&(c->codDescreveType), sizeof(char), 1, *arquivo);
    fwrite(c->descreveType, sizeof(char), 38, *arquivo);
    fwrite(&(c->codDescreveTargetIndustry), sizeof(char), 1, *arquivo);
    fwrite(c->descreveTargetIndustry, sizeof(char), 38, *arquivo);
    fwrite(&(c->codDescreveDefense), sizeof(char), 1, *arquivo);
    fwrite(c->descreveDefense, sizeof(char), 67, *arquivo);
}

/* 
Função que libera espaço alocado par ao cabecalho na heap
*/
void ApagarCabecalho(CABECALHO **c){
    if (*c == NULL) {
        return;
    }
    
    // Liberar memoria do cabecalho
    free(*c);
    *c = NULL; // Evita ponteiro selvagem
}