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


void PreencherRegistro(REGISTRO **reg,char *buffer){
    /*
    Manejo strok() - tirado site da IBM
    conteudo = strtok(linha do csv, delimitador).
    OBS: a cada chamada substitui o delimitador por um caracter nulo.
    Exemplo de execução de 3 strtoks:
    buffer("juan,20,santos") -> buffer("juan\020\0santos\0")
    Para chamadas em que a linha é nula,
    continua da onde parou no argumento não nulo mais recente.
    OBS: se o conteúdo é nulo(",,"), ele pula para o próximo.
    Caso não ache o delimitador(exemplo no final), outro caracter 
    de parada é o '\0'.
    OBS: ele ignora conteudo vazios e busca conteudos.
    */
    // Sumir com os delimitadores
    strtok(buffer, ",");
    while(strtok(NULL, ","));

    // Var. aux que ajuda a verificar campo vazio no buffer
    char *linha = buffer;
    
    // Campo 1: id do ataque(int)
    if(*linha != ','){
        // Armazenar id como inteiro
        (*reg)->idAttack = atoi(linha);
        // Andar tamanho do conteúdo
        linha += strlen(linha);
    }
    // Pula '\0' ou ','
    linha += 1;

    // Campo 2: year(int)
    if(*linha != ','){
        (*reg)->year = atoi(linha);
        // Andar tamanho do conteúdo como string
        linha += strlen(linha);
    }
    // Pula '\0' ou ','
    linha += 1;

    // Campo 3: financialLoss(float)
    if(*linha != ','){
        (*reg)->financialLoss = atof(linha);
        // Andar tamanho do conteúdo como string
        linha += strlen(linha);
    }
    // Pula '\0' ou ','
    linha += 1;

    // Armazenar tamanho dos campos variaveis
    int tamCamposVariaveis = 0;

    // Campo 4: country(strlen(country))
    if(*linha != ','){
        // Guarda conteúdo registro 
        int tamString = strlen(linha);
        // Aloca espaço string + '\0'
        char *string = (char*) malloc(sizeof(char)*(tamString + 1));
        strcpy(string, linha);
        // Garante \0
        string[tamString] = '\0';
        (*reg)->country = string; // Salva no registro.
        // Add  ao tamanho do registro
        tamCamposVariaveis += tamString;
        // Andar tamanho do conteúdo como string
        linha += tamString;
    }
    // Pula '\0' ou ','
    linha += 1;
    
    // Campo 5: attackType(strlen(attackType))
    if(*linha != ','){
        // Guarda conteúdo registro 
        int tamString = strlen(linha);
        // Aloca espaço string + '\0'
        char *string = (char*) malloc(sizeof(char)*(tamString + 1));
        strcpy(string, linha);
        // Garante \0
        string[tamString] = '\0';
        (*reg)->attackType = string; // Salva no registro.
        // Add  ao tamanho do registro
        tamCamposVariaveis  += tamString;
        // Andar tamanho do conteúdo como string
        linha += tamString;
    }
    // Pula '\0' ou ','
    linha += 1;

    // Campo 6: targetIndustry(strlen(targetIndustry))
    if(*linha != ','){
        // Guarda conteúdo registro 
        int tamString = strlen(linha);
        // Aloca espaço string + '\0'
        char *string = (char*) malloc(sizeof(char)*(tamString + 1));
        strcpy(string, linha);
        // Garante \0
        string[tamString] = '\0';
        (*reg)->targetIndustry = string; // Salva no registro.
        // Add  ao tamanho do registro
        tamCamposVariaveis += tamString;
        // Andar tamanho do conteúdo como string
        linha += tamString;
    }
    // Pula '\0' ou ','
    linha += 1;

    // Campo 7: defenseMechanism(strlen(defenseMechanism))
    if(*linha != ','){
        // Guarda conteúdo registro 
        int tamString = strlen(linha);
        // Aloca espaço string + '\0'
        char *string = (char*) malloc(sizeof(char)*(tamString + 1));
        strcpy(string, linha);
        // Garante \0
        string[tamString] = '\0';
        (*reg)->defenseMechanism = string; // Salva no registro.
        // Add  ao tamanho do registro
        tamCamposVariaveis += tamString;
        // Andar tamanho do conteúdo como string
        linha += tamString;
    }

    ExibirRegistro(*reg);

    return;
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

    // Buffer para armazenar linha do CSV
    int tamBuffer = 256;
    char *buffer = (char*) malloc(sizeof(char)*tamBuffer);

    // Eliminar introdução do arquivo CSV
    PegarLinha(&buffer, tamBuffer, arquivoEntrada);

    // Retorna tamanho da linha e não altera o ponteiro.
    // Se o tamanho da linha for zero, não há dados a serem escritos mais(fim do arquivo).
    while(PegarLinha(&buffer, tamBuffer, arquivoEntrada)){
        REGISTRO *reg = CriarRegistroVazio();
        PreencherRegistro(&reg, buffer);
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