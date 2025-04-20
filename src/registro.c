#include "registro.h"
#include "erros.h"
#include "cabecalho.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MSG_VAZIO "NADA CONSTA"

int _LerCampoVariavel(char **campoReg, char *campoString);
bool _EhRegistroValido(REGISTRO* filtro, REGISTRO* atual);

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

void PreencherCampoVariavel(char **campo, char **linha, int *tamVariaveis) {
    if(**linha != ',' && **linha != '\0') {
        int tamString = strlen(*linha);
        char *string = malloc(sizeof(char) * (tamString + 1));
        strcpy(string, *linha);
        string[tamString] = '\0';
        *campo = string;
        *tamVariaveis += tamString + 2;
        *linha += tamString;
    }
    *linha += 1;
}


// Função que dado uma linha csv, preenche registro
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

    // Preencher campos variáveis
    PreencherCampoVariavel(&(*reg)->country, &linha, &tamCamposVariaveis);
    PreencherCampoVariavel(&(*reg)->attackType, &linha, &tamCamposVariaveis);
    PreencherCampoVariavel(&(*reg)->targetIndustry, &linha, &tamCamposVariaveis);
    PreencherCampoVariavel(&(*reg)->defenseMechanism, &linha, &tamCamposVariaveis);

    // Somar o tamanho dos campos das variáveis
    (*reg)->tamanhoRegistro += tamCamposVariaveis;

    return;
}

/* 
Ler um campo variável de string alocando memória
Argumento: destino e origem
Retorno: número de bytes lidos
*/
int _LerCampoVariavel(char **campoReg, char *campoString) {
    // Um campo variável é formado por keyword + valor(string) + delimitador("|")
    campoString++; // Pular keyword
    // Aloca espaço para valor do campo
    int tamValor = strlen(campoString);
    *campoReg = (char*) malloc(sizeof(char)*(tamValor + 1));
    // Preenche valor do campo
    strcpy(*campoReg, campoString);
    // Garantir \0
    (*campoReg)[tamValor] = '\0';
    // Retorna quantidade de bytes lidos
    // (1 byte keyword + tamValor + 1 byte delimitador)
    return (tamValor + 2);
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

    char sep = SEPARADOR;

    // Escrever campos variáveis se existirem
    if(reg->country){
        // Campo: codDescreveCountry + country + "|"  
        fwrite("1", sizeof(char), 1, *arquivo);
        fwrite(reg->country, sizeof(char), strlen(reg->country), *arquivo);
        fwrite(&sep, sizeof(char), 1, *arquivo);
    }

    if(reg->attackType){
        // Campo: codDescreveAttackType + attackType + "|"  
        fwrite("2", sizeof(char), 1, *arquivo);
        fwrite(reg->attackType, sizeof(char), strlen(reg->attackType), *arquivo);
        fwrite(&sep, sizeof(char), 1, *arquivo);
    }

    if(reg->targetIndustry){
        // Campo: codDescreveTargetIndustry + targetIndustry + "|"  
        fwrite("3", sizeof(char), 1, *arquivo);
        fwrite(reg->targetIndustry, sizeof(char), strlen(reg->targetIndustry), *arquivo);
        fwrite(&sep, sizeof(char), 1, *arquivo);
    }

    if(reg->defenseMechanism){
        // Campo: codDescreveDefenseMechanism + defenseMechanism + "|"  
        fwrite("4", sizeof(char), 1, *arquivo);
        fwrite(reg->defenseMechanism, sizeof(char), strlen(reg->defenseMechanism), *arquivo);
        fwrite(&sep, sizeof(char), 1, *arquivo);
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

// Retorna se um registro é valido dado um filtro
bool _EhRegistroValido(REGISTRO* filtro, REGISTRO* atual) {
    if(filtro == NULL) return true;
    if(atual == NULL) return false;
    /* Um registro é valido, se ele tem os filtros passados*/
    // Verifica idAttack
    if (filtro->idAttack != -1 && 
        filtro->idAttack != atual->idAttack) {
        return false;
    }
    
    // Verifica year
    if (filtro->year != -1 && 
        filtro->year != atual->year) {
        return false;
    }
    
    // Verifica financialLoss
    if (filtro->financialLoss != -1 && 
        filtro->financialLoss != atual->financialLoss) {
        return false;
    }
    
    // Verifica strings (country, attackType, etc.)
    if (filtro->country != NULL && 
        (atual->country == NULL || 
            strcmp(filtro->country, atual->country) != 0)) {
        return false;
    }
    
    if (filtro->attackType != NULL && 
        (atual->attackType == NULL || 
            strcmp(filtro->attackType, atual->attackType) != 0)) {
        return false;
    }
    
    if (filtro->targetIndustry != NULL && 
        (atual->targetIndustry == NULL || 
            strcmp(filtro->targetIndustry, atual->targetIndustry) != 0)) {
        return false;
    }
    
    if (filtro->defenseMechanism != NULL && 
        (atual->defenseMechanism == NULL || 
            strcmp(filtro->defenseMechanism, atual->defenseMechanism) != 0)) {
        return false;
    }
    
    // Se todas as verificações passaram
    return true;
}

/*
Função para imprimir registros filtrados pelos parametros
*/
void BuscaRegistroPorCampo(FILE *arquivo, REGISTRO *reg) {
    // Se o arquivo de entrada não existir
    if(arquivo == NULL){
        // Dispara erro fatal.
        DispararErro(ErroPonteiroInvalido());
    }

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivo, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivo);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }

    // Atualizar ponteiro do arquivo para o início dos registros
    fseek(arquivo, TAM_HEAD, SEEK_SET);
    
    // Existe registro a ser buscado?
    bool registroEncontrado = false;
    // Percorrer arquivo
    while(fread(&byteAtual, sizeof(char), 1, arquivo)){
        if(byteAtual == REMOVIDO){
            // Se registro removido, pular para o próximo registro
            int tamRegistro;
            fread(&tamRegistro, sizeof(int), 1, arquivo);
            fseek(arquivo, tamRegistro, SEEK_CUR);
        } else {
            REGISTRO *regAtual = LerRegistro(arquivo);
            bool ehRegistroValido = _EhRegistroValido(reg, regAtual);

            if(ehRegistroValido){
                registroEncontrado = true;
                ExibirRegistro(regAtual);
            }
            ApagarRegistro(&regAtual);
        }
    }

    if(registroEncontrado == false){
        DispararErro(ErroRegistroInexistente());
    }

    printf("**********\n");


    return;
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

void BuscaTodosOsRegistros(FILE *arquivo){
    // Se o arquivo de entrada não existir
    if(arquivo == NULL){
        // Dispara erro fatal.
        DispararErro(ErroPonteiroInvalido());
    }

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivo, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivo);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }

    // Atualizar ponteiro do arquivo para o início dos registros
    fseek(arquivo, TAM_HEAD, SEEK_SET);
    
    // Existe registro a ser buscado?
    bool existeRegistro = false;
    // Percorrer arquivo
    while(fread(&byteAtual, sizeof(char), 1, arquivo)){
        if(byteAtual == REMOVIDO){
            // Se registro removido, pular para o próximo registro
            int tamRegistro;
            fread(&tamRegistro, sizeof(int), 1, arquivo);
            fseek(arquivo, tamRegistro, SEEK_CUR);
        } else {
            existeRegistro = true;
            REGISTRO *regAtual = LerRegistro(arquivo);
            if(regAtual) ExibirRegistro(regAtual);
            else DispararErro(ErroAoLerRegistro());
            ApagarRegistro(&regAtual);
        }
    }

    if(existeRegistro == false){
        DispararErro(ErroRegistroInexistente());
    }
    return;
}

