#include "cabecalho.h"

#include "erros.h"

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
        c->status = CONSISTENTE;
        c->topo = -1;
        c->proxByteOffset = TAM_HEAD - 1;
        c->nroRegArq = 0;
        c->nroRegRem = 0;
        memcpy(c->descreveIdentificador, "IDENTIFICADOR DO ATAQUE\0", 24);
        memcpy(c->descreveYear, "ANO EM QUE O ATAQUE OCORREU\0", 28);
        memcpy(c->descreveFinancialLoss, "PREJUIZO CAUSADO PELO ATAQUE\0", 29);
        c->codDescreveCountry = '1';
        memcpy(c->descreveCountry, "PAIS ONDE OCORREU O ATAQUE\0", 27);
        c->codDescreveType = '2';
        memcpy(c->descreveType, "TIPO DE AMEACA A SEGURANCA CIBERNETICA\0", 39);
        c->codDescreveTargetIndustry = '3';
        memcpy(c->descreveTargetIndustry, "SETOR DA INDUSTRIA QUE SOFREU O ATAQUE\0", 39);
        c->codDescreveDefense = '4';
        memcpy(c->descreveDefense, "ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA\0", 68);
    }

    return c;
}

/*
Função que atualiza o cabeçalho de um arquivo binário
OBS: campos com -1 não são alterados, o valor status é obrigatorio.
*/
void AtualizarCabecalho(
    FILE **arquivo,
    char status,
    long int topo,
    long int proxByteOffset,
    int nroRegArq,
    int nroRegRem
){
    if (*arquivo == NULL) {
        // Dispara erro fatal
        DispararErro(ErroPonteiroInvalido());
    }

    // Garante início do arquivo
    fseek(*arquivo, 0, SEEK_SET);

    // Sempre é necessário enviar o valor do status
    fwrite(&(status), sizeof(char), 1, *arquivo);

    // Altera os campos se forem != -1
    if(topo != -1) fwrite(&(topo), sizeof(long int), 1, *arquivo);
    else fseek(*arquivo, sizeof(long int),SEEK_CUR);

    if(proxByteOffset != -1) fwrite(&(proxByteOffset), sizeof(long int), 1, *arquivo);
    else fseek(*arquivo, sizeof(long int),SEEK_CUR);

    if(nroRegArq != -1) fwrite(&(nroRegArq), sizeof(int), 1, *arquivo);
    else fseek(*arquivo, sizeof(int),SEEK_CUR);

    if(nroRegRem != -1) fwrite(&(nroRegRem), sizeof(int), 1, *arquivo);
    else fseek(*arquivo, sizeof(int),SEEK_CUR);

    return;
}

/*
Função que lê o cabeçalho do arquivo informado
*/
CABECALHO *LerCabecalho(FILE **arquivo) {
    if (*arquivo == NULL) {
        return NULL;
    }

    // Garante início do arquivo
    fseek(*arquivo, 0, SEEK_SET);

    CABECALHO *c = (CABECALHO*) malloc(sizeof(CABECALHO));

    if (c != NULL) {
        fread(&(c->status), sizeof(char), 1, *arquivo);
        fread(&(c->topo), sizeof(long int), 1, *arquivo);
        fread(&(c->proxByteOffset), sizeof(long int), 1, *arquivo);
        fread(&(c->nroRegArq), sizeof(int), 1, *arquivo);
        fread(&(c->nroRegRem), sizeof(int), 1, *arquivo);
        fread(c->descreveIdentificador, sizeof(char), 23, *arquivo);
        fread(c->descreveYear, sizeof(char), 27, *arquivo);
        fread(c->descreveFinancialLoss, sizeof(char), 28, *arquivo);
        fread(&(c->codDescreveCountry), sizeof(char), 1, *arquivo);
        fread(c->descreveCountry, sizeof(char), 26, *arquivo);
        fread(&(c->codDescreveType), sizeof(char), 1, *arquivo);
        fread(c->descreveType, sizeof(char), 38, *arquivo);
        fread(&(c->codDescreveTargetIndustry), sizeof(char), 1, *arquivo);
        fread(c->descreveTargetIndustry, sizeof(char), 38, *arquivo);
        fread(&(c->codDescreveDefense), sizeof(char), 1, *arquivo);
        fread(c->descreveDefense, sizeof(char), 67, *arquivo);
    }

    return c;
}

/* 
Função que escreve o cabeçalho de dados no arquivo informado
*/
void EscreverCabecalho(FILE **arquivo, CABECALHO *c) {
    if (*arquivo == NULL || c == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    // Garante início do arquivo
    fseek(*arquivo, 0, SEEK_SET);

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