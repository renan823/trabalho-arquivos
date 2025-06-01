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

    // Setar cabeçalho INCONSISTENTE
    c->status = INCONSISTENTE;
    EscreverCabecalho(&arquivo, c);

    // Caso 1 - Pode ocupar o espaço em c->topo
    // Caso 2 - Procurar first fit
    // Caso 3 - Inserir no fim do arquivo

    bool fit = false;
    long int atual = c->topo;
    long int anterior = -1;

    // Lidos dos registros deletados
    long int proxOffset = -1;
    int tamDisponivel = 0;

    long int offsetInserir = -1;

    // Caso 1 e 2
    while (atual != -1) {
        // Lê apenas os bytes de interesse, não o registro todo

        fseek(arquivo, atual + 1, SEEK_SET); // Pula o char 'removido'
        
        fread(&tamDisponivel, sizeof(int), 1, arquivo);
        fread(&proxOffset, sizeof(long int), 1, arquivo);

        if (tamDisponivel >= reg->tamanhoRegistro) {
            fit = true;
            offsetInserir = atual; // Inserir na posição atual;

            // Atualizar topo
            if (anterior == -1) {
                c->topo = proxOffset;
            } else {
                fseek(arquivo, anterior + 1 + sizeof(int), SEEK_SET); // Vai até o campo prox do anterior
                fwrite(&proxOffset, sizeof(long), 1, arquivo);
            }

            // Atualizar cabeçalho
            c->nroRegRem--;
            reg->tamanhoRegistro += (tamDisponivel - reg->tamanhoRegistro);

            break;
        }

        anterior = atual;
        atual = proxOffset;
    }

    // Caso 3
    if (!fit) {
        // Indica que a inserção é no fim
        fseek(arquivo, 0, SEEK_END);
        offsetInserir = ftell(arquivo);

        // Atualiza 
        c->proxByteOffset = offsetInserir += reg->tamanhoRegistro;
    }

    // Escreve o registro na posição definida anteriormente
    fseek(arquivo, offsetInserir, SEEK_SET);
    EscreverRegistro(&arquivo, reg);

    // Atualizar cabeçalho
    c->nroRegArq += 1;
    c->status = CONSISTENTE;

    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);
}

/*
// Caso 2 - fit bem sucedido -> inserir no espaço e atualizar lista
    long atual = c->topo;
    long proxRegAnterior = 1;
    bool fit = false;

    // Percorrer lista de offsets usando seek
    for (int i = 0; i < c->nroRegRem; i++) {
        fseek(arquivo, atual, SEEK_SET);

        REGISTRO *removido = LerRegistro(arquivo);

        int disponivel = removido->tamanhoRegistro;
        long prox = removido->prox;

        ApagarRegistro(&removido);

        if (disponivel >= reg->tamanhoRegistro) {
            // First fit - usar espaço e adicionar $ no lixo

            // Atualizar prox offset do registro proxRegAnterior
            // Já cobre o caso do aterior ser o topo.
            fseek(arquivo, proxRegAnterior, SEEK_SET);
            fwrite(&prox, sizeof(long int), 1, arquivo);

            // Retorna para o começo do reg atual 
            fseek(arquivo, atual, SEEK_SET);

            // Escreve e preenche espaço vago
            EscreverRegistro(&arquivo, reg);

            // Atualiza o cabeçalho de removidos
            c->nroRegRem--;
            if (atual == c->topo) {
                c->topo = prox;
            }
            
            fit = true;
            break;
        }

        proxRegAnterior = atual + 5; // Byte offset removido (1) + tam (4)
        atual = prox;
    }

    // Caso não faça first fit, inserir no final;
    if (!fit) {
        fseek(arquivo, c->proxByteOffset, SEEK_SET);
        EscreverRegistro(&arquivo, reg);

        // Como foi inserido no fim, atualiza o offset
        c->proxByteOffset = ftell(arquivo);
    }

    // Ajustar cabeçalho
    c->nroRegArq++;
    c->status = CONSISTENTE;

    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);

*/