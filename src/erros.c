#include "erros.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct erro {
    char *mensagem;
    bool fatal;
};

ERRO *ErroBase(char *mensagem, bool fatal) {
    ERRO *erro = (ERRO*) malloc(sizeof(ERRO));

    if (erro != NULL) {
        erro->fatal = fatal;
        // Alocar espaço para a string e o \0
        int tamString = strlen(mensagem);
        erro->mensagem = (char*) malloc(sizeof(char)*(tamString + 1));
        if (erro->mensagem != NULL) {
            strcpy(erro->mensagem, mensagem);
            // Garantir \0 para evitar erros
            (erro->mensagem)[tamString] = '\0';
        }
    }  

    return erro;
}

ERRO *ErroPonteiroInvalido(void) {
    return ErroBase("Ponteiro nulo", true);
}

ERRO *ErroArquivoInvalido(void) {
    return ErroBase("Arquivo inválido", true);
}

ERRO *ErroArquivoVazio(void) {
    return ErroBase("Arquivo vazio", false);
}

ERRO *ErroProcessamentoArquivo(void) {
    return ErroBase("Falha no processamento do arquivo.", false);
}

ERRO *ErroRegistroInexistente(void) {
    return ErroBase("Registro inexistente.\n", false);
}

ERRO *ErroCriarArquivo(void){
    return ErroBase("Falha ao criar arquivo.", true);
}

void DispararErro(ERRO *erro) {
    if (erro == NULL) {
        fprintf(stderr, "Falha ao emitir erro!\n");
        exit(1);
    }

    bool fatal = erro->fatal;

    if (erro->mensagem != NULL) {
        printf("%s\n",  erro->mensagem);
        free(erro->mensagem);
        erro->mensagem = NULL;
    }

    free(erro);
    erro = NULL;

    if (fatal) {
        exit(1);
    }
}
