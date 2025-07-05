#include "erros.h"
#include "cabecalho.h"
#include "SQL.h"
#include "registro.h"
#include "criterio.h"
#include "utils.h"
#include "indice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Protótipos */
void FUNCIONALIDADE1(void);
void FUNCIONALIDADE2(void);
void FUNCIONALIDADE3(void);
void FUNCIONALIDADE4(void);
void FUNCIONALIDADE5(void);
void FUNCIONALIDADE6(void);
void FUNCIONALIDADE7(void);
void FUNCIONALIDADE8(void);
void FUNCIONALIDADE10(void);

int main(void) {
    // Ler funcionalidade selecionada e nome arquivo de entrada
    int funcionalidade;
    scanf("%d", &funcionalidade);

    switch (funcionalidade){
        case 1:
            FUNCIONALIDADE1();
            break;
        case 2:
            FUNCIONALIDADE2();
            break;
        case 3:
            FUNCIONALIDADE3();
            break;
        case 4:
            FUNCIONALIDADE4();
            break;
        case 5:
            FUNCIONALIDADE5();
            break;
        case 6:
            FUNCIONALIDADE6();
            break;
        case 7:
            FUNCIONALIDADE7();
            break;
        case 8:
            FUNCIONALIDADE8();
            break;
        case 10:
            FUNCIONALIDADE10();
            break;
        default:
            break;
    } 
    return 0;
}


/*
CREATE_TABLE:
Função que le registros em um CSV e armazena em um arquivo binário
*/
void FUNCIONALIDADE1(void){
    // Ler nome dos arquivos de entrada e saída.
    char *nomeArquivoEntrada = LerString();
    char *nomeArquivoSaida = LerString();
    // Abrir arquivos de entrada e saída
    FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "r");

    // Verificar se o arquivo de entrada existe
    if(arquivoEntrada == NULL){
        // Dispara mensagem de erro
        DispararErro(ErroProcessamentoArquivo());
    } else {
        FILE *arquivoSaida = fopen(nomeArquivoSaida, "wb");
        
        // Verifica se o arquivo foi criado
        if(arquivoSaida == NULL){
            // Dispara erro fatal
            DispararErro(ErroCriarArquivo());
        }

        // Ler CSV do arquivo de entrada e escreve em binário no de saída.
        LerCsvParaBinario(arquivoEntrada, arquivoSaida);

        // Fechar arquivos de entrada e saída
        fclose(arquivoEntrada);
        fclose(arquivoSaida);

        // Executar função fornecida para  
        // mostrar a saída do arquivo ataques.bin
        binarioNaTela(nomeArquivoSaida);
    }
    // Liberar memória do nome dos arquivos
    free(nomeArquivoEntrada);
    nomeArquivoEntrada = NULL;
    free(nomeArquivoSaida);
    nomeArquivoSaida = NULL;

    return;
}

/*
SELECT:
Exibe os registros no arquivo de dados informado
*/
void FUNCIONALIDADE2(void) {
    // Ler nome do arquivo de entrada.
    char *nomeArquivoEntrada = LerString();

    // Abrir arquivos de entrada
    FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb");

    // Verificar se arquivo de entrada existe
    if(arquivoEntrada == NULL){
        // Dispara mensagem de erro
        DispararErro(ErroProcessamentoArquivo());
    } else {
        // Imprimir todos os registros(não há critério).
        ExibirRegistrosDadoCriterio(arquivoEntrada, NULL);
        // Fechar arquivo
        fclose(arquivoEntrada);
        arquivoEntrada = NULL;
    }
    // Liberar memória 
    free(nomeArquivoEntrada);
    nomeArquivoEntrada = NULL;

    return;
}

