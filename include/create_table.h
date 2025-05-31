#ifndef CREATE_TABLE_H_
    #define CREATE_TABLE_H_
    
    #include<stdio.h>
    #include<cabecalho.h>
    #include<registro.h>

    // Ler arquivo CSV e escreve em binário.
    void LerCsvParaBinario(FILE *arquivoEntrada, FILE *arquivoSaida);

#endif