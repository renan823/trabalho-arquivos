#include "indice.h"

#include <stdio.h>

int main(void) {
    FILE *arquivo = fopen("indice.bin", "rb+");

    CABECALHO_ARVOREB *c = CriarCabecalhoIndicePadrao();
    EscreverCabecalhoIndice(&arquivo, c);
    ApagarCabecalhoIndice(&c);

    ARVB *arvb = CriarArvoreB(arquivo);

    // Inserção básica
    InserirArvoreB(arvb, 1, 100);
    InserirArvoreB(arvb, 5, 500);
    InserirArvoreB(arvb, 17, 1700);
    InserirArvoreB(arvb, 230, 23000);
    InserirArvoreB(arvb, 53, 5300);
    InserirArvoreB(arvb, 79, 7900);

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
