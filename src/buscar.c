#include "buscar.h"

#include "erros.h"
#include "registro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool _ValidarRegistroFiltrado(REGISTRO* filtro, REGISTRO* reg) {
    if(filtro == NULL) {
        return true;
    } 

    if(reg == NULL) {
        return false;
    }
    /* Um registro é valido, se ele tem os filtros passados*/
    // Verifica idAttack
    if (filtro->idAttack != -1 && 
        filtro->idAttack != reg->idAttack) {
        return false;
    }
    
    // Verifica year
    if (filtro->year != -1 && 
        filtro->year != reg->year) {
        return false;
    }
    
    // Verifica financialLoss
    if (filtro->financialLoss != -1 && 
        filtro->financialLoss != reg->financialLoss) {
        return false;
    }
    
    // Verifica strings (country, attackType, etc.)
    if (filtro->country != NULL && 
        (reg->country == NULL || 
            strcmp(filtro->country, reg->country) != 0)) {
        return false;
    }
    
    if (filtro->attackType != NULL && 
        (reg->attackType == NULL || 
            strcmp(filtro->attackType, reg->attackType) != 0)) {
        return false;
    }
    
    if (filtro->targetIndustry != NULL && 
        (reg->targetIndustry == NULL || 
            strcmp(filtro->targetIndustry, reg->targetIndustry) != 0)) {
        return false;
    }
    
    if (filtro->defenseMechanism != NULL && 
        (reg->defenseMechanism == NULL || 
            strcmp(filtro->defenseMechanism, reg->defenseMechanism) != 0)) {
        return false;
    }
    
    // Se todas as verificações passaram
    return true;
}

void Buscar(FILE *arquivo) {
    // Se o arquivo de entrada não existir
    if(arquivo == NULL){
        // Dispara erro fatal.
        DispararErro(ErroPonteiroInvalido());
    }

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivo, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivo);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }

    // Atualizar ponteiro do arquivo para o início dos registros
    fseek(arquivo, TAM_HEAD, SEEK_SET);
    
    // Existe registro a ser buscado?
    bool existeRegistro = false;

    // Percorrer arquivo
    while(fread(&byteAtual, sizeof(char), 1, arquivo)){
        if(byteAtual == REMOVIDO){
            // Se registro removido, pular para o próximo registro
            int tamRegistro;
            fread(&tamRegistro, sizeof(int), 1, arquivo);
            fseek(arquivo, tamRegistro, SEEK_CUR);
        } else {
            existeRegistro = true;
            REGISTRO *regAtual = LerRegistro(arquivo);

            if(regAtual) {
                ExibirRegistro(regAtual);
            } else {
                DispararErro(ErroAoLerRegistro());
            }

            ApagarRegistro(&regAtual);
        }
    }

    if(existeRegistro == false){
        DispararErro(ErroRegistroInexistente());
    }

    return;
}

void BuscarComFiltro(FILE *arquivo, REGISTRO *reg) {
    // Se o arquivo de entrada não existir
    if(arquivo == NULL){
        // Dispara erro fatal.
        DispararErro(ErroPonteiroInvalido());
    }

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivo, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivo);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }

    // Atualizar ponteiro do arquivo para o início dos registros
    fseek(arquivo, TAM_HEAD, SEEK_SET);
    
    // Existe registro a ser buscado?
    bool registroEncontrado = false;

    // Percorrer arquivo
    while(fread(&byteAtual, sizeof(char), 1, arquivo)){
        if(byteAtual == REMOVIDO){
            // Se registro removido, pular para o próximo registro
            int tamRegistro;
            fread(&tamRegistro, sizeof(int), 1, arquivo);
            fseek(arquivo, tamRegistro, SEEK_CUR);
        } else {
            REGISTRO *regAtual = LerRegistro(arquivo);

            if(_ValidarRegistroFiltrado(reg, regAtual)){
                registroEncontrado = true;
                ExibirRegistro(regAtual);
            }

            ApagarRegistro(&regAtual);
        }
    }

    if(registroEncontrado == false){
        DispararErro(ErroRegistroInexistente());
    }

    printf("**********\n");

    return;
}

