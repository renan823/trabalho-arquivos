#include "erros.h"
#include "cabecalho.h"
#include "registro.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
Função que le registros em um CSV e armazena em um arquivo binário
*/
void FUNCIONALIDADE1(void){
    // Ler nome dos arquivos de entrada e saída.
    char *nomeArquivoEntrada = LerString();
    char *nomeArquivoSaida = LerString();
    // Abrir arquivos de entrada e saída
    FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "r");
    FILE *arquivoSaida = fopen(nomeArquivoSaida, "wb");
    
    // Escreve cabecalho em binario no arquivo de saida
    CABECALHO *cabecalho =  CriarCabecalhoPadrao();
    EscreverCabecalho(&arquivoSaida, cabecalho);
    ApagarCabecalho(&cabecalho);

    // Buffer para armazenar linha do CSV
    int tamBuffer = 256;
    char *buffer = (char*) malloc(sizeof(char)*tamBuffer);

    // Eliminar introdução do arquivo CSV
    PegarLinha(&buffer, tamBuffer, arquivoEntrada);

    // Retorna tamanho da linha e não altera o ponteiro.
    // Se o tamanho da linha for zero, não há dados a serem escritos mais(fim do arquivo).
    while(PegarLinha(&buffer, tamBuffer, arquivoEntrada)){
        // Preencher registro passando buffer
        REGISTRO *reg = CriarRegistroVazio();
        PreencherRegistro(&reg, buffer);
        ExibirRegistro(reg);
        // Inserir registro em arquivo binário        
        EscreverRegistro(&arquivoSaida, reg);
        // Apagar registro
        ApagarRegistro(&reg);
    };

    // Liberar memória do buffer
    free(buffer);
    buffer = NULL;

    // Fechar arquivos de entrada e saída
    fclose(arquivoEntrada);
    fclose(arquivoSaida);

    // Executar função fornecida para  
    // mostrar a saída do arquivo ataques.bin
    binarioNaTela(nomeArquivoSaida);
    
    // Liberar memória do nome dos arquivos
    free(nomeArquivoEntrada);
    nomeArquivoEntrada = NULL;
    free(nomeArquivoSaida);
    nomeArquivoSaida = NULL;

    return;
}

/*
Exibe os registros no arquivo de dados informado
*/
void FUNCIONALIDADE2(void) {
    char *nomeArquivoEntrada = LerString();

    // Abrir arquivo para leitura
    FILE *arquivo = fopen(nomeArquivoEntrada, "rb");
    if (arquivo == NULL) {
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    // Executar leitura
    CABECALHO *c = LerCabecalho(&arquivo);

    if (c == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    // Cabeçalho inconsistente não será lido
    if (c->status == INCONSISTENTE) {
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    if (c->nroRegArq == 0) {
        DispararErro(ErroRegistroInexistente());
        return;
    }

    printf("%d\n", c->nroRegArq);

    // Ler dados 
    int total = 0;

    while (total < c->nroRegArq) {
        REGISTRO *reg = LerRegistro(arquivo);

        if (!reg->removido) {
            ExibirRegistro(reg);
        }

        ApagarRegistro(&reg); 

        total++;
    }

    ApagarCabecalho(&c);

    fclose(arquivo);
    free(nomeArquivoEntrada);
}

int main(void) {
    // Ler funcionalidade selecionada e nome arquivo de entrada
    int funcionalidade;
    scanf("%d", &funcionalidade);

    switch (funcionalidade){
        case 1:
            FUNCIONALIDADE1();
            break;
        case 2:
            FUNCIONALIDADE2();
            break;
        case 3:
            // TODO: WHERE
            break;

        default:
            break;
    }   
    return 0;
}