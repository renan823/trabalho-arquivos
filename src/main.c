#include "cabecalho.h"
#include "registro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Retorna tamanho em bytes da linha a ser lida(0 caso chegue no final)
char *PegarLinha(char **buffer, int tamBuffer,FILE *arquivoAberto){
    int byteOffset = 0;
    char byteAtual;

    // Retorna 1 se conseguiu ler o caracter, se não conseguiu, chegou-se ao final do arquivo.
    while(1 == fread(&byteAtual, sizeof(char), 1, arquivoAberto)){

        // Verifica se o tamanho atual do buffer é suficiente
        if(byteOffset >= tamBuffer){
            // Aumenta tamanho do buffer em 50
            tamBuffer = tamBuffer + 50;
            (*buffer) = (char*) realloc((*buffer), tamBuffer);
        }
        // Não armazenar esses caracteres no buffer
        if(byteAtual != '\r' && byteAtual != '\n'){
            // Byte atual no buffer
            (*buffer)[byteOffset] = byteAtual;
            byteOffset++;
        }

        // Arquivo CSV tem conteúdo encerrado em \n ou \r
        if(byteAtual == '\n') break;
    }
    (*buffer)[byteOffset] = '\0';

    // Retorna nulo se não foi possível ler nada(EOF).
    return byteOffset == 0 ? NULL : (*buffer);
}

// Função lê uma string sem desperdício de memória
char *LerString(void){
    char buffer[256];
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
    
    return;
}

int main(void) {
    FILE *arquivo = fopen("teste.bin", "rb");

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

    fclose(arquivo);

    return 0;
}