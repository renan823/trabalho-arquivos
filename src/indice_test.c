#include "indice.h"

#include <stdio.h>

int main(void) {
    FILE *arquivo = NULL;

    // Remova arquivo antigo se quiser sempre testar do zero
    remove("indice.bin");
    AbrirIndice(&arquivo, "indice.bin");

    // Inserção básica
    AdicionarChaveIndice(&arquivo, 1, 100);
    AdicionarChaveIndice(&arquivo, 5, 500);
    AdicionarChaveIndice(&arquivo, 17, 1700);
    AdicionarChaveIndice(&arquivo, 230, 23000);
    AdicionarChaveIndice(&arquivo, 53, 5300);
    AdicionarChaveIndice(&arquivo, 79, 7900);

    // Testes de busca básicos
    printf("Offset chave 1 (presente): %ld\n", BuscarChaveIndice(&arquivo, 1));
    printf("Offset chave 5 (presente): %ld\n", BuscarChaveIndice(&arquivo, 5));
    printf("Offset chave 15 (ausente): %ld\n", BuscarChaveIndice(&arquivo, 15)); 
    printf("Offset chave 17 (presente): %ld\n", BuscarChaveIndice(&arquivo, 17));
    printf("Offset chave 79 (presente): %ld\n", BuscarChaveIndice(&arquivo, 79));
    printf("Offset chave 105 (ausente): %ld\n", BuscarChaveIndice(&arquivo, 105)); 

    // Caso de duplicata — não deve inserir, pode exibir erro ou ignorar
    printf("Tentando inserir duplicata 17 offset 7777\n");
    AdicionarChaveIndice(&arquivo, 17, 7777); // já existe
    printf("Offset chave 17 (presente): %ld\n", BuscarChaveIndice(&arquivo, 17));

    // Inserções em ordem crescente que forçam splits sucessivos
    printf("Inserções em ordem crescente forçando splits...\n");
    AdicionarChaveIndice(&arquivo, 300, 30000);
    AdicionarChaveIndice(&arquivo, 400, 40000);
    AdicionarChaveIndice(&arquivo, 500, 50000);
    AdicionarChaveIndice(&arquivo, 600, 60000);

    // Inserções em ordem decrescente
    printf("Inserções em ordem decrescente...\n");
    AdicionarChaveIndice(&arquivo, 0, 10);

    // Testes de busca após rebalanceamentos
    printf("Offset chave 300 (presente): %ld\n", BuscarChaveIndice(&arquivo, 300));
    printf("Offset chave 600 (presente): %ld\n", BuscarChaveIndice(&arquivo, 600));
    printf("Offset chave 999 (ausente): %ld\n", BuscarChaveIndice(&arquivo, 999));

    fclose(arquivo);
    return 0;
}
