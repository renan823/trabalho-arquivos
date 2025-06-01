#include "insert.h"

#include "registro.h"
#include "erros.h"

#include <stdio.h>
#include <stdbool.h>

/*
*/
void INSERT(FILE *arquivo, REGISTRO *reg) {
    // Se arquivo nulo, encerra execução.
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    CABECALHO *c = LerCabecalho(&arquivo);
    if(c->status == INCONSISTENTE) {
        ApagarCabecalho(&c);
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    // Caso 1 - nenhum registro removido ou sem fit -> inserir fim
    // Caso 2 - fit bem sucedido -> inserir no espaço e atualizar lista
    long atual = c->topo;
    long proxRegAnterior = 1;
    bool fit = false;

    // Percorrer lista de offsets usando seek
    for (int i = 0; i < c->nroRegRem; i++) {
        fseek(arquivo, atual, SEEK_SET);

        REGISTRO *removido = LerRegistro(arquivo);
        
        int disponivel = removido->tamanhoRegistro;
        long prox = removido->prox; // Próximo registro removido.

        ApagarRegistro(&removido);

        if (disponivel >= reg->tamanhoRegistro) {
            // First fit - usar espaço e adicionar $ no lixo

            // Atualizar prox offset do registro proxRegAnterior
            // Já cobre o caso do aterior ser o topo.
            fseek(arquivo, proxRegAnterior, SEEK_SET);
            fwrite(&prox, sizeof(long int), 1, arquivo);

            // Retorna para o começo do reg atual 
            fseek(arquivo, atual, SEEK_SET);

            reg->tamanhoRegistro = disponivel;
            // Escreve e preenche espaço vago
            EscreverRegistro(&arquivo, reg);
            
            fit = true;
            break;
        }

        proxRegAnterior = atual + 5; // Byte offset id + tam
        atual = prox;
    }

    // Caso não faça first fit, inserir no final;
    if (!fit) {
        fseek(arquivo, 0, SEEK_END);
        EscreverRegistro(&arquivo, reg);
        c->proxByteOffset = ftell(arquivo);
    } else c->nroRegRem--;

    // Ajustar cabeçalho
    c->nroRegArq++;
    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);

    return;
}