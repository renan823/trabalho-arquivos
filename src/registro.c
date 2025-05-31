#include "registro.h"
#include "erros.h"
#include "cabecalho.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MSG_VAZIO "NADA CONSTA"

// Função que ajusta lixo dos campos variaveis.
void _ajustarLixo(char *campoVariavel){
    for(int i = 0; campoVariavel[i] != '\0'; i++) campoVariavel[i] = '$';
}

// Função para ler campos fixos do registro
int _LerCamposFixos(FILE *arquivo, REGISTRO *reg) {
    if ((fread(&(reg->removido), sizeof(char), 1, arquivo)) != 1) {
        return 0;
    } 
   
    if ((fread(&(reg->tamanhoRegistro), sizeof(int), 1, arquivo)) != 1) {
        return 0;
    } 

    if ((fread(&(reg->prox), sizeof(long int), 1, arquivo)) != 1) {
        return 0;
    }

    if ((fread(&(reg->idAttack), sizeof(int), 1, arquivo)) != 1) {
        return 0;
    }

    if ((fread(&(reg->year), sizeof(int), 1, arquivo)) != 1) {
        return 0;
    }

    if ((fread(&(reg->financialLoss), sizeof(float), 1, arquivo)) != 1) {
        return 0;
    }

    return 1;
}

// Função para alocar e copiar string de um campo variável
char *_CopiarCampoString(const char *origem) {
    int tam = 0;
    // Tomar cuidado para não tratar lixo como parte da string.
    while(origem[tam] != '$' && origem[tam] != '\0') tam++;
    char *destino = (char*) malloc(sizeof(char) * (tam + 1));

    if (destino != NULL) {
        strncpy(destino, origem, tam);
        destino[tam] = '\0';
    }

    return destino;
}

// Função para processar cada campo variável
int _ProcessarCampoVariavel(char codigo, char *valor, REGISTRO *reg) {
    switch (codigo) {
        case '1': // country
            reg->country = _CopiarCampoString(valor);
            return reg->country != NULL;
        case '2': // attackType
            reg->attackType = _CopiarCampoString(valor);
            return reg->attackType != NULL;
        case '3': // targetIndustry
            reg->targetIndustry = _CopiarCampoString(valor);
            return reg->targetIndustry != NULL;
        case '4': // defenseMechanism
            reg->defenseMechanism = _CopiarCampoString(valor);
            return reg->defenseMechanism != NULL;
        default:
            printf("Erro ao processar campo variável.\n");
            return 0;
    }
}

// Função para ler e processar campos variáveis
int _LerCamposVariaveis(FILE *arquivo, int tamanhoRestante, REGISTRO *reg) {
    char *buffer = (char*) malloc((tamanhoRestante + 1)*sizeof(char));
    if (!buffer) {
        printf("Erro ao alocar buffer para ler campo variável.\n");
        DispararErro(ErroAlocacaoMemoria());
        return 0;
    }
    
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
        if(codigo == '$') break;
        char *valor = token + 1; // Pula o código
        
        if (!_ProcessarCampoVariavel(codigo, valor, reg)) {
            free(buffer);
            return 0;
        }
        
        token = strtok(NULL, "|");
    }
    
    free(buffer);
    return 1;
}