/*
SELECT_WHERE:
Busca registros de acordo com um filtro
*/
void FUNCIONALIDADE3(void){
    // Ler nome do arquivo de entrada.
    char *nomeArquivoEntrada = LerString();

    // Abrir arquivos de entrada
    FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb");

    // Quantidades de buscas a serem realizadas
    int quantBuscas;
    scanf("%d", &quantBuscas);

    // Verificar se arquivo de entrada existe
    if(arquivoEntrada == NULL){
        // Dispara mensagem de erro
        DispararErro(ErroProcessamentoArquivo());
    } else {
        while(quantBuscas--){
            // Le os criterios a serem avaliados
            CRITERIO *criterio = DefinirCriterio();

            ExibirRegistrosDadoCriterio(arquivoEntrada, criterio);

            printf("**********\n");

            // Apagar registro filtro
            ApagarCriterio(&criterio);
        }
        // Fechar arquivo
        fclose(arquivoEntrada);
        arquivoEntrada = NULL;
    }   

    // Liberar memória 
    free(nomeArquivoEntrada);
    nomeArquivoEntrada = NULL;

    return;
}

/*
DELETE: 
Remove registros logicamente de acordo com filtro 
*/
void FUNCIONALIDADE4(void){
    // Ler nome do arquivo de entrada.
    char *nomeArquivoEntrada = LerString();

    // Abrir arquivo de entrada para leitura e escrita
    FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb+");

    // Quantidades de remoções a serem realizadas
    int quantRemove;
    scanf("%d", &quantRemove);

    // Verificar se arquivo de entrada existe
    if(arquivoEntrada == NULL){
        // Dispara mensagem de erro
        DispararErro(ErroProcessamentoArquivo());
    } else {
        while(quantRemove--){
            // Le os criterios a serem avaliados
            CRITERIO *criterio = DefinirCriterio();

            RemoverRegistroDadoCriterio(arquivoEntrada, criterio);
            // Apagar registro criterio
            ApagarCriterio(&criterio);
        }
        // Fechar arquivo
        fclose(arquivoEntrada);
        arquivoEntrada = NULL;

        // Executar função fornecida para  
        // mostrar a saída do arquivo ataques.bin
        binarioNaTela(nomeArquivoEntrada);
    }   

    // Liberar memória 
    free(nomeArquivoEntrada);
    nomeArquivoEntrada = NULL;

    return;
}

/* INSERT */
void FUNCIONALIDADE5(void){
    // Ler nome do arquivo de entrada.
    char *nomeArquivoEntrada = LerString();

    // Abrir arquivo de entrada para leitura e escrita
    FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb+");

    // Quantidades de remoções a serem realizadas
    int quantInsert;
    scanf("%d\n", &quantInsert);

    // Verificar se arquivo de entrada existe
    if(arquivoEntrada == NULL){
        // Dispara mensagem de erro
        DispararErro(ErroProcessamentoArquivo());
    } else {
        while(quantInsert--){
            // Ler entrada da inserção.
            REGISTRO *reg = CriarRegistroVazio();

            // Ler campos fixos
            scanf("%d", &reg->idAttack);
            reg->year = LerCampoInteiro();
            reg->financialLoss = LerCampoFloat();

            // Ler Campos variáveis
            reg->country = LerStringComAspas();
            if(reg->country != NULL) 
                reg->tamanhoRegistro += strlen(reg->country) + 2;

            reg->attackType = LerStringComAspas();
            if(reg->attackType != NULL) 
                reg->tamanhoRegistro += strlen(reg->attackType) + 2;

            reg->targetIndustry = LerStringComAspas();
            if(reg->targetIndustry != NULL) 
                reg->tamanhoRegistro += strlen(reg->targetIndustry) + 2;

            reg->defenseMechanism = LerStringComAspas();
            if(reg->defenseMechanism != NULL) 
                reg->tamanhoRegistro += strlen(reg->defenseMechanism) + 2;

            InserirRegistro(arquivoEntrada, reg);
            ApagarRegistro(&reg);
        }
        // Fechar arquivo
        fclose(arquivoEntrada);
        arquivoEntrada = NULL;

        // Executar função fornecida para  
        // mostrar a saída do arquivo ataques.bin
        binarioNaTela(nomeArquivoEntrada);
    }   

    // Liberar memória 
    free(nomeArquivoEntrada);
    nomeArquivoEntrada = NULL;

    return;
}

