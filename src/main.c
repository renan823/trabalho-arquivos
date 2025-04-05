#include "cabecalho.h"
#include "registro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Retorna tamanho em bytes da linha a ser lida(0 caso chegue no final)
int TamanhoLinha(FILE *arquivoAberto){
    int tamLinha = 0;
    char byteAtual;
    // Retorna 1 se conseguiu ler o caracter, se não conseguiu, chegou-se ao final do arquivo.
    while(1 == fread(&byteAtual, sizeof(char), 1, arquivoAberto)){
        tamLinha++;
        // Arquivo CSV tem conteúdo encerrado em \n ou \r
        if(byteAtual == '\r' || byteAtual == '\n') break;
    }
    return tamLinha;
}

// Função lê uma string sem desperdício de memória
char *LerString(void){
    char buffer[100];
    char *stringDinamica;

    scanf(" %s", buffer);
    // Retorna tamanho da string sem o \0
    int tamString = strlen(buffer);

    // Alocar espaço na heap para a tamString + \0
    stringDinamica = (char*) malloc(sizeof(char)*(tamString + 1));
    strcpy(stringDinamica, buffer);
    // Garante \0 no final da string
    stringDinamica[tamString] = '\0';

    return stringDinamica;
}

/* 
Função que le registros em um CSV e armazena em um arquivo binário
*/
void LerCsvSalvarBin(){
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

    // Transferir dados de um arquivo para o outro
    int tamBuffer = 100;
    char *buffer = (char*) malloc(sizeof(char)*tamBuffer);
    int tamLinha;

    // Eliminar introdução do arquivo CSV
    tamLinha = TamanhoLinha(arquivoEntrada);
    // Seta ponteiro em cima do \n
    // Ultimo caracter lido foi o \r 
    
    // Retorna tamanho da linha e não altera o ponteiro.
    // Se o tamanho da linha for zero, não há dados a serem escritos mais(fim do arquivo).
    while((tamLinha = TamanhoLinha(arquivoEntrada)) != 0){
        // Verifica se o tamanho atual do buffer é suficiente
        while(tamLinha >= strlen(buffer)){
            free(buffer);
            // Aumenta tamanho do buffer em 50
            tamBuffer = tamBuffer + 50;
            buffer = (char*) malloc(sizeof(char)*tamBuffer);
        }
        // Ler dados do arquivo de entrada no buffer
        fread(buffer, sizeof(char), -tamLinha, arquivoEntrada);
        // TODO: formatar dados para serem salvos em binário
        fwrite(buffer, sizeof(char), tamLinha, stdout);
        // Adicionar função preencher registro passando buffer
        // TODO: escrever no arquivo de saida
        // Adicionar função inserir registro em arquivo binário        
    };

}

int main(void) {
    FILE *arquivo = fopen("teste.bin", "rb");

    // Ler funcionalidade selecionada e nome arquivo de entrada
    int funcionalidade;
    scanf("%d", &funcionalidade);

    switch (funcionalidade){
        case 1:
            LerCsvSalvarBin();
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

    fclose(arquivo);

    return 0;
}