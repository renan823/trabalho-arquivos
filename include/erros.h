#ifndef ERROS_H_
    #define ERROS_H_

    typedef struct erro ERRO;

    ERRO *ErroPonteiroInvalido(void);
    ERRO *ErroArquivoInvalido(void);
    ERRO *ErroArquivoVazio(void);
    ERRO *ErroProcessamentoArquivo(void);
    ERRO *ErroRegistroInexistente(void);
    ERRO *ErroCriarArquivo(void);

    void DispararErro(ERRO *erro);
#endif