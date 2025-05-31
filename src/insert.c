#include "insert.h"

#include "registro.h"
#include "erros.h"

#include <stdio.h>

void INSERT(FILE *arquivo, REGISTRO *reg) {
    // Se arquivo nulo, encerra execução.
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    CABECALHO *c = LerCabecalho(&arquivo);
    if(c->status == INCONSISTENTE) {
        ApagarCabecalho(&c);
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    // Estrategia first fit
    // Lixo -> $

    // Caso 1 - nenhum registro removido ou sem fit -> inserir fim
    // Caso 2 - fit bem sucedido -> inserir no espaço e atualizar lista

    if (c->topo == -1)


    ExibirRegistro(reg);
}