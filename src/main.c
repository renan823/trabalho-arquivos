#include "cabecalho.h"
#include "registro.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
Função que le registros em um CSV e armazena em um arquivo binário
*/
void FUNCIONALIDADE1(){
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

int main(void) {
    // Ler funcionalidade selecionada e nome arquivo de entrada
    int funcionalidade;
    scanf("%d", &funcionalidade);

    switch (funcionalidade){
        case 1:
            FUNCIONALIDADE1();
            break;
        case 2:
            // TODO: SELECT 
            break;
        case 3:
            // TODO: WHERE
            break;

        default:
            break;
    }   
    return 0;
}