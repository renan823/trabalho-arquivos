#include "delete.h"
#include "erros.h"
#include "buscar.h"
#include "update.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void UPDATE(FILE *arquivo, REGISTRO *criterio, REGISTRO *valoresAtualizados){
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
        (registroBuscado = SELECT_WHERE(arquivo, criterio)) != NULL) {
        int espacoDisponivel = registroBuscado->tamanhoRegistro;
        AtualizarRegistro(registroBuscado, valoresAtualizados);
        fseek(arquivo, -(registroBuscado->tamanhoRegistro + 5), SEEK_CUR);
        if(espacoDisponivel >= registroBuscado->tamanhoRegistro){
          EscreverRegistro(&arquivo, registroBuscado);
        } else {
          long int byteAtual = ftell(arquivo);
          RemoverRegistro(arquivo, c, registroBuscado);
          // TO-DO: Inserir registro.
          fseek(arquivo, byteAtual, SEEK_SET);
        }
        ApagarRegistro(&registroBuscado);
    }    

    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);

    return;
}