#include "registro.h"

#include "erros.h"
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
        DispararErro(ErroPonteiroInvalido());
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
void PreencherRegistro(REGISTRO **reg, char *buffer){
    if (*reg == NULL || buffer == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

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
        // Converte string para float e atualiza linha(linha += strlen(linha))
        (*reg)->financialLoss = strtof(linha, &linha);
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
    if(*linha != '\0'){
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
    // Somar o tamanho dos campos das variáveis
    (*reg)->tamanhoRegistro += tamCamposVariaveis;

    return;
}

char *LerCampoVariavel(FILE *arquivo) {
    int tam = 0;
    int max = 50;
    char c;

    // Buffer para ler os dados
    char *buffer = (char*) malloc(sizeof(char) * max);
    if (buffer == NULL) {
        return NULL;
    }

    while (fread(&c, sizeof(char), 1, arquivo) == 1 && c != '|') {
        buffer[tam] = c;
        tam++;
       
        // Aumenta o buffer se precisar
        if (tam >= max) {
            max *= 2;

            char *bufferMaior = (char*) malloc(sizeof(char) * max);
            if (bufferMaior == NULL) {
                free(buffer);
                return NULL;
            }

            buffer = bufferMaior;
        }
    }

    // Alocar uma string
    char *str = (char*) malloc(sizeof(char) * tam);
    if (str == NULL) {
        free(buffer);
        return NULL;
    }

    strncpy(str, buffer, tam);

    free(buffer);

    return str;
}

/*Ler registro exceto campo removido*/
REGISTRO *LerRegistro(FILE *arquivo) {
    REGISTRO *reg = (REGISTRO*) malloc(sizeof(REGISTRO));

    if (arquivo == NULL || reg == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    // Ler campos fixos(exceto byte removido)
    fread(&(reg->tamanhoRegistro), sizeof(int), 1, arquivo);
    fread(&(reg->prox), sizeof(long int), 1, arquivo);
    fread(&(reg->idAttack), sizeof(int), 1, arquivo);
    fread(&(reg->year), sizeof(int), 1, arquivo);
    fread(&(reg->financialLoss), sizeof(float), 1, arquivo);

    // Ler country
    reg->country = LerCampoVariavel(arquivo);

    // Ler attackType
    reg->attackType = LerCampoVariavel(arquivo);

    // Ler targetIndsutry
    reg->targetIndustry = LerCampoVariavel(arquivo);

    // Ler defenseMechanism
    reg->defenseMechanism = LerCampoVariavel(arquivo);

    return reg;
}

/* 
Função que escreve um registro em um arquivo binário
*/
void EscreverRegistro(FILE **arquivo, REGISTRO *reg){
    if (*arquivo == NULL || reg == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    // Escrever campos fixos
    fwrite(&(reg->removido), sizeof(char), 1, *arquivo);
    fwrite(&(reg->tamanhoRegistro), sizeof(int), 1, *arquivo);
    fwrite(&(reg->prox), sizeof(long int), 1, *arquivo);
    fwrite(&(reg->idAttack), sizeof(int), 1, *arquivo);
    fwrite(&(reg->year), sizeof(int), 1, *arquivo);
    fwrite(&(reg->financialLoss), sizeof(float), 1, *arquivo);

    // Escrever campos variáveis se existirem
    if(reg->country){
        // Campo: codDescreveCountry + country + "|"  
        fwrite("1", sizeof(char), 1, *arquivo);
        fwrite(reg->country, sizeof(char), strlen(reg->country), *arquivo);
        fwrite("|", sizeof(char), 1, *arquivo);
    }

    if(reg->attackType){
        // Campo: codDescreveAttackType + attackType + "|"  
        fwrite("2", sizeof(char), 1, *arquivo);
        fwrite(reg->attackType, sizeof(char), strlen(reg->attackType), *arquivo);
        fwrite("|", sizeof(char), 1, *arquivo);
    }

    if(reg->targetIndustry){
        // Campo: codDescreveTargetIndustry + targetIndustry + "|"  
        fwrite("3", sizeof(char), 1, *arquivo);
        fwrite(reg->targetIndustry, sizeof(char), strlen(reg->targetIndustry), *arquivo);
        fwrite("|", sizeof(char), 1, *arquivo);
    }

    if(reg->defenseMechanism){
        // Campo: codDescreveDefenseMechanism + defenseMechanism + "|"  
        fwrite("4", sizeof(char), 1, *arquivo);
        fwrite(reg->defenseMechanism, sizeof(char), strlen(reg->defenseMechanism), *arquivo);
        fwrite("|", sizeof(char), 1, *arquivo);
    }

    return;
}

/*
Exibe os campos do regsitro, cuidando de valores nulos/vazios.
*/
void ExibirRegistro(REGISTRO *reg) {
    CABECALHO *c = CriarCabecalhoPadrao();

    if (reg == NULL || c == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    // Campo idAttack
    if (reg->idAttack == -1) {
        printf("%s: %s\n", c->descreveIdentificador, MSG_VAZIO);
    } else {
        printf("%s: %d\n", c->descreveIdentificador, reg->idAttack);
    }

    // Campo year
    if (reg->year == -1) {
        printf("%s: %s\n", c->descreveYear, MSG_VAZIO);
    } else { 
        printf("%s: %d\n", c->descreveYear, reg->year);
    }

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
    if (reg->financialLoss == -1) {
        printf("%s: %s\n", c->descreveFinancialLoss, MSG_VAZIO);
    } else {
        printf("%s: %.2f\n", c->descreveFinancialLoss, reg->financialLoss);
    }

    // Campo defenseMechanism
    printf("%s: %s\n", c->descreveDefense,
                        reg->defenseMechanism == NULL ? MSG_VAZIO : reg->defenseMechanism);

    ApagarCabecalho(&c);

    // Pular linha
    printf("\n");
}

/*
Função para imprimir registros filtrados pelos parametros
*/
bool BuscaRegistroPorParametro(FILE *arquivo, REGISTRO *reg) {
    // Se o arquivo de entrada não existir
    if(arquivo == NULL){
        // Dispara erro fatal.
        DispararErro(ErroPonteiroInvalido());
    }

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivo, 0, SEEK_SET);
    
    char byteAtual;
    // Se o arquivo for não consistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivo);
    if(byteAtual == '0') return false;

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivo, 276, SEEK_SET);
    
    // Percorrer arquivo
    while(fread(&byteAtual, sizeof(char), 1, arquivo)){
        if(byteAtual == '1'){
            // Se arquivo removido, pular para o próximo registro
            int tamRegistro;
            fread(&tamRegistro, sizeof(int), 4, arquivo);
            fseek(arquivo, tamRegistro, SEEK_CUR);
        } else {
            REGISTRO *regAtual = LerRegistro(arquivo);
            bool ehRegistroValido = true;
            // TODO: verificações registros
            /*
            Ideia: se existe em reg, deve ser verificado
            Se valor for diferente, não deve ser impresso
            */
            for(int i = 0; i < 1; i++){
                if(reg->idAttack != -1){
                    if(reg->idAttack != regAtual->idAttack){
                        ehRegistroValido = false;
                        break;
                    } 
                }
            }

            if(ehRegistroValido){
                ExibirRegistro(regAtual);
                printf("\n**********\n");
            }
        }
    }

    return true;
}

void ApagarRegistro(REGISTRO **reg) {
    if (*reg == NULL) {
       DispararErro(ErroPonteiroInvalido());
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

    // Desalocar registro todo
    free(*reg);
    *reg = NULL;
}
