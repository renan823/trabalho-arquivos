#include "erros.h"
#include "cabecalho.h"
#include "registro.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
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

        // Escreve cabecalho em binario no arquivo de saida
        CABECALHO *cabecalho =  CriarCabecalhoPadrao();
        EscreverCabecalho(&arquivoSaida, cabecalho);
        ApagarCabecalho(&cabecalho);

        // Buffer para armazenar linha do CSV
        int tamBuffer = 256;
        char *buffer = (char*) malloc(sizeof(char)*tamBuffer);

        // Eliminar introdução do arquivo CSV
        PegarLinha(&buffer, tamBuffer, arquivoEntrada);

        // número de registros não removidos presentes no arquivo
        int nroRegArq = 0;

        // Retorna tamanho da linha e não altera o ponteiro.
        // Se o tamanho da linha for zero, não há dados a serem escritos mais(fim do arquivo).
        while(PegarLinha(&buffer, tamBuffer, arquivoEntrada)){
            // Adicionar numero de registros não removidos no arquivo.
            nroRegArq++;
            
            // Preencher registro passando buffer
            REGISTRO *reg = CriarRegistroVazio();
            PreencherRegistro(&reg, buffer);
            // Inserir registro em arquivo binário        
            EscreverRegistro(&arquivoSaida, reg);

            // Apagar registro
            ApagarRegistro(&reg);
        };
        
        // Guardar próximo byteOffSet disponível
        long int proxByteOffset = ftell(arquivoSaida);

        // Atualizar cabeçalho arquivo(-1 para não alterar campos desnecessários)
        AtualizarCabecalho(&arquivoSaida, '1', -1, proxByteOffset, nroRegArq, -1);

        // Liberar memória do buffer
        free(buffer);
        buffer = NULL;

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
Exibe os registros no arquivo de dados informado
*/
void FUNCIONALIDADE2(void) {
    char *nomeArquivoEntrada = LerString();

    // Abrir arquivo para leitura
    FILE *arquivo = fopen(nomeArquivoEntrada, "rb");
    if (arquivo == NULL) {
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    // Executar leitura
    CABECALHO *c = LerCabecalho(&arquivo);

    if (c == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    // Cabeçalho inconsistente não será lido
    if (c->status == INCONSISTENTE) {
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    if (c->nroRegArq == 0) {
        DispararErro(ErroRegistroInexistente());
        return;
    }

    printf("%d\n", c->nroRegArq);

    // Ler dados 
    int total = 0;

    while (total < c->nroRegArq) {
        REGISTRO *reg = LerRegistro(arquivo);

        if (!reg->removido) {
            ExibirRegistro(reg);
        }

        ApagarRegistro(&reg); 

        total++;
    }

    ApagarCabecalho(&c);

    fclose(arquivo);
    free(nomeArquivoEntrada);
}

/*
Busca registros de acordo com um filtro
*/
void FUNCIONALIDADE3(void){
    // Ler nome dos arquivos de entrada e saída.
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
                    scanf("%d", &(reg->idAttack));
                } else if(!strcmp(campo, "year")) {
                    scanf("%d", &(reg->year));
                } else if(!strcmp(campo, "financialLoss")) {
                    scanf("%f", &(reg->financialLoss));
                } else if(!strcmp(campo, "country")) {
                    reg->country = LerString();
                } else if(!strcmp(campo, "attackType")) {
                    reg->attackType = LerString();
                } else if(!strcmp(campo, "targetIndustry")) {
                    reg->targetIndustry = LerString();
                } else if(!strcmp(campo, "defenseMechanism")) {
                    reg->defenseMechanism = LerString();
                } else {
                    // TODO: erro select invalido
                }

                free(campo);
                campo = NULL;
            }

            BuscaRegistroPorCampo(arquivoEntrada, reg);

            // Apagar registro filtro
            ApagarRegistro(&reg);
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

        default:
            break;
    }   
    return 0;
}