void RemoverRegistrosComFiltro(FILE *arquivo, REGISTRO *reg) {
    // Se o arquivo de entrada não existir
    if(arquivo == NULL){
        // Dispara erro fatal.
        DispararErro(ErroArquivoInvalido());
    }

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivo, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivo);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }

    // Atualizar ponteiro do arquivo para o início dos registros
    fseek(arquivo, TAM_HEAD, SEEK_SET);

    // Necessário para atualizar o cabeçalho.
    int quantRegRemovidos = 0;

    // Percorrer arquivo
    while(fread(&byteAtual, sizeof(char), 1, arquivo)){
        if(byteAtual == REMOVIDO){
            // Se registro removido, pular para o próximo registro
            int tamRegistro;
            fread(&tamRegistro, sizeof(int), 1, arquivo);
            fseek(arquivo, tamRegistro, SEEK_CUR);
        } else {
            REGISTRO *regAtual = LerRegistro(arquivo);
            if(regAtual == NULL){
                DispararErro(ErroAoLerRegistro());
            }

            if(_ValidarRegistroFiltrado(reg, regAtual)){
                // TO-DO: Lógica de remoção de um registro
                RemoverRegistro(arquivo, regAtual);
                quantRegRemovidos++;

            }

            ApagarRegistro(&regAtual);
        }
    }

    CABECALHO *c = LerCabecalho(&arquivo);
    c->nroRegRem += quantRegRemovidos;
    c->nroRegArq -= quantRegRemovidos;
    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);

    return;
}

void InserirRegistro(FILE *arquivo, REGISTRO *reg) {
    // Se o arquivo de entrada não existir
    if(arquivo == NULL){
        // Dispara erro fatal.
        DispararErro(ErroArquivoInvalido());
    }
    CABECALHO *c = LerCabecalho(&arquivo);

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivo, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivo);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }

    // Verifica se houve inserção no fim.
    bool inseriFim = false;

    // Variáveis que auxiliares na manipulação do arq. bin.
    long int byteOffset; // byteOffset do reg. atual
    long int origem = 1; // byteOffset do anterior do reg. atual

    fread(&byteOffset, sizeof(long int), 1, arquivo);
    if(byteOffset == -1){
        fseek(arquivo, 0, SEEK_END);
        EscreverRegistro(&arquivo, reg);
        inseriFim = true;
    } else {
        // Percorrer a lista de registros removidos, buscando espaço.
        while(fseek(arquivo, byteOffset + 1, SEEK_SET) == 0){
            int tamRegAtual;
            fread(&tamRegAtual, sizeof(int), 1, arquivo);
            if (tamRegAtual >= reg->tamanhoRegistro){
                /*Nota: Caso deva trocar o tamanho do reg atual
                pelo tamanho do registro, deve-se arrumar a logica 
                de outras funções, caso contrário, deve-se ajustar a remover
                para att o espaço do registro para um valor do registro anterior até
                o próximo registro, validos.*/
                long int prox;
                fread(&prox, sizeof(long int), 1, arquivo);
                fseek(arquivo, byteOffset, SEEK_SET);
                EscreverRegistro(&arquivo, reg);

                // Ajustar lista atualizando reg. anterior
                // Se a origem for um registro, pula-se para o campo prox.
                if(origem != 1) origem += 5;
                // Busca posição para atualizar o prox da lista.  
                fseek(arquivo, origem, SEEK_SET);
                fwrite(&prox, sizeof(long int), 1, arquivo);
            } else {
                origem = byteOffset;
                byteOffset = reg->prox;
            } 

            if(byteOffset == -1){
                fseek(arquivo, 0, SEEK_END);
                EscreverRegistro(&arquivo, reg);
                inseriFim = true;
                break;
            }
        }
    }
    

    c->nroRegRem -= (inseriFim ? 0 : 1);
    c->nroRegArq++;
    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);

    return;
}