// Função para ler campos fixos do registro
int LerCamposFixos(FILE *arquivo, REGISTRO *reg) {
    if ((fread(&(reg->tamanhoRegistro), sizeof(int), 1, arquivo)) != 1) return 0;
    if ((fread(&(reg->prox), sizeof(long int), 1, arquivo)) != 1) return 0;
    if ((fread(&(reg->idAttack), sizeof(int), 1, arquivo)) != 1) return 0;
    if ((fread(&(reg->year), sizeof(int), 1, arquivo)) != 1) return 0;
    if ((fread(&(reg->financialLoss), sizeof(float), 1, arquivo)) != 1) return 0;
    return 1;
}

// Função para alocar e copiar string de um campo variável
char* CopiarCampoString(const char *origem) {
    int tam = strlen(origem);
    char *destino = (char*) malloc(sizeof(char) * (tam + 1));
    if (destino) {
        strcpy(destino, origem);
        destino[tam] = '\0';
    }
    return destino;
}

// Função para processar cada campo variável
int ProcessarCampoVariável(char codigo, char *valor, REGISTRO *reg) {
    switch (codigo) {
        case '1': // country
            reg->country = CopiarCampoString(valor);
            return reg->country != NULL;
        case '2': // attackType
            reg->attackType = CopiarCampoString(valor);
            return reg->attackType != NULL;
        case '3': // targetIndustry
            reg->targetIndustry = CopiarCampoString(valor);
            return reg->targetIndustry != NULL;
        case '4': // defenseMechanism
            reg->defenseMechanism = CopiarCampoString(valor);
            return reg->defenseMechanism != NULL;
        default:
            // Código inválido
            return 0;
    }
}

// Função para ler e processar campos variáveis
int LerCamposVariaveis(FILE *arquivo, int tamanhoRestante, REGISTRO *reg) {
    char *buffer = (char*) malloc(tamanhoRestante + 1);
    if (!buffer) return 0;
    
    buffer[tamanhoRestante] = '\0';
    if (fread(buffer, sizeof(char), tamanhoRestante, arquivo) != tamanhoRestante) {
        free(buffer);
        return 0;
    }
    
    // srttok retorna NULL caso não encontre delimitador
    // Processar campos delimitados por |
    char *token = strtok(buffer, "|");
    while (token != NULL) {
        if (strlen(token) < 1) {
            token = strtok(NULL, "|");
            continue;
        }
        
        char codigo = token[0];
        char *valor = token + 1; // Pula o código
        
        if (!ProcessarCampoVariável(codigo, valor, reg)) {
            free(buffer);
            return 0;
        }
        
        token = strtok(NULL, "|");
    }
    
    free(buffer);
    return 1;
}

REGISTRO *LerRegistro(FILE *arquivo) {
    if (arquivo == NULL) {
        DispararErro(ErroPonteiroInvalido());
        return NULL;
    }
    
    REGISTRO *reg = CriarRegistroVazio();
    if (reg == NULL) {
        DispararErro(ErroAlocacaoMemoria());
        return NULL;
    }
    
    // Ler campos fixos
    if (!LerCamposFixos(arquivo, reg)) {
        ApagarRegistro(&reg);
        return NULL;
    }
    
    // Calcular tamanho dos campos variáveis
    int tamRestante = reg->tamanhoRegistro - 20; // 20 bytes dos campos fixos
    
    // Ler campos variáveis se existirem
    if (tamRestante > 0 && !LerCamposVariaveis(arquivo, tamRestante, reg)) {
        ApagarRegistro(&reg);
        return NULL;
    }
    
    return reg;
}