#include "indice.h"

#include "erros.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct promovido PROMOVIDO;
struct promovido {
    int chave;
    int offset;
    int rrn;
};

/*
Cria um cabeçalho de indice vazio.
*/
CABECALHO_INDICE *CriarCabecalhoIndicePadrão(void) {
    CABECALHO_INDICE *c = (CABECALHO_INDICE*) malloc(sizeof(CABECALHO_INDICE));

    if (c != NULL) {
        c->status = CONSISTENTE;
        c->noRaiz = -1;
        c->proxRRN = 0;
        c->nroNos = 0;
        memset(&c->lixo, LIXO, 31);
    }

    return c;
}

/*
Apaga o cabeçalho e libera a memória alocada.
*/
void ApagarCabecalhoIndice(CABECALHO_INDICE **c) {
    if (*c == NULL) {
        return;
    }

    free(*c);
    *c = NULL;
}

/*
Lê o cabeçalho do arquivo de índice
*/
CABECALHO_INDICE *LerCabecalhoIndice(FILE **arquivo) {
    if (*arquivo == NULL) {
        return NULL;
    }

    // Garante início do arquivo
    fseek(*arquivo, 0, SEEK_SET);

    CABECALHO_INDICE *c = (CABECALHO_INDICE*) malloc(sizeof(CABECALHO_INDICE));

    if (c != NULL) {
        fread(&(c->status), sizeof(char), 1, *arquivo);
        fread(&(c->noRaiz), sizeof(int), 1, *arquivo);
        fread(&(c->proxRRN), sizeof(int), 1, *arquivo);
        fread(&(c->nroNos), sizeof(int), 1, *arquivo);
        fread(c->lixo, sizeof(char), 31, *arquivo);
    }

    return c;
}

