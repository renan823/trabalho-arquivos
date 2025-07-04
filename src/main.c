// #include "erros.h"
// #include "cabecalho.h"
// #include "SQL.h"
// #include "registro.h"
// #include "criterio.h"
// #include "utils.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// /* Protótipos */
// void FUNCIONALIDADE1(void);
// void FUNCIONALIDADE2(void);
// void FUNCIONALIDADE3(void);
// void FUNCIONALIDADE4(void);
// void FUNCIONALIDADE5(void);
// void FUNCIONALIDADE6(void);

// int main(void) {
//     // Ler funcionalidade selecionada e nome arquivo de entrada
//     int funcionalidade;
//     scanf("%d", &funcionalidade);

//     switch (funcionalidade){
//         case 1:
//             FUNCIONALIDADE1();
//             break;
//         case 2:
//             FUNCIONALIDADE2();
//             break;
//         case 3:
//             FUNCIONALIDADE3();
//             break;
//         case 4:
//             FUNCIONALIDADE4();
//             break;
//         case 5:
//             FUNCIONALIDADE5();
//             break;
//         case 6:
//             FUNCIONALIDADE6();
//             break;
//         default:
//             break;
//     } 
//     return 0;
// }


// /*
// CREATE_TABLE:
// Função que le registros em um CSV e armazena em um arquivo binário
// */
// void FUNCIONALIDADE1(void){
//     // Ler nome dos arquivos de entrada e saída.
//     char *nomeArquivoEntrada = LerString();
//     char *nomeArquivoSaida = LerString();
//     // Abrir arquivos de entrada e saída
//     FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "r");

//     // Verificar se o arquivo de entrada existe
//     if(arquivoEntrada == NULL){
//         // Dispara mensagem de erro
//         DispararErro(ErroProcessamentoArquivo());
//     } else {
//         FILE *arquivoSaida = fopen(nomeArquivoSaida, "wb");
        
//         // Verifica se o arquivo foi criado
//         if(arquivoSaida == NULL){
//             // Dispara erro fatal
//             DispararErro(ErroCriarArquivo());
//         }

//         // Ler CSV do arquivo de entrada e escreve em binário no de saída.
//         LerCsvParaBinario(arquivoEntrada, arquivoSaida);

//         // Fechar arquivos de entrada e saída
//         fclose(arquivoEntrada);
//         fclose(arquivoSaida);

//         // Executar função fornecida para  
//         // mostrar a saída do arquivo ataques.bin
//         binarioNaTela(nomeArquivoSaida);
//     }
//     // Liberar memória do nome dos arquivos
//     free(nomeArquivoEntrada);
//     nomeArquivoEntrada = NULL;
//     free(nomeArquivoSaida);
//     nomeArquivoSaida = NULL;

//     return;
// }

// /*
// SELECT:
// Exibe os registros no arquivo de dados informado
// */
// void FUNCIONALIDADE2(void) {
//     // Ler nome do arquivo de entrada.
//     char *nomeArquivoEntrada = LerString();

//     // Abrir arquivos de entrada
//     FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb");

//     // Verificar se arquivo de entrada existe
//     if(arquivoEntrada == NULL){
//         // Dispara mensagem de erro
//         DispararErro(ErroProcessamentoArquivo());
//     } else {
//         // Imprimir todos os registros(não há critério).
//         ExibirRegistrosDadoCriterio(arquivoEntrada, NULL);
//         // Fechar arquivo
//         fclose(arquivoEntrada);
//         arquivoEntrada = NULL;
//     }
//     // Liberar memória 
//     free(nomeArquivoEntrada);
//     nomeArquivoEntrada = NULL;

//     return;
// }

// /*
// SELECT_WHERE:
// Busca registros de acordo com um filtro
// */
// void FUNCIONALIDADE3(void){
//     // Ler nome do arquivo de entrada.
//     char *nomeArquivoEntrada = LerString();

//     // Abrir arquivos de entrada
//     FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb");

//     // Quantidades de buscas a serem realizadas
//     int quantBuscas;
//     scanf("%d", &quantBuscas);

