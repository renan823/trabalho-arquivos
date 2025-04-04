#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Cabeçalho do arquivo de dados.

status: 0 arquivo inconsistente, 1 arquivo consistente
topo: byte offset de um registro removido (-1 caso não haja registro)
proxByteOffset: armazena o próximo byte offset, iniciado em zero
nroRegArq: numero de registros não removidos, iniciado em zero
nroRegRem: numero de registros removidos, iniciado em zero
descreveIdentificador: descrição do idAttack (por padrão: IDENTIFICADOR DO ATAQUE)
descreveYear: descrição do year (por padrão: ANO EM QUE O ATAQUE OCORREU)
descreveFinancialLoss: descrição do financialLoss (por padrão: PREJUIZO CAUSADO PELO ATAQUE)
codDescreveContry: código da keyword de country (por padrão: 1)
descreveCountry: descrição do country (por padrão: PAIS ONDE OCORREU O ATAQUE)
codDescreveType: código da keyword de attackType (por padrão: 2)
descreveType: descrição de attackType (por padrão: TIPO DE AMEACA A SEGURANCA CIBERNETICA)
codDescreveTargetIndustry: keyword para targetIndustry (por padrão: 3)
descreveTargetIndustry: descrição de targetIndustry (por padrão: SETOR DA INDUSTRIA QUE SOFREU O ATAQUE)
codDescreveDefense: keyword de defenseMechanism (por padrão: 4);
descreveDefense: descrição de defenseMechanism (por padrão: ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA)
*/

typedef struct cabecalho CABECALHO;

struct cabecalho {
    char status;
    long int topo;
    long int proxByteOffset;
    int nroRegArq;
    int nroRegRem;
    char descreveIdentificador[23];
    char descreveYear[27];
    char descreveFinancialLoss[28];
    char codDescreveCountry;
    char descreveCountry[26];
    char codDescreveType;
    char descreveType[38];
    char codDescreveTargetIndustry;
    char descreveTargetIndustry[38];
    char codDescreveDefense;
    char descreveDefense[67];
};

/*
Registro do arquivo de dados.

- Gerenciamento de registros logicamente removidos:

removido: 1 registro logicamente removido, 0 registro não marcado como removido(char - 1 byte)
tamanhoRegistro: nº bytes do registro(inteiro - 4 bytes)
prox: byte offset do próximo registro removido(long int - 8 bytes)

- Campos de tamanho fixo:

idAttack: código identificador do ataque(inteiro - 4 bytes)
year: ano em que o ataque ocorreu(inteiro - 4 bytes)
financialLoss: prejuízo causado pelo ataque(float - 4 bytes)

- Campos de tamanho variável:

country: país onde ocorreu o ataque(string)
attackType: tipo de ameaça a segunrança cibernética(string)
targetIndustry: setor da indústria que sofreu o ataque(string)
defenseMechanism: estratégia de defesa cibernética empregada para resolver o problema(string)
*/

typedef struct registro REGISTRO;

struct REGISTRO {
    // Gerenciamento
    char removido;
    int tamanhoRegistro;
    long int prox;
    // Tamanho fixo
    int idAttack;
    int year;
    float financialLoss;
    // Tamanho variável
    char *country; 
    char *attackType; 
    char *targetIndustry; 
    char *defenseMechanism; 
};

/*
Função para gerar um cabeçalho de dados padrão.
*/
CABECALHO *CriarCabecalhoPadrao(void) {
    CABECALHO *c = (CABECALHO*) malloc(sizeof(CABECALHO));

    // Se houver espaço na memória heap
    if (c != NULL) {
        // Inicializar campos do cabecalho
        c->status = '1';
        c->topo = -1;
        c->proxByteOffset = 0;
        c->nroRegArq = 0;
        c->nroRegRem = 0;
        memcpy(c->descreveIdentificador, "IDENTIFICADOR DO ATAQUE", 23);
        memcpy(c->descreveYear, "ANO EM QUE O ATAQUE OCORREU", 27);
        memcpy(c->descreveFinancialLoss, "PREJUIZO CAUSADO PELO ATAQUE", 28);
        c->codDescreveCountry = '1';
        memcpy(c->descreveCountry, "PAIS ONDE OCORREU O ATAQUE", 26);
        c->codDescreveType = '2';
        memcpy(c->descreveType, "TIPO DE AMEACA A SEGURANCA CIBERNETICA", 38);
        c->codDescreveTargetIndustry = '3';
        memcpy(c->descreveTargetIndustry, "SETOR DA INDUSTRIA QUE SOFREU O ATAQUE", 38);
        c->codDescreveDefense = '4';
        memcpy(c->descreveDefense, "ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA", 67);
    }

    return c;
}

