#ifndef UTILS_H_
    #define UTILS_H_

    #define INCONSISTENTE '0'
    #define CONSISTENTE '1'
    #define LIXO '$'
    
    #include<stdio.h>
    #include<cabecalho.h>
    #include<registro.h>

    // Retorna tamanho em bytes da linha a ser lida(0 caso chegue no final)
    char *PegarLinha(char **buffer, int tamBuffer,FILE *arquivoAberto);

    // Ler arquivo CSV e escreve em binário.
    void LerCsvParaBinario(FILE *arquivoEntrada, FILE *arquivoSaida);

    // Função lê uma string sem desperdício de memória
    char *LerString(void);

    // Função para imprimir dados salvos no arquivo em binário
    // (util para comparar saida no run codes)
    void binarioNaTela(char *nomeArquivoBinario);

    // Lê conteúdo da string entre "" e salva
    void scan_quote_string(char *str);

    // Lê string com aspas.
    char *LerStringComAspas(void);

    /*Funções de leitura para FUNC5*/
    // Le uma String e converte em inteiro.
    int LerCampoInteiro(void);
    // Le uma String e converte em float.
    float LerCampoFloat(void);
    // Le String com aspas e retorna NULL se campo NULO.
    char *LerCampoStringComAspas(void); 

    void FecharArquivo(FILE **arquivo);
#endif