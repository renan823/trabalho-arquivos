#include "delete.h"
#include "erros.h"
#include "buscar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void DELETE(FILE *arquivo, REGISTRO *reg) {
    // Se arquivo nulo, encerra execução.
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    CABECALHO *c = LerCabecalho(&arquivo);
    if(c->status == INCONSISTENTE) {
        ApagarCabecalho(&c);
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    // Buscar registro sem filtro(todos os não removidos)
    REGISTRO *registroBuscado = NULL;
    while (c->nroRegArq != 0 &&
        (registroBuscado = SELECT_WHERE(arquivo, reg)) != NULL) {
        fseek(arquivo, -(registroBuscado->tamanhoRegistro + 5), SEEK_CUR);
        RemoverRegistro(arquivo, c, registroBuscado);
        c->nroRegArq--;
        c->nroRegRem++;
        ApagarRegistro(&registroBuscado);
    }    

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