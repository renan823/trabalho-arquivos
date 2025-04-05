#include "registro.h"

#include "cabecalho.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MSG_VAZIO "NADA CONSTA"

/*
Aloca memória para um registro.
Dispara erro em caso de ponteiro NULL
*/
REGISTRO *CriarRegistroVazio(void) {
    REGISTRO *reg = (REGISTRO*) malloc(sizeof(REGISTRO));

    if (reg == NULL) {
        //erro!
    } 
    reg->removido = '0';
    // Tamanho fixo do registro
    reg->tamanhoRegistro = 20;
    // Inicializar campos fixos com -1.
    reg->prox = -1;
    reg->idAttack = -1;
    reg->year = -1;
    reg->financialLoss = -1;
    // Campos variaveis setar para NULL
    reg->country = NULL; 
    reg->attackType = NULL; 
    reg->targetIndustry = NULL; 
    reg->defenseMechanism = NULL;

    return reg;
}

/*
Função que dado uma linha csv, preenche registro
*/
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
        // Add string ao registro junto ao seu código e delimitador 
        tamCamposVariaveis += tamString + 2;
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
        // Add string ao registro junto ao seu código e delimitador 
        tamCamposVariaveis += tamString + 2;
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
        // Add string ao registro junto ao seu código e delimitador 
        tamCamposVariaveis += tamString + 2;
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
        // Add string ao registro junto ao seu código e delimitador 
        tamCamposVariaveis += tamString + 2;
        // Andar tamanho do conteúdo como string
        linha += tamString;
    }

    return;
}

/* 
Função que escreve um registro em um arquivo binário
*/
void EscreverRegistro(FILE **arquivoSaida,REGISTRO *reg){
    // Escrever campos fixos
    fwrite(&(reg->removido), sizeof(char), 1, *arquivoSaida);
    fwrite(&(reg->tamanhoRegistro), sizeof(int), 1, *arquivoSaida);
    fwrite(&(reg->prox), sizeof(long int), 1, *arquivoSaida);
    fwrite(&(reg->idAttack), sizeof(int), 1, *arquivoSaida);
    fwrite(&(reg->year), sizeof(int), 1, *arquivoSaida);
    fwrite(&(reg->financialLoss), sizeof(float), 1, *arquivoSaida);
    // Escrever campos variáveis se existirem
    if(reg->country){
        // Campo: codDescreveCountry + country + "|"  
        fwrite("1", sizeof(char), 1, *arquivoSaida);
        fwrite(reg->country, sizeof(char), strlen(reg->country), *arquivoSaida);
        fwrite("|", sizeof(char), 1, *arquivoSaida);
    }
    if(reg->attackType){
        // Campo: codDescreveAttackType + attackType + "|"  
        fwrite("2", sizeof(char), 1, *arquivoSaida);
        fwrite(reg->attackType, sizeof(char), strlen(reg->attackType), *arquivoSaida);
        fwrite("|", sizeof(char), 1, *arquivoSaida);
    }
    if(reg->targetIndustry){
        // Campo: codDescreveTargetIndustry + targetIndustry + "|"  
        fwrite("3", sizeof(char), 1, *arquivoSaida);
        fwrite(reg->targetIndustry, sizeof(char), strlen(reg->targetIndustry), *arquivoSaida);
        fwrite("|", sizeof(char), 1, *arquivoSaida);
    }
    if(reg->defenseMechanism){
        // Campo: codDescreveDefenseMechanism + defenseMechanism + "|"  
        fwrite("4", sizeof(char), 1, *arquivoSaida);
        fwrite(reg->defenseMechanism, sizeof(char), strlen(reg->defenseMechanism), *arquivoSaida);
        fwrite("|", sizeof(char), 1, *arquivoSaida);
    }

    return;
}

/*
Exibe os campos do regsitro, cuidando de valores nulos/vazios.
*/
void ExibirRegistro(REGISTRO *reg) {
    CABECALHO *c = CriarCabecalhoPadrao();
    if (reg == NULL || c == NULL) {
        // erro!
        return;
    }

    // Campo idAttack
    if (reg->idAttack == -1) printf("%s: %s\n", c->descreveIdentificador, MSG_VAZIO);
    else printf("%s: %d\n", c->descreveIdentificador, reg->idAttack);
    // Campo year
    if (reg->year == -1) printf("%s: %s\n", c->descreveYear, MSG_VAZIO);
    else printf("%s: %d\n", c->descreveYear, reg->year);
    // Campo country
    printf("%s: %s\n", c->descreveCountry, 
        reg->country == NULL ? MSG_VAZIO : reg->country);
    // Campo targetIndustry
    printf("%s: %s\n", c->descreveTargetIndustry, 
                        reg->targetIndustry == NULL ? MSG_VAZIO:reg->targetIndustry);
    // Campo attackType
    printf("%s: %s\n", c->descreveType,
                        reg->attackType == NULL ? MSG_VAZIO : reg->attackType);
    // Campo financialLoss
    if (reg->financialLoss == -1) printf("%s: %s\n", c->descreveFinancialLoss, MSG_VAZIO);
    else printf("%s: %.2f\n", c->descreveFinancialLoss, reg->financialLoss);
    // Campo defenseMechanism
    printf("%s: %s\n", c->descreveDefense,
                        reg->defenseMechanism == NULL ? MSG_VAZIO : reg->defenseMechanism);

    ApagarCabecalho(&c);

    // Pular linha
    printf("\n");
}

void ApagarRegistro(REGISTRO **reg) {
    if (*reg == NULL) {
        //erro!
    } 

    // Desalocar campos variáveis
    if ((*reg)->country != NULL) {
        free((*reg)->country);
        (*reg)->country = NULL;
    }; 
    if ((*reg)->attackType != NULL) {
        free((*reg)->attackType);
        (*reg)->attackType = NULL;
    }; 
    if ((*reg)->targetIndustry != NULL) {
        free((*reg)->targetIndustry);
        (*reg)->targetIndustry = NULL;
    }; 
    if ((*reg)->defenseMechanism != NULL) {
        free((*reg)->defenseMechanism);
        (*reg)->defenseMechanism = NULL;
    }; 

    return;
}