//     // Verificar se arquivo de entrada existe
//     if(arquivoEntrada == NULL){
//         // Dispara mensagem de erro
//         DispararErro(ErroProcessamentoArquivo());
//     } else {
//         while(quantBuscas--){
//             // Le os criterios a serem avaliados
//             CRITERIO *criterio = DefinirCriterio();

//             ExibirRegistrosDadoCriterio(arquivoEntrada, criterio);

//             printf("**********\n");

//             // Apagar registro filtro
//             ApagarCriterio(&criterio);
//         }
//         // Fechar arquivo
//         fclose(arquivoEntrada);
//         arquivoEntrada = NULL;
//     }   

//     // Liberar memória 
//     free(nomeArquivoEntrada);
//     nomeArquivoEntrada = NULL;

//     return;
// }

// /*
// DELETE: 
// Remove registros logicamente de acordo com filtro 
// */
// void FUNCIONALIDADE4(void){
//     // Ler nome do arquivo de entrada.
//     char *nomeArquivoEntrada = LerString();

//     // Abrir arquivo de entrada para leitura e escrita
//     FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb+");

//     // Quantidades de remoções a serem realizadas
//     int quantRemove;
//     scanf("%d", &quantRemove);

//     // Verificar se arquivo de entrada existe
//     if(arquivoEntrada == NULL){
//         // Dispara mensagem de erro
//         DispararErro(ErroProcessamentoArquivo());
//     } else {
//         while(quantRemove--){
//             // Le os criterios a serem avaliados
//             CRITERIO *criterio = DefinirCriterio();

//             RemoverRegistroDadoCriterio(arquivoEntrada, criterio);
//             // Apagar registro criterio
//             ApagarCriterio(&criterio);
//         }
//         // Fechar arquivo
//         fclose(arquivoEntrada);
//         arquivoEntrada = NULL;

//         // Executar função fornecida para  
//         // mostrar a saída do arquivo ataques.bin
//         binarioNaTela(nomeArquivoEntrada);
//     }   

//     // Liberar memória 
//     free(nomeArquivoEntrada);
//     nomeArquivoEntrada = NULL;

//     return;
// }

// /* INSERT */
// void FUNCIONALIDADE5(void){
//     // Ler nome do arquivo de entrada.
//     char *nomeArquivoEntrada = LerString();

//     // Abrir arquivo de entrada para leitura e escrita
//     FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb+");

//     // Quantidades de remoções a serem realizadas
//     int quantInsert;
//     scanf("%d\n", &quantInsert);

//     // Verificar se arquivo de entrada existe
//     if(arquivoEntrada == NULL){
//         // Dispara mensagem de erro
//         DispararErro(ErroProcessamentoArquivo());
//     } else {
//         while(quantInsert--){
//             // Ler entrada da inserção.
//             REGISTRO *reg = CriarRegistroVazio();

//             // Ler campos fixos
//             scanf("%d", &reg->idAttack);
//             reg->year = LerCampoInteiro();
//             reg->financialLoss = LerCampoFloat();

//             // Ler Campos variáveis
//             reg->country = LerStringComAspas();
//             if(reg->country != NULL) 
//                 reg->tamanhoRegistro += strlen(reg->country) + 2;

//             reg->attackType = LerStringComAspas();
//             if(reg->attackType != NULL) 
//                 reg->tamanhoRegistro += strlen(reg->attackType) + 2;

//             reg->targetIndustry = LerStringComAspas();
//             if(reg->targetIndustry != NULL) 
//                 reg->tamanhoRegistro += strlen(reg->targetIndustry) + 2;

//             reg->defenseMechanism = LerStringComAspas();
//             if(reg->defenseMechanism != NULL) 
//                 reg->tamanhoRegistro += strlen(reg->defenseMechanism) + 2;

//             InserirRegistro(arquivoEntrada, reg);
//             ApagarRegistro(&reg);
//         }
//         // Fechar arquivo
//         fclose(arquivoEntrada);
//         arquivoEntrada = NULL;

//         // Executar função fornecida para  
//         // mostrar a saída do arquivo ataques.bin
//         binarioNaTela(nomeArquivoEntrada);
//     }   