/*
Aloca memória para um registro.
Dispara erro em caso de ponteiro NULL
*/
REGISTRO *CriarRegistroVazio(void) {
    REGISTRO *reg = (REGISTRO*) malloc(sizeof(REGISTRO));

    if (reg == NULL) DispararErro(ErroAlocacaoMemoria());

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
Função que escreve um registro em um arquivo binário
*/
void EscreverRegistro(FILE **arquivo, REGISTRO *reg) {
    if (*arquivo == NULL) DispararErro(ErroArquivoInvalido());
    if (reg == NULL) {
        printf("Passado registro nulo para ser escrito.\n");
        DispararErro(ErroPonteiroInvalido());
    }

    // Tamanho dos campos fixos (calculado dinamicamente)
    int tamanhoCamposFixos = sizeof(long int) + sizeof(int) + sizeof(int) + sizeof(float);

    // Escrever campos fixos
    fwrite(&(reg->removido), sizeof(char), 1, *arquivo);
    fwrite(&(reg->tamanhoRegistro), sizeof(int), 1, *arquivo);
    fwrite(&(reg->prox), sizeof(long int), 1, *arquivo);
    fwrite(&(reg->idAttack), sizeof(int), 1, *arquivo);
    fwrite(&(reg->year), sizeof(int), 1, *arquivo);
    fwrite(&(reg->financialLoss), sizeof(float), 1, *arquivo);

    char sep = SEPARADOR;
    int tamRestante = reg->tamanhoRegistro - tamanhoCamposFixos;

    // Escrever campos variáveis se existirem
    if (reg->country && reg->country[0] != '$') {
        fwrite("1", sizeof(char), 1, *arquivo);
        tamRestante -= strlen(reg->country) + 2; 
        fwrite(reg->country, sizeof(char), strlen(reg->country), *arquivo);
        fwrite(&sep, sizeof(char), 1, *arquivo);
    }

    if (reg->attackType && reg->attackType[0] != '$') {
        fwrite("2", sizeof(char), 1, *arquivo);
        tamRestante -= strlen(reg->attackType) + 2;
        fwrite(reg->attackType, sizeof(char), strlen(reg->attackType), *arquivo);
        fwrite(&sep, sizeof(char), 1, *arquivo);
    }

    if (reg->targetIndustry && reg->targetIndustry[0] != '$') {
        fwrite("3", sizeof(char), 1, *arquivo);
        tamRestante -= strlen(reg->targetIndustry) + 2;
        fwrite(reg->targetIndustry, sizeof(char), strlen(reg->targetIndustry), *arquivo);
        fwrite(&sep, sizeof(char), 1, *arquivo);
    }

    if (reg->defenseMechanism && reg->defenseMechanism[0] != '$') {
        fwrite("4", sizeof(char), 1, *arquivo);
        tamRestante -= strlen(reg->defenseMechanism) + 2;
        fwrite(reg->defenseMechanism, sizeof(char), strlen(reg->defenseMechanism), *arquivo);
        fwrite(&sep, sizeof(char), 1, *arquivo);
    }

    // Preencher o restante com '$'
    if (tamRestante > 0) {
        char lixo = '$';
        for (int i = 0; i < tamRestante; i++) {
            fwrite(&lixo, sizeof(char), 1, *arquivo);
        }
    }
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
    Desaloca um registro da memória
*/
void ApagarRegistro(REGISTRO **reg) {
    if (*reg == NULL){
        printf("Passado registro nulo para ser apagado.\n");
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

REGISTRO *LerRegistro(FILE *arquivo) {
    if (arquivo == NULL) {
        DispararErro(ErroArquivoInvalido());
        return NULL;
    }
    
    REGISTRO *reg = CriarRegistroVazio();
    if (reg == NULL) {
        DispararErro(ErroAlocacaoMemoria());
        return NULL;
    }
    
    // Ler campos fixos
    if (!_LerCamposFixos(arquivo, reg)) {
        // Falha nesse processo pode indicar que
        // chegou-se ao fim do arquivo, logo é esperado
        // caso de falha, assim, deve-se continuar execução.
        ApagarRegistro(&reg);
        return NULL;
    }

    // Calcular tamanho dos campos variáveis
    int tamRestante = reg->tamanhoRegistro - 20; // 20 bytes dos campos fixos
    
    // Ler campos variáveis se existirem
    if (tamRestante > 0 && !_LerCamposVariaveis(arquivo, tamRestante, reg)) {
        // Se existe campo fixo, o registro existe, logo não é esperado
        // continuar o programa desse ponto se os campos variáveis não foram lidos.
        printf("Erro ao ler os campos variáveis.\n");
        ApagarRegistro(&reg);
        return NULL;
    }
    
    return reg;
}

void RemoverRegistro(FILE *arquivo, CABECALHO *c, REGISTRO *reg) {
    if (arquivo == NULL || reg == NULL) {
        printf("Registro ou arquivo nulos na remoção do registro.\n");
        DispararErro(ErroPonteiroInvalido());
        return;
    }

    // Salvar dados a serem mantidos no registrador
    // e faz o ajuste do lixo decorrente dos dados antigos.
    reg->removido = '1';
    reg->prox = c->topo;
    c->topo = ftell(arquivo);

    EscreverRegistro(&arquivo, reg);
    return;
}

bool SelecionarPorCriterio(REGISTRO* criterio, REGISTRO* reg) {
    if(reg->removido == REMOVIDO || reg == NULL) return false;
    // Se não há critério e registro não removido e não nulo
    // o registro é valido.
    if(criterio == NULL) return true;

    /* Um registro é valido, se ele tem os criterios passados*/
    // Verifica idAttack
    if (criterio->idAttack != -1 && 
        criterio->idAttack != reg->idAttack) {
        return false;
    }
    
    // Verifica year
    if (criterio->year != -1 && 
        criterio->year != reg->year) {
        return false;
    }
    
    // Verifica financialLoss
    if (criterio->financialLoss != -1 && 
        criterio->financialLoss != reg->financialLoss) {
        return false;
    }
    
    // Verifica strings (country, attackType, etc.)
    if (criterio->country != NULL && 
        (reg->country == NULL || 
            strcmp(criterio->country, reg->country) != 0)) {
        return false;
    }
    
    if (criterio->attackType != NULL && 
        (reg->attackType == NULL || 
            strcmp(criterio->attackType, reg->attackType) != 0)) {
        return false;
    }
    
    if (criterio->targetIndustry != NULL && 
        (reg->targetIndustry == NULL || 
            strcmp(criterio->targetIndustry, reg->targetIndustry) != 0)) {
        return false;
    }
    
    if (criterio->defenseMechanism != NULL && 
        (reg->defenseMechanism == NULL || 
            strcmp(criterio->defenseMechanism, reg->defenseMechanism) != 0)) {
        return false;
    }
    
    // Se todas as verificações passaram
    return true;
}

REGISTRO *DefinirCriterio(){
    // Quantidade de campos em cada busca(filtros).
    int quantCampos;
    scanf("%d", &quantCampos);

    // Registro a ser usado como filtro
    REGISTRO *reg = CriarRegistroVazio();

    // Ler campos inseridos pelo usuário
    for(int i = 0; i < quantCampos; i++){
        char *campo = LerString();
        // Switch case do campo
        if(!strcmp(campo, "idAttack")){
            scanf(" %d", &(reg->idAttack));
        } else if(!strcmp(campo, "year")) {
            scanf(" %d", &(reg->year));
        } else if(!strcmp(campo, "financialLoss")) {
            scanf(" %f", &(reg->financialLoss));
        } else if(!strcmp(campo, "country")) {
            reg->country = LerStringComAspas();
        } else if(!strcmp(campo, "attackType")) {
            reg->attackType = LerStringComAspas();
        } else if(!strcmp(campo, "targetIndustry")) {
            reg->targetIndustry = LerStringComAspas();
        } else if(!strcmp(campo, "defenseMechanism")) {
            reg->defenseMechanism = LerStringComAspas();
        } else {
            // TODO: erro select invalido
        }

        free(campo);
        campo = NULL;
    }
    return reg;
}

void AtualizarRegistro(REGISTRO *destino,REGISTRO *origem){
    if(destino == NULL) DispararErro(ErroPonteiroInvalido());

    if(origem == NULL) return;

    /* Atualizar campos que foram inicializados */
    if (origem->idAttack != -1) destino->idAttack = origem->idAttack;
    if (origem->year != -1) destino->year = origem->year;
    if (origem->financialLoss != -1) destino->financialLoss = origem->financialLoss;
    
    /* Atualizar Campos das Strings */
    if (origem->country != NULL) {
        // Remover campo variável
        destino->tamanhoRegistro -= strlen(destino->country);
        free(destino->country);
        // Alocar memória para novo valor.
        int tamCampoVariavel = strlen(origem->country);
        char *campoAtualizado = (char*) malloc((tamCampoVariavel + 1)*sizeof(char));
        destino->tamanhoRegistro += tamCampoVariavel;
        // Preencher campo, e atualizar ponteiro.
        strcpy(campoAtualizado, origem->country);
        campoAtualizado[tamCampoVariavel] = '\0';
        destino->country = campoAtualizado;
    }

    if (origem->attackType != NULL) {
        // Remover campo variável
        destino->tamanhoRegistro -= strlen(destino->attackType);
        free(destino->attackType);
        // Alocar memória para novo valor.
        int tamCampoVariavel = strlen(origem->attackType);
        char *campoAtualizado = (char*) malloc((tamCampoVariavel + 1)*sizeof(char));
        destino->tamanhoRegistro += tamCampoVariavel;
        // Preencher campo, e atualizar ponteiro.
        strcpy(campoAtualizado, origem->attackType);
        campoAtualizado[tamCampoVariavel] = '\0';
        destino->attackType = campoAtualizado;
    }

    if (origem->defenseMechanism != NULL) {
        // Remover campo variável
        destino->tamanhoRegistro -= strlen(destino->defenseMechanism);
        free(destino->defenseMechanism);
        // Alocar memória para novo valor.
        int tamCampoVariavel = strlen(origem->defenseMechanism);
        char *campoAtualizado = (char*) malloc((tamCampoVariavel + 1)*sizeof(char));
        destino->tamanhoRegistro += tamCampoVariavel;
        // Preencher campo, e atualizar ponteiro.
        strcpy(campoAtualizado, origem->defenseMechanism);
        campoAtualizado[tamCampoVariavel] = '\0';
        destino->defenseMechanism = campoAtualizado;
    }

    if (origem->targetIndustry != NULL) {
        // Remover campo variável
        destino->tamanhoRegistro -= strlen(destino->targetIndustry);
        free(destino->targetIndustry);
        // Alocar memória para novo valor.
        int tamCampoVariavel = strlen(origem->targetIndustry);
        char *campoAtualizado = (char*) malloc((tamCampoVariavel + 1)*sizeof(char));
        destino->tamanhoRegistro += tamCampoVariavel;
        // Preencher campo, e atualizar ponteiro.
        strcpy(campoAtualizado, origem->targetIndustry);
        campoAtualizado[tamCampoVariavel] = '\0';
        destino->targetIndustry = campoAtualizado;
    }
    
    return;
}