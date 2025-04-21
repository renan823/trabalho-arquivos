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