//     // Liberar memória 
//     free(nomeArquivoEntrada);
//     nomeArquivoEntrada = NULL;

//     return;
// }

// /* UPDATE */
// void FUNCIONALIDADE6(void){
//     // Ler nome do arquivo de entrada.
//     char *nomeArquivoEntrada = LerString();

//     // Abrir arquivo de entrada para leitura e escrita
//     FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb+");

//     // Quantidades de remoções a serem realizadas
//     int quantUpdate;
//     scanf("%d\n", &quantUpdate);

//     // Verificar se arquivo de entrada existe
//     if(arquivoEntrada == NULL){
//         // Dispara mensagem de erro
//         DispararErro(ErroProcessamentoArquivo());
//     } else {
//         while(quantUpdate--){
//             // Le os criterios a serem avaliados
//             CRITERIO *criterio = DefinirCriterio();
//             CRITERIO *valoresAtualizados = DefinirCriterio();

//             AtualizarRegistroDadoCriterio(arquivoEntrada, criterio, valoresAtualizados);
           
//             // Apagar registro criterio
//             ApagarCriterio(&criterio);
//             ApagarCriterio(&valoresAtualizados);
//         }
//         // Fechar arquivo
//         fclose(arquivoEntrada);
//         arquivoEntrada = NULL;

//         // Executar função fornecida para  
//         // mostrar a saída do arquivo ataques.bin
//         binarioNaTela(nomeArquivoEntrada);
//     }   

//     // Liberar memória 
//     free(nomeArquivoEntrada);
//     nomeArquivoEntrada = NULL;

//     return;
// }

#include "indice.h"

#include <stdio.h>

int main(void) {
    FILE *arquivo = fopen("test.bin", "rb+");

    ARVB *arvb = CriarArvoreB(arquivo);
    printf("Árvore B criada.\n");
    // Inserção básica
    InserirArvoreB(arvb, 1, 100);
    // InserirArvoreB(arvb, 5, 500);
    // InserirArvoreB(arvb, 17, 1700);
    // InserirArvoreB(arvb, 230, 23000);
    // InserirArvoreB(arvb, 53, 5300);
    // InserirArvoreB(arvb, 79, 7900);

    ApagarArvoreB(&arvb);
    // // Testes de busca básicos
    // printf("Offset chave 1 (presente): %ld\n", BuscarChaveIndice(arvb, 1));
    // printf("Offset chave 5 (presente): %ld\n", BuscarChaveIndice(arvb, 5));
    // printf("Offset chave 15 (ausente): %ld\n", BuscarChaveIndice(arvb, 15)); 
    // printf("Offset chave 17 (presente): %ld\n", BuscarChaveIndice(arvb, 17));
    // printf("Offset chave 79 (presente): %ld\n", BuscarChaveIndice(arvb, 79));
    // printf("Offset chave 105 (ausente): %ld\n", BuscarChaveIndice(arvb, 105)); 

    // // Caso de duplicata — não deve inserir, pode exibir erro ou ignorar
    // printf("Tentando inserir duplicata 17 offset 7777\n");
    // InserirArvoreB(arvb, 17, 7777); // já existe
    // printf("Offset chave 17 (presente): %ld\n", BuscarChaveIndice(arvb, 17));

    // // Inserções em ordem crescente que forçam splits sucessivos
    // printf("Inserções em ordem crescente forçando splits...\n");
    // InserirArvoreB(arvb, 300, 30000);
    // InserirArvoreB(arvb, 400, 40000);
    // InserirArvoreB(arvb, 500, 50000);
    // InserirArvoreB(arvb, 600, 60000);

    // // Inserções em ordem decrescente
    // printf("Inserções em ordem decrescente...\n");
    // InserirArvoreB(arvb, 0, 10);

    // // Testes de busca após rebalanceamentos
    // printf("Offset chave 300 (presente): %ld\n", BuscarChaveIndice(arvb, 300));
    // printf("Offset chave 600 (presente): %ld\n", BuscarChaveIndice(arvb, 600));
    // printf("Offset chave 999 (ausente): %ld\n", BuscarChaveIndice(arvb, 999));

    fclose(arquivo);
    return 0;
}

