#include "delete.h"
#include "erros.h"
#include "buscar.h"
#include "update.h"
#include "insert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void UPDATE(FILE *arquivo, CRITERIO *criterio, CRITERIO *valoresAtualizados){
    // Se arquivo nulo, encerra execução.
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    CABECALHO *c = LerCabecalho(&arquivo);
    if(c->status == INCONSISTENTE) {
        ApagarCabecalho(&c);
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    // Buscar registro sem filtro(todos os não removidos)
    REGISTRO *regBuscado = NULL;
    while (c->nroRegArq != 0 &&
        (regBuscado = SELECT_WHERE(arquivo, criterio)) != NULL) {
    
        // Atualizar registro encontrado, guardando o tamanho do desse registro.
        int espacoDisponivel = regBuscado->tamanhoRegistro;
        REGISTRO *regAtualizado = AtualizarRegistro(regBuscado, valoresAtualizados);
    
        // Retornar file pointer para inicio desse espaço.
        fseek(arquivo, -(regBuscado->tamanhoRegistro + 5), SEEK_CUR);
        
        // Se após atualizar dados, verificar se cabe no espaço.
        if(espacoDisponivel >= regAtualizado->tamanhoRegistro){
            EscreverRegistro(&arquivo, regBuscado);
        } else {
            // Procurar nova posição para inserir, salvando aonde estava.
            long int byteAtual = ftell(arquivo);
            // Remover registro buscado
            RemoverRegistro(arquivo, c, regBuscado);
            // Inserir registro atualizado
            INSERT(arquivo, regAtualizado);
            fseek(arquivo, byteAtual, SEEK_SET);
        }

        ApagarRegistro(&regBuscado);
        ApagarRegistro(&regAtualizado);
    }    

    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);

    return;
}