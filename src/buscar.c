#include "buscar.h"

#include "erros.h"
#include "registro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void ExibirRegistrosDadoCriterio(FILE *arquivo, REGISTRO *reg) {
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

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
    
    // Buscar registro sem filtro(todos os não removidos)
    REGISTRO *registroBuscado = NULL;
        
    while((registroBuscado = SELECT_WHERE(arquivo, reg)) != NULL){
        ExibirRegistro(registroBuscado);
        ApagarRegistro(&registroBuscado);
        existeRegistro = true;
    }    

    if(existeRegistro == false) DispararErro(ErroRegistroInexistente());

    return;
}

/*
Função que busca um registro dado um critério,
inicia na posição do ponteiro FILE.
*/
REGISTRO *SELECT_WHERE(FILE *arquivo, REGISTRO *criterio){
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    REGISTRO *registroAtual = NULL;
    do {
        if(registroAtual != NULL) ApagarRegistro(&registroAtual);
        registroAtual = LerRegistro(arquivo);
        // Se retorno nulo, chegou-se ao final do arquivo.
        if(registroAtual == NULL) break;
    } while (!SelecionarPorCriterio(criterio, registroAtual));

    return registroAtual;
}