/* UPDATE */
void FUNCIONALIDADE6(void){
    // Ler nome do arquivo de entrada.
    char *nomeArquivoEntrada = LerString();

    // Abrir arquivo de entrada para leitura e escrita
    FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb+");

    // Quantidades de atualizações a serem realizadas
    int quantUpdate;
    scanf("%d\n", &quantUpdate);

    // Verificar se arquivo de entrada existe
    if(arquivoEntrada == NULL){
        // Dispara mensagem de erro
        DispararErro(ErroProcessamentoArquivo());
    } else {
        while(quantUpdate--){
            // Le os criterios a serem avaliados
            CRITERIO *criterio = DefinirCriterio();
            CRITERIO *valoresAtualizados = DefinirCriterio();

            AtualizarRegistroDadoCriterio(arquivoEntrada, criterio, valoresAtualizados);
           
            // Apagar registro criterio
            ApagarCriterio(&criterio);
            ApagarCriterio(&valoresAtualizados);
        }
        // Fechar arquivo
        fclose(arquivoEntrada);
        arquivoEntrada = NULL;

        // Executar função fornecida para  
        // mostrar a saída do arquivo ataques.bin
        binarioNaTela(nomeArquivoEntrada);
    }   

    // Liberar memória 
    free(nomeArquivoEntrada);
    nomeArquivoEntrada = NULL;

    return;
}

/*
CREATE_INDEX:
Função que recebe um arquivo de dados 
e retorna um arquivo de índices.
*/
void FUNCIONALIDADE7(void){
    // Ler nome dos arquivos de entrada e saída.
    char *nomeArquivoEntrada = LerString();
    char *nomeArquivoSaida = LerString();
    // Abrir arquivos de entrada e saída
    FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb");

    // Verificar se o arquivo de entrada existe
    if(arquivoEntrada == NULL){
        // Dispara mensagem de erro
        DispararErro(ErroProcessamentoArquivo());
    } else {
        FILE *arquivoSaida = fopen(nomeArquivoSaida, "wb+");
        
        // Verifica se o arquivo foi criado
        if(arquivoSaida == NULL){
            // Dispara erro fatal
            DispararErro(ErroCriarArquivo());
        }
        
        CriarArquivoIndice(arquivoEntrada, arquivoSaida);

        // Fechar arquivos de entrada e saída
        fclose(arquivoEntrada);
        fclose(arquivoSaida);

        // Executar função fornecida para  
        // mostrar a saída do arquivo ataques.bin
        binarioNaTela(nomeArquivoSaida);
    }
    // Liberar memória do nome dos arquivos
    free(nomeArquivoEntrada);
    nomeArquivoEntrada = NULL;
    free(nomeArquivoSaida);
    nomeArquivoSaida = NULL;

    return;
}

void FUNCIONALIDADE8(void){
    // Ler nome do arquivo de dados.
    char *nomeArquivoDados = LerString();

    // Ler nome do arquivo de índice.
    char *nomeArquivoIndice = LerString();

    // Abrir arquivos de entrada
    FILE *arquivoDados = fopen(nomeArquivoDados, "rb");
    FILE *arquivoIndices = fopen(nomeArquivoIndice, "rb");

    // Quantidades de buscas a serem realizadas
    int quantBuscas;
    scanf("%d", &quantBuscas);

    // Verificar se arquivo de entrada existe
    if(arquivoDados == NULL || arquivoIndices == NULL){
        // Dispara mensagem de erro
        DispararErro(ErroProcessamentoArquivo());
    } else {
        while(quantBuscas--){
            // Le os criterios a serem avaliados
            CRITERIO *criterio = DefinirCriterio();

            if(criterio->temIdAttack) {
                ExibirRegistroDadoIndice(arquivoDados, 
                                        arquivoIndices, 
                                        criterio->criterios->idAttack);
            } else {
                ExibirRegistrosDadoCriterio(arquivoDados, criterio);
            }

            printf("**********\n");

            // Apagar registro filtro
            ApagarCriterio(&criterio);
        }
        // Fechar arquivo
        fclose(arquivoDados);
        arquivoDados = NULL;
        fclose(arquivoIndices);
        arquivoIndices = NULL;
    }   

    // Liberar memória 
    free(nomeArquivoDados);
    nomeArquivoDados = NULL;
    free(nomeArquivoIndice);
    nomeArquivoIndice = NULL;

    return;
}

