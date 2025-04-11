#include "registro.h"

#include "erros.h"
#include "cabecalho.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MSG_VAZIO "NADA CONSTA"
#define STR_EQUAL(a, b) (a != NULL && b != NULL && strcmp(a, b) == 0)

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
    int tamValorsVariaveis = 0;

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
        tamValorsVariaveis += tamString + 2;
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
        tamValorsVariaveis += tamString + 2;
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
        tamValorsVariaveis += tamString + 2;
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
        tamValorsVariaveis += tamString + 2;
        // Andar tamanho do conteúdo como string
        linha += tamString;
    }
    // Somar o tamanho dos campos das variáveis
    (*reg)->tamanhoRegistro += tamValorsVariaveis;

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

/*Ler registro exceto campo removido*/
REGISTRO *LerRegistro(FILE *arquivo) {
    REGISTRO *reg = (REGISTRO*) malloc(sizeof(REGISTRO));

    if (arquivo == NULL || reg == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    // Ler campos no registro do arquivo
    fread(&(reg->tamanhoRegistro), sizeof(int), 1, arquivo); // 4 bytes
    fread(&(reg->prox), sizeof(long int), 1, arquivo); // 8 bytes
    fread(&(reg->idAttack), sizeof(int), 1, arquivo); // 4 bytes
    fread(&(reg->year), sizeof(int), 1, arquivo); // 4 bytes
    fread(&(reg->financialLoss), sizeof(float), 1, arquivo); // 4 bytes
    // Total de bytes armazenados = 20 bytes(sem contar o primeiro byte)
    int tamRestanteRegistro = (reg->tamanhoRegistro) - 20;
    
    if(tamRestanteRegistro != 0){
        // Espaço para armazenar conteúdo restante do registro
        char *buffer = (char*) malloc(sizeof(char)*tamRestanteRegistro);
        
        // Ler bytes restantes do arquivo
        fread(buffer, sizeof(char), tamRestanteRegistro, arquivo);
        
        char sep = SEPARADOR;
        // Sumir com os delimitadores
        strtok(buffer, &sep);
        while(strtok(NULL, &sep));

        // Variável auxiliar para percorrer buffer guardando campos
        char *camposVar = buffer;
        // Variavel para guardar tamanho lido do campo
        int tamCampo = 0;

        // Preencher todos os campos
        while(tamRestanteRegistro != 0){
            switch (*camposVar)
            {
            // codDescreveCountry
            case '1':
                // Ler campo variavel e atualizar para próximos campos
                tamCampo = _LerCampoVariavel(&(reg->country), camposVar);
                tamRestanteRegistro -= tamCampo;    
                camposVar += tamCampo;
                break;
            // codDescreveAttackType
            case '2':
                // Ler campo variavel e atualizar para próximos campos
                tamCampo = _LerCampoVariavel(&(reg->attackType), camposVar);
                tamRestanteRegistro -= tamCampo;    
                camposVar += tamCampo;
                break;
            // codDescreveTargetIndustry
            case '3':
                // Ler campo variavel e atualizar para próximos campos
                tamCampo = _LerCampoVariavel(&(reg->targetIndustry), camposVar);
                tamRestanteRegistro -= tamCampo;    
                camposVar += tamCampo; 
                break;
            // codDescreveDefense
            case '4':
                // Ler campo variavel e atualizar para próximos campos
                tamCampo = _LerCampoVariavel(&(reg->defenseMechanism), camposVar);
                tamRestanteRegistro -= tamCampo;    
                camposVar += tamCampo;
                break;
            
            default:
                // Erro: Código do campo não encotrado
                break;
            }
        }

        // Liberar memória
        free(buffer);
        buffer = NULL;
    }

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
        (atual->country == NULL || strcmp(filtro->country, atual->country) != 0)) {
        return false;
    }
    
    if (filtro->attackType != NULL && 
        (atual->attackType == NULL || strcmp(filtro->attackType, atual->attackType) != 0)) {
        return false;
    }
    
    if (filtro->targetIndustry != NULL && 
        (atual->targetIndustry == NULL || strcmp(filtro->targetIndustry, atual->targetIndustry) != 0)) {
        return false;
    }
    
    if (filtro->defenseMechanism != NULL && 
        (atual->defenseMechanism == NULL || strcmp(filtro->defenseMechanism, atual->defenseMechanism) != 0)) {
        return false;
    }
    
    // Se todas as verificações passaram
    return true;
}

/*
Função para imprimir registros filtrados pelos parametros
*/
bool BuscaRegistroPorCampo(FILE *arquivo, REGISTRO *reg) {
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
        return true; 
    }

    // Atualizar ponteiro do arquivo para o início dos registros
    fseek(arquivo, 276, SEEK_SET);
    
    // Existe registro a ser buscado?
    bool registroEncontrado = false;
    // Percorrer arquivo
    while(fread(&byteAtual, sizeof(char), 1, arquivo)){
        if(byteAtual == REMOVIDO){
            // Se registro removido, pular para o próximo registro
            int tamRegistro;
            fread(&tamRegistro, sizeof(int), 4, arquivo);
            fseek(arquivo, tamRegistro, SEEK_CUR);
        } else {
            REGISTRO *regAtual = LerRegistro(arquivo);
            bool ehRegistroValido = _EhRegistroValido(reg, regAtual);

            if(ehRegistroValido){
                registroEncontrado = true;
                ExibirRegistro(regAtual);
            }
        }
    }
    printf("**********\n");

    return registroEncontrado;
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
