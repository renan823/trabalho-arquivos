#include "insert.h"

#include "registro.h"
#include "erros.h"

#include <stdio.h>
#include <stdbool.h>

void INSERT(FILE *arquivo, REGISTRO *reg) {
    // Se arquivo nulo, encerra execução.
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    CABECALHO *c = LerCabecalho(&arquivo);
    if(c->status == INCONSISTENTE) {
        ApagarCabecalho(&c);
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    // Byteoffset do campo Prox do registro Anterior 
    // da lista de registros removidos.
    long int campoProxAnterior = -1; // -1: não há.
    long int succRegRem = c->topo; // Proximo Reg. Removido.
    long int byteInsercao = -1; // Local de inserção do registro.

    while(succRegRem != -1) 
    {
        // Iniciar leitura a partir do campo tamanho.(Pula campo removido)
        fseek(arquivo, succRegRem + sizeof(char), SEEK_SET);
        int tamDisponivel;
        fread(&tamDisponivel, sizeof(int), 1, arquivo); 
        long int regAtual = succRegRem; 
        fread(&succRegRem, sizeof(long int), 1, arquivo);  
        
        if(tamDisponivel >= reg->tamanhoRegistro)
        {
            // Caso: Registro a ser removido é o primeiro.
            if (campoProxAnterior == -1) c->topo = succRegRem;
            else {
                // Manter ligamento da lista de removidos
                fseek(arquivo, campoProxAnterior, SEEK_SET);
                fwrite(&succRegRem, sizeof(long int), 1, arquivo);
            }

            // Prevalece tamanho do maior registro.
            reg->tamanhoRegistro = tamDisponivel; 
            byteInsercao = regAtual;
            c->nroRegRem--;
            break; 
        }

        // Salvar campo prox do registro atual.
        campoProxAnterior = regAtual +  sizeof(char) + sizeof(int);
    }

    // Caso: Inserção no fim do arquivo
    if(succRegRem == -1) 
    {
        byteInsercao = c->proxByteOffset;
        c->proxByteOffset += reg->tamanhoRegistro;
    }

    // Inserir registro
    fseek(arquivo, byteInsercao, SEEK_SET);
    EscreverRegistro(&arquivo, reg);

    // Atualizar cabeçalho
    c->nroRegArq++;
    c->status = CONSISTENTE;
    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);

    return;
}