void FUNCIONALIDADE10(void){
    // Ler nome do arquivo de dados.
    char *nomeArquivoDados = LerString();
    // Ler nome do arquivo de índice.
    char *nomeArquivoIndice = LerString();

    // Abrir arquivos de entrada
    FILE *arquivoDados = fopen(nomeArquivoDados, "rb+");
    FILE *arquivoIndices = fopen(nomeArquivoIndice, "rb+");

    // Quantidades de remoções a serem realizadas
    int quantInsert;
    scanf("%d\n", &quantInsert);

    // Verificar se arquivo de entrada existe
    if(arquivoDados == NULL || arquivoIndices == NULL){
        // Dispara mensagem de erro
        DispararErro(ErroProcessamentoArquivo());
    } else {
        while(quantInsert--){
            // Ler entrada da inserção.
            REGISTRO *reg = CriarRegistroVazio();

            // Ler campos fixos
            scanf("%d", &reg->idAttack);
            reg->year = LerCampoInteiro();
            reg->financialLoss = LerCampoFloat();

            // Ler Campos variáveis
            reg->country = LerStringComAspas();
            if(reg->country != NULL) 
                reg->tamanhoRegistro += strlen(reg->country) + 2;

            reg->attackType = LerStringComAspas();
            if(reg->attackType != NULL) 
                reg->tamanhoRegistro += strlen(reg->attackType) + 2;

            reg->targetIndustry = LerStringComAspas();
            if(reg->targetIndustry != NULL) 
                reg->tamanhoRegistro += strlen(reg->targetIndustry) + 2;

            reg->defenseMechanism = LerStringComAspas();
            if(reg->defenseMechanism != NULL) 
                reg->tamanhoRegistro += strlen(reg->defenseMechanism) + 2;

            InserirRegistroIndice(arquivoDados, arquivoIndices, reg);
            
            ApagarRegistro(&reg);
        }
        // Fechar arquivo
        fclose(arquivoDados);
        arquivoDados = NULL;
        fclose(arquivoIndices);
        arquivoDados = NULL;

        // Executar função fornecida para  
        // mostrar a saída do arquivo ataques.bin
        binarioNaTela(nomeArquivoDados);
        binarioNaTela(nomeArquivoIndice);
    }   

    // Liberar memória 
    free(nomeArquivoDados);
    nomeArquivoDados = NULL;
    free(nomeArquivoIndice);
    nomeArquivoIndice = NULL;

    return;
}

void FUNCIONALIDADE11(void){
    // Ler nome do arquivo de dados.
    char *nomeArquivoDados = LerString();
    // Ler nome do arquivo de índice.
    char *nomeArquivoIndice = LerString();

    // Abrir arquivos de entrada
    FILE *arquivoDados = fopen(nomeArquivoDados, "rb+");
    FILE *arquivoIndices = fopen(nomeArquivoIndice, "rb+");

    // Quantidades de atualizações a serem realizadas
    int quantUpdate;
    scanf("%d\n", &quantUpdate);

    // Verificar se arquivo de entrada existe
    if(arquivoDados == NULL || arquivoIndices == NULL){
        // Dispara mensagem de erro
        DispararErro(ErroProcessamentoArquivo());
    } else {
        while(quantUpdate--){
            // Le os criterios a serem avaliados
            CRITERIO *criterio = DefinirCriterio();
            CRITERIO *valoresAtualizados = DefinirCriterio();

            // Atualizar registros
            AtualizarRegistroDadoIndice(arquivoDados,
                                        arquivoIndices,
                                        criterio, 
                                        valoresAtualizados);
           
            // Apagar registro criterio
            ApagarCriterio(&criterio);
            ApagarCriterio(&valoresAtualizados);
        }
        // Fechar arquivo
        fclose(arquivoDados);
        arquivoDados = NULL;
        fclose(arquivoIndices);
        arquivoDados = NULL;

        // Executar função fornecida para  
        // mostrar a saída do arquivo ataques.bin
        binarioNaTela(nomeArquivoDados);
        binarioNaTela(nomeArquivoIndice);
    }   

    // Liberar memória 
    free(nomeArquivoDados);
    nomeArquivoDados = NULL;
    free(nomeArquivoIndice);
    nomeArquivoIndice = NULL;

    return;
}