/* 
Função que escreve o cabeçalho de dados no arquivo informado
*/
void EscreverCabecalho(FILE **arquivo, CABECALHO *c) {
    if (*arquivo == NULL || c == NULL) {
        return;
    }

    // Escrever dados
    fwrite(&(c->status), sizeof(char), 1, *arquivo);
    fwrite(&(c->topo), sizeof(long int), 1, *arquivo);
    fwrite(&(c->proxByteOffset), sizeof(long int), 1, *arquivo);
    fwrite(&(c->nroRegArq), sizeof(int), 1, *arquivo);
    fwrite(&(c->nroRegRem), sizeof(int), 1, *arquivo);
    fwrite(c->descreveIdentificador, sizeof(char), 23, *arquivo);
    fwrite(c->descreveYear, sizeof(char), 27, *arquivo);
    fwrite(c->descreveFinancialLoss, sizeof(char), 28, *arquivo);
    fwrite(&(c->codDescreveCountry), sizeof(char), 1, *arquivo);
    fwrite(c->descreveCountry, sizeof(char), 26, *arquivo);
    fwrite(&(c->codDescreveType), sizeof(char), 1, *arquivo);
    fwrite(c->descreveType, sizeof(char), 38, *arquivo);
    fwrite(&(c->codDescreveTargetIndustry), sizeof(char), 1, *arquivo);
    fwrite(c->descreveTargetIndustry, sizeof(char), 38, *arquivo);
    fwrite(&(c->codDescreveDefense), sizeof(char), 1, *arquivo);
    fwrite(c->descreveDefense, sizeof(char), 67, *arquivo);

}

/* 
Função que libera espaço alocado par ao cabecalho na heap
*/
void ApagarCabecalho(CABECALHO **c){
    // Verificar se endereço passado é nulo
    if(c != NULL){
        // Verificar se cabecalho existe
        if(*c != NULL){
            // Liberar memoria do cabecalho
            free(*c);
            *c = NULL; // Evita ponteiro selvagem
        }
    }
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
    arquivoEntrada += (tamLinha - 1); 
    // Lê o \n para chegar na primeira linha do arquivo
    fread(buffer, sizeof(char), 1, arquivoEntrada);

    // Retorna tamanho da linha e não altera o ponteiro.
    // Se o tamanho da linha for zero, não há dados a serem escritos mais(fim do arquivo).
    while((tamLinha = TamanhoLinha(arquivoEntrada)) != 0){
        // Verifica se o tamanho atual do buffer é suficiente
        while(tamLinha >= buffer){
            free(buffer);
            // Aumenta tamanho do buffer em 50
            tamBuffer = tamBuffer + 50;
            buffer = (char*) malloc(sizeof(char)*tamBuffer);
        }
        // Ler dados do arquivo de entrada no buffer
        fread(buffer, sizeof(char), tamLinha, arquivoEntrada);
        // TODO: formatar dados para serem salvos em binário
        // Adicionar função preencher registro passando buffer
        // TODO: escrever no arquivo de saida
        // Adicionar função inserir registro em arquivo binário        
    };

}

// Retorna tamanho em bytes da linha a ser lida(0 caso chegue no final)
int TamanhoLinha(FILE *arquivoAberto){
    int tamLinha = 0;
    char byteAtual;
    // Retorna 1 se conseguiu ler o caracter, se não conseguiu, chegou-se ao final do arquivo.
    while(1 == fread(&byteAtual, sizeof(char), 1, arquivoAberto)){
        tamLinha++;
        if(byteAtual == '\n') break;
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