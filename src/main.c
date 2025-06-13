#include "erros.h"
#include "cabecalho.h"
#include "registro.h"
#include "utils.h"
#include "buscar.h"
#include "create_table.h"
#include "delete.h"
#include "insert.h"
#include "update.h"
#include "criterio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

            DELETE(arquivoEntrada, criterio);
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

/*
Insere um registro
*/
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

            INSERT(arquivoEntrada, reg);
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

void FUNCIONALIDADE6(void){
    // Ler nome do arquivo de entrada.
    char *nomeArquivoEntrada = LerString();

    // Abrir arquivo de entrada para leitura e escrita
    FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb+");

    // Quantidades de remoções a serem realizadas
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

            UPDATE(arquivoEntrada, criterio, valoresAtualizados);
           
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
        default:
            break;
    }   
    return 0;
}
