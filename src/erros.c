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

        erro->mensagem = (char*) malloc(strlen(mensagem));
        if (erro->mensagem != NULL) {
            strcpy(erro->mensagem, mensagem);
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
    return ErroBase("Registro Inexistente.", false);
}

void DispararErro(ERRO *erro) {
    if (erro == NULL) {
        fprintf(stderr, "Algo deu errado!\n");
        exit(1);
    }

    bool fatal = erro->fatal;

    if (erro->mensagem != NULL) {
        fprintf(stderr, "%s\n",  erro->mensagem);
        free(erro->mensagem);
    }

    free(erro);

    if (fatal) {
        exit(1);
    }
}
