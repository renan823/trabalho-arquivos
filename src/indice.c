#include "indice.h"

#include "arvoreb.h"
#include "erros.h"
#include "utils.h"
#include "cabecalho.h"
#include "SQL.h"
#include "criterio.h"
#include "fila.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
Retorna o offset do nó a partir do inicio do arquivo.
Pula o cabeçalho.
*/
long OffsetNo(int rrn) {
    return ((rrn + 1) * TAM_REG_IND);
}

void CriarArquivoIndice(FILE *arquivoEntrada, FILE *arquivoSaida) {
    if(arquivoEntrada == NULL || arquivoSaida == NULL){
        DispararErro(ErroPonteiroInvalido());
    }

    // Criar Árvore B
    ARVB *arvb = CriarArvoreB(arquivoSaida);

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivoEntrada, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivoEntrada);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }

    // Atualizar ponteiro do arquivo para o início dos registros
    fseek(arquivoEntrada, TAM_HEAD, SEEK_SET);

    // Buscar registro sem filtro(todos os não removidos)
    REGISTRO *reg = NULL;

    while((reg = SELECT_WHERE(arquivoEntrada, NULL)) != NULL){
        long int offset = ftell(arquivoEntrada) - (reg->tamanhoRegistro + 5);
        InserirArvoreB(arvb, reg->idAttack, offset);
        ApagarRegistro(&reg);
    }    
    
    // Apagar Árvore B
    ApagarArvoreB(&arvb);

    return; 
}

void ExibirRegistroDadoIndice(FILE *arquivoDados, FILE *arquivoIndices, int indice){
    if(arquivoDados == NULL || arquivoIndices == NULL){
        DispararErro(ErroPonteiroInvalido());
    }

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivoDados, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivoDados);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }
    
    // Atualizar ponteiro do arquivo para o início
    fseek(arquivoIndices, 0, SEEK_SET);

    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivoIndices);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }
    
    // Criar Árvore B
    ARVB *arvb = CriarArvoreB(arquivoIndices);

    // Buscar indice pedido.
    long int offset = BuscarArvoreB(arvb, indice);

    if(offset == -1) {
        DispararErro(ErroRegistroInexistente());
    } else {
        // Atualizar ponteiro do arquivo para o início dos registros
        fseek(arquivoDados, offset, SEEK_SET);

        // Buscar registro sem filtro(todos os não removidos)
        REGISTRO *reg = LerRegistro(arquivoDados);
        ExibirRegistro(reg);
        ApagarRegistro(&reg);
    }
    
    // Apagar Árvore B
    ApagarArvoreB(&arvb);

    return; 
}

void InserirRegistroIndice(FILE *arquivoDados, FILE *arquivoIndices, REGISTRO *reg) {
    // Se arquivo nulo, encerra execução.
    if(arquivoDados == NULL) DispararErro(ErroArquivoInvalido());

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivoIndices, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivoIndices);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }

    CABECALHO *c = LerCabecalho(&arquivoDados);
    if(c->status == INCONSISTENTE) {
        ApagarCabecalho(&c);
        DispararErro(ErroProcessamentoArquivo());
        return;
    }
    // Marcar arquivo de dados para 
    // inconsistente durante manipulação
    c->status = INCONSISTENTE;
    EscreverCabecalho(&arquivoDados, c);

    INSERT(arquivoDados, c, reg);

    ARVB *arvb = CriarArvoreB(arquivoIndices);

    long int offset = ftell(arquivoDados) - (reg->tamanhoRegistro + 5);
    c->status = CONSISTENTE;
    EscreverCabecalho(&arquivoDados, c);
    ApagarCabecalho(&c);

    InserirArvoreB(arvb, reg->idAttack, offset);
    ApagarArvoreB(&arvb);
   
    return;   
}

/* 
Dado registro de dados e indices, 
busca offset do indice passado o qual 
será usado para atualizar registro.
*/
void AtualizarRegistroDadoIndice(FILE *arquivoDados,
                                FILE *arquivoIndices,
                                int indice, 
                                CRITERIO *valoresAtualizados)
{
    // Se arquivo nulo, encerra execução.
    if(arquivoDados == NULL || arquivoIndices == NULL){
        DispararErro(ErroArquivoInvalido());
    } 

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivoDados, 0, SEEK_SET);
    // Atualizar ponteiro do arquivo para o início
    fseek(arquivoIndices, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivoIndices);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivoDados);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }

    // Criar árvore.
    ARVB *arvb = CriarArvoreB(arquivoIndices);

    // Encontrar offset do registro a ser atualizado
    long int offset = BuscarArvoreB(arvb, indice);
    
    if(offset != -1) {
        // Ler cabeçalho
        CABECALHO *c = LerCabecalho(&arquivoDados);
        
        // Marcar arquivo de dados para 
        // inconsistente durante manipulação
        c->status = INCONSISTENTE;
        EscreverCabecalho(&arquivoDados, c);

        // Ler e atualizar registro
        fseek(arquivoDados, offset, SEEK_SET);
        REGISTRO *reg = LerRegistro(arquivoDados);
        offset = UPDATE(arquivoDados, c, valoresAtualizados, reg); 
        
        // Verificar se houve mudança do offset
        if(offset != -1) AtualizarOffsetArvoreB(arvb, reg->idAttack, offset);

        // Salvar dado atualizados
        c->status = CONSISTENTE;
        EscreverCabecalho(&arquivoDados, c);

        // Liberar memória alocada
        ApagarRegistro(&reg);
        ApagarCabecalho(&c);    
    }
    
    ApagarArvoreB(&arvb);

    return;
}

FILA *RertornaIndicesDadoCriterio(FILE *arquivoDados,
                                FILE *arquivoIndices,
                                CRITERIO *criterio)
{
    // Se arquivo nulo, encerra execução.
    if(arquivoDados == NULL || arquivoIndices == NULL){
        DispararErro(ErroArquivoInvalido());
    } 

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivoDados, 0, SEEK_SET);
    // Atualizar ponteiro do arquivo para o início
    fseek(arquivoIndices, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivoIndices);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return NULL; 
    }
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivoDados);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return NULL; 
    }

    // Prepara parâmetros para DFS
    ARVB *arvb = CriarArvoreB(arquivoIndices);
    int *visitados = (int*) calloc(NroNosArvoreB(arvb), sizeof(int));
    FILA *fila = fila_criar();
    BuscaEmProfundidade(arvb,
                        NoRaizArvoreB(arvb),
                        arquivoDados,
                        criterio,
                        visitados,
                        fila);

    // Liberar memória
    ApagarArvoreB(&arvb);
    free(visitados);
    visitados = NULL;

    // Retorna fila de indices.
    return fila;
}





   

