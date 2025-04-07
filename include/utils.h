#ifndef UTILS_H_
    #define UTILS_H_
    
    #include<stdio.h>

    // Retorna tamanho em bytes da linha a ser lida(0 caso chegue no final)
    char *PegarLinha(char **buffer, int tamBuffer,FILE *arquivoAberto);

    // Função lê uma string sem desperdício de memória
    char *LerString(void);

    // Função para imprimir dados salvos no arquivo em binário
    // (util para comparar saida no run codes)
    void binarioNaTela(char *nomeArquivoBinario);

    // Lê conteúdo da string entre "" e salva
    void scan_quote_string(char *str);

    // Lê string com aspas
    char *LerStringComAspas(void);

#endif