/*
Escreve os dados do cabeçãho no arquivo de índice.
*/
void EscreverCabecalhoIndice(FILE **arquivo, CABECALHO_INDICE *c) {
    if (*arquivo == NULL || c == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    // Garante início do arquivo
    fseek(*arquivo, 0, SEEK_SET);

    // Escrever dados
    fwrite(&(c->status), sizeof(char), 1, *arquivo);
    fwrite(&(c->noRaiz), sizeof(int), 1, *arquivo);
    fwrite(&(c->proxRRN), sizeof(int), 1, *arquivo);
    fwrite(&(c->nroNos), sizeof(int), 1, *arquivo);
    fwrite(c->lixo, sizeof(char), 31, *arquivo);
}

/*
Retorna o offset do nó a partir do inicio do arquivo.
Pula o cabeçalho.
*/
long _OffsetNo(int rrn) {
    return (rrn * TAM_REG_IND) + TAM_REG_IND;
}

/*
Retorna o offset do registro no arquivo de dados dada sua chave.
Caso a chave não esteja presente, retorna -1.
*/
long _OffsetChaveNo(NO_INDICE *no, int chave) {
    if (no == NULL) {
        return -1;
    }

    if (chave == no->C1) {
        return no->Pr1;
    } else if (chave == no->C2) {
        return no->Pr2;
    } else {
        return -1;
    }
}

/*
Cria um nó vazio para o indice.
O tipo do nó deve ser especificado.
*/
NO_INDICE *CriarNoIndice(int tipo) {
    NO_INDICE *no = (NO_INDICE*) malloc(sizeof(NO_INDICE));

    if (no != NULL) {
        no->tipoNo = tipo;
        no->nroChaves = 0;

        // Chave 1 e ptrs
        no->P1 = -1;
        no->C1 = -1;
        no->Pr1 = -1;

        // Chave 2 e ptrs
        no->P2 = -1;
        no->C2 = -1;
        no->Pr2 = -1;

        no->P3 = -1;
    }

    return no;
}

/*
Apaga um nó e liberá memória
*/
void ApagarNoIndice(NO_INDICE **no) {
    if (*no == NULL) {
        return;
    }

    free(*no);
    *no = NULL;
}

/*
Lê um nó do arquivo de índice.
*/
NO_INDICE *LerNoIndice(FILE **arquivo, int rrn) {
    if (*arquivo == NULL) {
        return NULL;
    }

    // Move para a posição do RRN se RRN != -1
    if (rrn != -1) {
        fseek(*arquivo, _OffsetNo(rrn), SEEK_SET);
    }

    // Cria um novo nó e preenche os valores
    NO_INDICE *no = (NO_INDICE*) malloc(sizeof(NO_INDICE));

    if (no != NULL) {
        fread(&(no->tipoNo), sizeof(int), 1, *arquivo);
        fread(&(no->nroChaves), sizeof(int), 1, *arquivo);

        // Chave 1 e ptrs
        fread(&(no->P1), sizeof(int), 1, *arquivo);
        fread(&(no->C1), sizeof(int), 1, *arquivo);
        fread(&(no->Pr1), sizeof(int), 1, *arquivo);

        // Chave 2 e ptrs
        fread(&(no->C2), sizeof(int), 1, *arquivo);
        fread(&(no->P2), sizeof(int), 1, *arquivo);
        fread(&(no->Pr2), sizeof(int), 1, *arquivo);

        fread(&(no->P3), sizeof(int), 1, *arquivo);
    }

    return no;
}

/*
Escreve os dados do nó no arquivo de índice.
*/
void EscreverNoIndice(FILE **arquivo, NO_INDICE *no, int rrn) {
    if (*arquivo == NULL || no == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    // Mover para a posição de inserção do nó
    fseek(*arquivo, _OffsetNo(rrn), SEEK_SET);

    fwrite(&(no->tipoNo), sizeof(int), 1, *arquivo);
    fwrite(&(no->nroChaves), sizeof(int), 1, *arquivo);

    // Chave 1 e ptrs
    fwrite(&(no->P1), sizeof(int), 1, *arquivo);
    fwrite(&(no->C1), sizeof(int), 1, *arquivo);
    fwrite(&(no->Pr1), sizeof(int), 1, *arquivo);

    // Chave 2 e ptrs
    fwrite(&(no->C2), sizeof(int), 1, *arquivo);
    fwrite(&(no->P2), sizeof(int), 1, *arquivo);
    fwrite(&(no->Pr2), sizeof(int), 1, *arquivo);

    fwrite(&(no->P3), sizeof(int), 1, *arquivo);
}

/*
Insere a nova chave especificada e ordena (se necessário).
Não faz verificação do tipo de nó (isso deve ser verificado antes).
*/
void _InserirChaveNo(NO_INDICE *no, int chave, long offset) {
    if (no == NULL) {
        return;
    }

    // Ignora chave repetida
    if (no->C1 == chave || no->C2 == chave) {
        return;
    }

    // No vazio
    if (no->nroChaves == 0) {
        no->C1 = chave;
        no->Pr1 = offset;
    } else {
        // Chave atual menor que C1
        if (chave < no->C1) {
            no->C2 = no->C1;
            no->Pr2 = no->Pr1;

            no->C1 = chave;
            no->Pr1 = offset;
        } else {
            // Chave maior que C1
            no->C2 = chave;
            no->Pr2 = offset;
        }
    }

    no->nroChaves++;
}

/*
Busca a posição da chave no nó.
Armazena em *pos a posição da chave (ou a posição que ela deveria estar)
*/
bool _BuscarPosicaoChave(NO_INDICE *no, int chave, int *pos) {
    if (chave == no->C1) {
        *pos = 1; // C1 ou P1
        return true;
    }

    if (chave == no->C2) {
        *pos = 2; // C2 ou P2
        return true;
    }

    *pos = 3; // P3
    return false;
}

/*
Retorna o filho que deve ser buscado a seguir usando seu numero (1, 2 ou 3).
*/
int _ProxNo(NO_INDICE *no, int pos) {
    if (pos == 1) {
        return no->P1;
    } else if (pos == 2) {
        return no->P2;
    } else {
        return no->P3;
    }
}

/*
Busca recursivamente o nó do índice que possui (ou deveria possuir) a dada chave.
Inicia a busca no rrn especificado (por padrão, começa no nó raiz)
Salva a posição da chave e o rrn do nó nos ponteiros passados como parâmetro.
*/
bool _BuscarNo(FILE **arquivo, int chave, int rrn, int *rrn_achou, int *pos_achou) {
    // Ponteiro nulo, sem nó filho
    if (rrn == -1) {
        return false;
    }

    // Move o arquivo para o proximo nó a ser lido
    NO_INDICE *no = LerNoIndice(arquivo, rrn);
    if (no == NULL) {
        return false;
    }

    // Verificar se chave está no nó
    if (_BuscarPosicaoChave(no, chave, pos_achou)) {
        *rrn_achou = rrn;
        return true;
    }

    // Caso não tenha encontrado, guia para o próximo nó (pos_achou)
    rrn = _ProxNo(no, *pos_achou);

    // Liberar memória
    ApagarNoIndice(&no);

    return _BuscarNo(arquivo, chave, rrn, rrn_achou, pos_achou);
}

/*
Executa a função de split da arvore B no índice.
*/
void _Split(FILE **arquivo, CABECALHO_INDICE *c, int chave, long offset, PROMOVIDO *promo, int rrn, NO_INDICE *atual, NO_INDICE **novo) {
    // Armazenar estados
    int chaves[3] = { atual->C1, atual->C2, -1};
    int offsets[3] = { atual->Pr1, atual->Pr2, -1 };
    int ptrs[MAX_PTRS + 2] = { atual->P1, atual->P2, atual->P3, -1, -1 };

    // Ordenar e inserir nova chave na lista
    int i;
    for (i = MAX_CHAVES; chave < chaves[i-1] && i > 0; i--) {
        chaves[i] = chaves[i-1];
        offsets[i] = offsets[i-1];
        ptrs[i+1] = ptrs[i];
    }

    chaves[i] = chave;
    offsets[i] = offset;
    ptrs[i+1] = rrn;

    // Inicializar novo nó
    *novo = CriarNoIndice(INTERMEDIARIO);
    
    // Ajustar valores
    atual->C1 = chaves[0];
    atual->P1 = ptrs[0];
    atual->Pr1 = offsets[0];

    (*novo)->C1 = chaves[2];
    (*novo)->Pr1 = offsets[2];
    (*novo)->P1 = ptrs[2];
    (*novo)->P2 = ptrs[3];
    (*novo)->C2 = -1;
    (*novo)->Pr2 = -1;
    (*novo)->P3 = -1;

    promo->rrn = c->proxRRN;
    promo->chave = chaves[1];
    promo->offset = offsets[1];

    // Ajustar cabeçalho
    c->proxRRN++;
}

/*
Busca a posição e insere um nova chave no indice.
Caso seja necessário, um split é feito
*/
bool _InserirChave(FILE **arquivo, CABECALHO_INDICE *c, int chave, long offset, int rrn, PROMOVIDO *promo) {
    // Chave será inserida em nó folha
    if (rrn == -1) {
        promo->chave = chave;
        promo->rrn = -1;

        return true;
    }

    // Buscar nó e verificar chave
    int rrn_achou = 0;
    int pos_achou = 0;
    if (_BuscarNo(arquivo, chave, rrn, &rrn_achou, &pos_achou)) {
        return false; // chave duplicada
    }

    // Armazenar proximo RRN a ser visistado
    NO_INDICE *no = LerNoIndice(arquivo, rrn);
    
    bool promovido = _InserirChave(arquivo, c, chave, offset, _ProxNo(no, pos_achou), promo);
    if (!promovido) {
        return false;
    }

    if (no->nroChaves < MAX_CHAVES) {
        _InserirChaveNo(no, chave, offset);
        EscreverNoIndice(arquivo, no, rrn);
        ApagarNoIndice(&no);

        return false;
    } else {
        NO_INDICE *novo = NULL;
        _Split(arquivo, c, chave, offset, promo, 0, no, &novo);

        EscreverNoIndice(arquivo, no, rrn);
        EscreverNoIndice(arquivo, novo, promo->rrn);
        ApagarNoIndice(&no);
        ApagarNoIndice(&novo);

        return true;
    }
}

/*
Insere a nova chave e o offset do arquivo de dados no índice.
Mantém o índice correto usando operações de split.
*/
void AdicionarChave(FILE **arquivo, int chave, long offset) {
    if (*arquivo == NULL) {
        DispararErro(ErroArquivoInvalido());
    }

    // Ler cabeçalho e alterar status
    CABECALHO_INDICE *c = LerCabecalhoIndice(arquivo);
    if (c == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    c->status = INCONSISTENTE;
    EscreverCabecalhoIndice(arquivo, c);

    // Realizar inserção e gerenciar promoção
    PROMOVIDO *promo = (PROMOVIDO*) malloc(sizeof(PROMOVIDO));
    if (promo == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    bool promovido = _InserirChave(arquivo, c, chave, offset, _OffsetNo(c->noRaiz), promo);
    if (promovido) {
        // Atualizar raiz
        NO_INDICE *raiz = CriarNoIndice(RAIZ);

        // Ajustar valores
        raiz->C1 = promo->chave;
        raiz->Pr1 = promo->offset;

        // Ajustar filhos
        raiz->P1 = c->noRaiz;
        raiz->P2 = promo->rrn;

        // Salvar mudanças
        EscreverNoIndice(arquivo, raiz, c->proxRRN);
        ApagarNoIndice(&raiz);

        // Ajustar cabeçalho
        c->noRaiz = c->proxRRN;
        c->proxRRN++;
    }

    // Atualizar cabeçalho
    c->status = CONSISTENTE;
    EscreverCabecalhoIndice(arquivo, c);
    ApagarCabecalhoIndice(&c);
}

/*
Remove a chave informada e retorna o offset do arquivo de dados.
*/
long RemoverChave(FILE **arquivo, int chave) {
    return -1;
}


/*
Retorna o byte offset da dada chave no arquivo de dados.
Caso a chave não exista, retorna -1.
*/
long BuscarChave(FILE **arquivo, int chave) {
    if (*arquivo == NULL) {
        DispararErro(ErroArquivoInvalido());
    }

    // Ler cabeçalho e alterar status
    CABECALHO_INDICE *c = LerCabecalhoIndice(arquivo);
    if (c == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    int rrn = 0; // RRN do nó achado
    int pos = 0; // Posição da chave (C1 ou C2)
    int offset = -1;
    
    if (_BuscarNo(arquivo, chave, c->noRaiz, &rrn, &pos)) {
        NO_INDICE *no = LerNoIndice(arquivo, rrn);

        if (no != NULL) {
            offset = pos == 1 ? no->C1 : no->C2;

            ApagarNoIndice(&no);
        }
    }

    ApagarCabecalhoIndice(&c);
    
    return offset;
}