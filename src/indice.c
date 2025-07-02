#include "indice.h"

#include "erros.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
Cria um cabeçalho de indice vazio.
*/
CABECALHO_INDICE *CriarCabecalhoIndicePadrao(void) {
    CABECALHO_INDICE *c = (CABECALHO_INDICE*) malloc(sizeof(CABECALHO_INDICE));

    if (c != NULL) {
        c->status = CONSISTENTE;
        c->noRaiz = -1;
        c->proxRRN = 0;
        c->nroNos = 0;
        memset(&c->lixo, LIXO, 31);
    } else {
        DispararErro(ErroAlocacaoMemoria());
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
        DispararErro(ErroPonteiroInvalido());
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
    } else {
        DispararErro(ErroAlocacaoMemoria());
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

    long offset = _OffsetNo(rrn);

    // Verifica o tamanho do arquivo
    fseek(*arquivo, 0, SEEK_END);
    long tamanho = ftell(*arquivo);

    if (offset + TAM_REG_IND > tamanho) {
        // O nó solicitado vai além do fim do arquivo
        return NULL;
    }

    fseek(*arquivo, offset, SEEK_SET);

    // Cria um novo nó e preenche os valores
    NO_INDICE *no = (NO_INDICE*) malloc(sizeof(NO_INDICE));

    if (no != NULL) {
        fread(&(no->tipoNo), sizeof(int), 1, *arquivo);
        fread(&(no->nroChaves), sizeof(int), 1, *arquivo);

        // Chave 1 e ptrs
        fread(&(no->P1), sizeof(int), 1, *arquivo);
        fread(&(no->C1), sizeof(int), 1, *arquivo);
        fread(&(no->Pr1), sizeof(long), 1, *arquivo);

        // Chave 2 e ptrs
        fread(&(no->P2), sizeof(int), 1, *arquivo);
        fread(&(no->C2), sizeof(int), 1, *arquivo);
        fread(&(no->Pr2), sizeof(long), 1, *arquivo);

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
    fwrite(&(no->Pr1), sizeof(long), 1, *arquivo);

    // Chave 2 e ptrs
    fwrite(&(no->P2), sizeof(int), 1, *arquivo);
    fwrite(&(no->C2), sizeof(int), 1, *arquivo);
    fwrite(&(no->Pr2), sizeof(long), 1, *arquivo);

    fwrite(&(no->P3), sizeof(int), 1, *arquivo);

    fflush(*arquivo);
}

/*
Insere a nova chave especificada e ordena (se necessário).
Não faz verificação do tipo de nó (isso deve ser verificado antes).
*/
void _InserirChaveNo(NO_INDICE *no, int chave, long offset, int rrn_dir) {
    if (no == NULL || no->nroChaves == MAX_CHAVES) {
        return;
    }

    // Ignora chave repetida
    if (no->C1 == chave || no->C2 == chave) {
        return;
    }

        // Vetores temporários para reorganização
    int chaves[3] = { -1, -1, -1 };
    long offsets[3] = { -1, -1, -1 };
    int filhos[4] = { no->P1, no->P2, no->P3, -1 };

    // Copia chaves e offsets existentes
    if (no->nroChaves >= 1) {
        chaves[0] = no->C1;
        offsets[0] = no->Pr1;
    }
    if (no->nroChaves == 2) {
        chaves[1] = no->C2;
        offsets[1] = no->Pr2;
    }

    // Inserção ordenada
    int i = no->nroChaves;
    while (i > 0 && chave < chaves[i - 1]) {
        chaves[i] = chaves[i - 1];
        offsets[i] = offsets[i - 1];
        if (no->tipoNo != FOLHA) {
            filhos[i + 1] = filhos[i];
        }
           
        i--;
    }

    chaves[i] = chave;
    offsets[i] = offset;
    if (no->tipoNo != FOLHA) {
        filhos[i + 1] = rrn_dir;
    }
        
    // Atualiza estrutura do nó
    no->nroChaves++;

    // Atribui chaves e offsets de volta ao nó
    no->C1 = chaves[0];
    no->Pr1 = offsets[0];

    if (no->nroChaves > 1) {
        no->C2 = chaves[1];
        no->Pr2 = offsets[1];
    } else {
        no->C2 = -1;
        no->Pr2 = -1;
    }

    // Atualiza ponteiros apenas se não for folha
    if (no->tipoNo != FOLHA) {
        no->P1 = filhos[0];
        no->P2 = filhos[1];
        no->P3 = (no->nroChaves == 2) ? filhos[2] : -1;
    } else {
        no->P1 = no->P2 = no->P3 = -1;
    }
}

/*
Atualiza os ponteiros quando ocrre uma nova inserção.
*/
void _AtualizarFilhos(NO_INDICE *no, int novoPtr) {
    if (no == NULL) {
        return;
    }

    if (no->nroChaves == 1) {
        // Dois ponteiros apenas
        if (no->C1 != -1 && novoPtr < no->C1) {
            no->P2 = no->P1;
            no->P1 = novoPtr;
        } else {
            no->P2 = novoPtr;
        }
        no->P3 = -1;
    } else if (no->nroChaves == 2) {
        // Três ponteiros
        int c1 = no->C1;
        int c2 = no->C2;

        if (novoPtr < c1) {
            no->P3 = no->P2;
            no->P2 = no->P1;
            no->P1 = novoPtr;
        } else if (novoPtr < c2) {
            no->P3 = no->P2;
            no->P2 = novoPtr;
        } else {
            no->P3 = novoPtr;
        }
    }
}

/*
Retorna o filho que deve ser buscado a seguir usando seu numero (1, 2 ou 3).
*/
int _ProxRRN(NO_INDICE *no, int pos) {
    switch (pos) {
        case PTR_ESQ: return no->P1;
        case PTR_MED: return no->P2;
        case PTR_DIR: return no->P3;
        default: return -1;
    }
}

/*
Decide qual caminho seguir dada uma chave
*/
int _ProxFilho(NO_INDICE *no, int chave) {
    if (chave < no->C1) {
        return PTR_ESQ;
    } else if (no->nroChaves == 1 || (no->nroChaves == 2 && no->C2 != -1 && chave < no->C2)) {
        return PTR_MED;
    } else {
        return PTR_DIR;
    }
}

/*
Busca recursivamente o nó do índice que possui (ou deveria possuir) a dada chave.
Inicia a busca no rrn especificado (por padrão, começa no nó raiz)
Salva a posição da chave e o rrn do nó nos ponteiros passados como parâmetro.
*/
bool _BuscarNo(FILE **arquivo, int chave, int rrn, int *rrn_achou, int *pos_achou) {
    if (rrn == -1) {
        return false;
    }

    NO_INDICE *no = LerNoIndice(arquivo, rrn);
    if (no == NULL) {
        return false;
    }

    if (chave == no->C1) {
        *rrn_achou = rrn;
        *pos_achou = 0; // C1
        ApagarNoIndice(&no);
        return true;
    }

    if (chave == no->C2) {
        *rrn_achou = rrn;
        *pos_achou = 1; // C2
        ApagarNoIndice(&no);
        return true;
    }

    // Decide em qual filho descer
    int prox_rrn = _ProxRRN(no, _ProxFilho(no, chave));

    ApagarNoIndice(&no);
    return _BuscarNo(arquivo, chave, prox_rrn, rrn_achou, pos_achou);
}

/*
Executa a função de split da arvore B no índice.
*/
void _Split(FILE **arquivo, 
            CABECALHO_INDICE *c, 
            int chave, long offset, 
            PROMOVIDO *promo, 
            int rrn, NO_INDICE 
            *atual, 
            NO_INDICE **novo) 
{
    int chaves[MAX_CHAVES + 1] = { atual->C1, atual->C2, chave };
    long offsets[MAX_CHAVES + 1] = { atual->Pr1, atual->Pr2, offset };
    int ptrs[MAX_PTRS + 1] = { atual->P1, atual->P2, atual->P3, rrn };

    // Ordenar chaves (offset acompanha)
    for (int i = 0; i < MAX_CHAVES; i++) {
        for (int j = i + 1; j < MAX_CHAVES + 1; j++) {
            if (chaves[i] > chaves[j]) {
                // swap chave
                int ctmp = chaves[i];
                chaves[i] = chaves[j];
                chaves[j] = ctmp;

                // swap offset
                long otmp = offsets[i];
                offsets[i] = offsets[j];
                offsets[j] = otmp;
            }
        }
    }

    // Criar novo nó (direita)
    *novo = CriarNoIndice(atual->tipoNo);

    // Dividir os dados entre atual e novo
    atual->C1 = chaves[0];
    atual->Pr1 = offsets[0];
    atual->C2 = -1;
    atual->Pr2 = -1;
    atual->nroChaves = 1;

    (*novo)->C1 = chaves[2];
    (*novo)->Pr1 = offsets[2];
    (*novo)->C2 = -1;
    (*novo)->Pr2 = -1;
    (*novo)->nroChaves = 1;

    if (atual->tipoNo == FOLHA) {
        // Folha não tem filhos
        atual->P1 = -1;
        atual->P2 = -1;
        atual->P3 = -1;
        (*novo)->P1 = -1;
        (*novo)->P2 = -1;
        (*novo)->P3 = -1;
    } else {
        // Reorganizar filhos
        atual->P1 = ptrs[0];
        atual->P2 = ptrs[1];
        atual->P3 = -1;

        (*novo)->P1 = ptrs[2];
        (*novo)->P2 = ptrs[3];
        (*novo)->P3 = -1;
    }

    // Promove chave do meio
    promo->chave = chaves[1];
    promo->offset = offsets[1];
    promo->rrn = c->proxRRN;  // RRN do novo nó

    // Atualizar cabeçalho do índice
    c->proxRRN++;
    c->nroNos++;
}

/*
Busca a posição e insere um nova chave no indice.
Caso seja necessário, um split é feito
*/
bool _InserirChave(FILE **arquivo, 
                CABECALHO_INDICE *c, 
                int chave, 
                long offset, 
                int rrn, 
                PROMOVIDO *promo) 
{
    if (rrn == -1) {
        // Caso base: nó filho inexistente, deve criar nova folha promovendo chave para o pai
        promo->chave = chave;
        promo->offset = offset;
        promo->rrn = c->proxRRN;

        // Criar folha no disco
        NO_INDICE *folha = CriarNoIndice(FOLHA);
        folha->C1 = chave;
        folha->Pr1 = offset;
        folha->nroChaves = 1;

        EscreverNoIndice(arquivo, folha, promo->rrn);
        ApagarNoIndice(&folha);

        c->proxRRN++;
        c->nroNos++;

        return true; // indica promoção para o nível superior
    }

    NO_INDICE *no = LerNoIndice(arquivo, rrn);
    if (no == NULL) {
        return false; // erro
    }

    // Verifica chave duplicada
    if (no->C1 == chave || no->C2 == chave) {
        ApagarNoIndice(&no);
        return false; // erro: duplicata
    }

    if (no->tipoNo == FOLHA) {
        // Caso nó folha
        if (no->nroChaves < MAX_CHAVES) {
            // Insere direto na folha
            _InserirChaveNo(no, chave, offset, -1);
            EscreverNoIndice(arquivo, no, rrn);
            ApagarNoIndice(&no);

            return false; // sem promoção para cima
        } else {
            // Faz split da folha
            NO_INDICE *novo = NULL;
            _Split(arquivo, c, chave, offset, promo, -1, no, &novo);

            EscreverNoIndice(arquivo, no, rrn);
            EscreverNoIndice(arquivo, novo, promo->rrn);

            ApagarNoIndice(&no);
            ApagarNoIndice(&novo);

            return true; // promoção para o nível superior
        }
    } else {
        // Nó intermediário — descer recursivamente no filho correto
        int pos = _ProxFilho(no, chave);

        PROMOVIDO promo_filho;
        bool promovido = _InserirChave(arquivo, c, chave, offset, _ProxRRN(no, pos), &promo_filho);

        if (!promovido) {
            ApagarNoIndice(&no);
            return false; // sem promoção do filho ou erro
        }

        // Se tem espaço no nó atual, insere chave promovida e atualiza filho
        if (no->nroChaves < MAX_CHAVES) {
            _InserirChaveNo(no, promo_filho.chave, promo_filho.offset, promo_filho.rrn);
            EscreverNoIndice(arquivo, no, rrn);
            ApagarNoIndice(&no);

            return false; // sem promoção para nível superior
        }

        // Nó cheio — faz split e promove chave
        NO_INDICE *novo = NULL;
        _Split(arquivo, c, promo_filho.chave, promo_filho.offset, promo, promo_filho.rrn, no, &novo);

        EscreverNoIndice(arquivo, no, rrn);
        EscreverNoIndice(arquivo, novo, promo->rrn);

        ApagarNoIndice(&no);
        ApagarNoIndice(&novo);

        return true; // promoção para nível superior
    }
}

/*
Insere a nova chave e o offset do arquivo de dados no índice.
Mantém o índice correto usando operações de split.
*/
void AdicionarChaveIndice(FILE **arquivo, int chave, long offset) {
    if (*arquivo == NULL) {
        DispararErro(ErroArquivoInvalido());
    }

    // Ler cabeçalho
    CABECALHO_INDICE *c = LerCabecalhoIndice(arquivo);
    if (c == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    if (c->status == INCONSISTENTE) {
        DispararErro(ErroProcessamentoArquivo());
    }

    // Realizar inserção e gerenciar promoção
    PROMOVIDO *promo = (PROMOVIDO*) malloc(sizeof(PROMOVIDO));
    if (promo == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    // Marcar inconsistente
    c->status = INCONSISTENTE;
    EscreverCabecalhoIndice(arquivo, c);

    bool promovido = _InserirChave(arquivo, c, chave, offset, c->noRaiz, promo);

    if (c->noRaiz == -1) {
        // primeira inserção cria raiz
        c->noRaiz = promo->rrn;

        // Atualizar tipo da folha recém-criada
        NO_INDICE *no = LerNoIndice(arquivo, promo->rrn);
        if (no != NULL) {
            no->tipoNo = RAIZ;
            EscreverNoIndice(arquivo, no, promo->rrn);
            ApagarNoIndice(&no);
        }

    } else if (promovido) {
        // Criar nova raiz
        NO_INDICE *novaRaiz = CriarNoIndice(RAIZ);
        novaRaiz->C1 = promo->chave;
        novaRaiz->Pr1 = promo->offset;
        novaRaiz->P1 = c->noRaiz; // raiz antiga
        novaRaiz->P2 = promo->rrn; // novo nó promovido
        novaRaiz->nroChaves = 1;

        // Escrever nova raiz
        EscreverNoIndice(arquivo, novaRaiz, c->proxRRN);

        // Atualizar tipo dos filhos antigos para INTERMEDIARIO
        NO_INDICE *filhoEsq = LerNoIndice(arquivo, novaRaiz->P1);
        NO_INDICE *filhoDir = LerNoIndice(arquivo, novaRaiz->P2);

        if (filhoEsq != NULL) {
            filhoEsq->tipoNo = INTERMEDIARIO;
            EscreverNoIndice(arquivo, filhoEsq, novaRaiz->P1);
            ApagarNoIndice(&filhoEsq);
        }

        if (filhoDir != NULL) {
            filhoDir->tipoNo = INTERMEDIARIO;
            EscreverNoIndice(arquivo, filhoDir, novaRaiz->P2);
            ApagarNoIndice(&filhoDir);
        }

        // Atualizar cabeçalho
        c->noRaiz = c->proxRRN;
        c->proxRRN++;
        c->nroNos++;

        ApagarNoIndice(&novaRaiz);
    }

    // Atualizar cabeçalho
    c->status = CONSISTENTE;
    EscreverCabecalhoIndice(arquivo, c);
    ApagarCabecalhoIndice(&c);

    // Liberar mem
    free(promo);
}

/*
Remove a chave informada e retorna o offset do arquivo de dados.
*/
long RemoverChaveIndice(FILE **arquivo, int chave) {
    if (*arquivo == NULL) {
        DispararErro(ErroArquivoInvalido());
    }

    // Ler cabeçalho e alterar status
    CABECALHO_INDICE *c = LerCabecalhoIndice(arquivo);
    if (c == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    if (c->status == INCONSISTENTE) {
        DispararErro(ErroProcessamentoArquivo());
    }

    c->status = INCONSISTENTE;
    EscreverCabecalhoIndice(arquivo, c);

    // Realizar remoção e despromoção de chave (se necessário)


    // Atualizar cabeçalho
    c->status = CONSISTENTE;
    EscreverCabecalhoIndice(arquivo, c);
    ApagarCabecalhoIndice(&c);

    return -1;
}

/*
Retorna o byte offset da dada chave no arquivo de dados.
Caso a chave não exista, retorna -1.
*/
long BuscarChaveIndice(FILE **arquivo, int chave) {
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
    long offset = -1;
    
    if (_BuscarNo(arquivo, chave, c->noRaiz, &rrn, &pos)) {
        NO_INDICE *no = LerNoIndice(arquivo, rrn);

        if (no != NULL) {
            offset = (pos == 0) ? no->Pr1 : no->Pr2;

            ApagarNoIndice(&no);
        }
    }

    ApagarCabecalhoIndice(&c);
    
    return offset;
}

/*
Abre o arquivo especificado e salva no ponteiro para FILE.
Caso seja um índice vazio, inicializa o cabeçalho padrão.
*/
void AbrirIndice(FILE **arquivo, char *nome) {
    *arquivo = fopen(nome, "rb+");
    if (*arquivo == NULL) {
        // Não existe
        *arquivo = fopen(nome, "wb+");
        if (*arquivo == NULL) {
            DispararErro(ErroArquivoInvalido());
        }
    }

    // Verificar se tem algo no arquivo (tam = 0)
    fseek(*arquivo, 0, SEEK_END);
    long tamanho = ftell(*arquivo);

    if (tamanho == 0) {
        CABECALHO_INDICE *c = CriarCabecalhoIndicePadrao();
        NO_INDICE *raiz = CriarNoIndice(FOLHA);

        c->noRaiz = 0;
        c->nroNos++;
        c->proxRRN++;

        EscreverCabecalhoIndice(arquivo, c);
        ApagarCabecalhoIndice(&c);

        EscreverNoIndice(arquivo, raiz, 0);
        ApagarNoIndice(&raiz);

        fflush(*arquivo);
    }

    fseek(*arquivo, 0, SEEK_SET);
}