#include<stdlib.h>
#include<string.h>

#include "SQL.h"
#include "erros.h"
#include "registro.h"
#include "utils.h"

/* Protótipos */
char *_PegarLinha(char **buffer, int tamBuffer,FILE *arquivoAberto);
void _PreencherCampoVariavel(char **campo, char **linha, int *tamVariaveis);
void _PreencherRegistro(REGISTRO **reg, char *buffer);

/* 
Recebe arquivos abertos(entrada e saida),
lê arquivo CSV e escreve em binário. 
*/
void LerCsvParaBinario(FILE *arquivoEntrada, FILE *arquivoSaida){
	// Escreve cabecalho em binario no arquivo de saida
	CABECALHO *cabecalho =  CriarCabecalhoPadrao();
	EscreverCabecalho(&arquivoSaida, cabecalho);
	ApagarCabecalho(&cabecalho);

	// Buffer para armazenar linha do CSV
	int tamBuffer = 256;
	char *buffer = (char*) malloc(sizeof(char)*tamBuffer);

	// Eliminar introdução do arquivo CSV
	_PegarLinha(&buffer, tamBuffer, arquivoEntrada);

	// número de registros não removidos presentes no arquivo
	int nroRegArq = 0;

	// Retorna tamanho da linha e não altera o ponteiro.
	// Se o tamanho da linha for zero, não há dados a serem escritos mais(fim do arquivo).
	while(_PegarLinha(&buffer, tamBuffer, arquivoEntrada)){
			// Adicionar numero de registros não removidos no arquivo.
			nroRegArq++;
			
			// Preencher registro passando buffer
			REGISTRO *reg = CriarRegistroVazio();
			_PreencherRegistro(&reg, buffer);
			// Inserir registro em arquivo binário        
			EscreverRegistro(&arquivoSaida, reg);

			// Apagar registro
			ApagarRegistro(&reg);
	};
	
	// Guardar próximo byteOffSet disponível
	long int proxByteOffset = ftell(arquivoSaida);

	// Atualizar cabeçalho arquivo(-1 para não alterar campos desnecessários)
	AtualizarCabecalho(&arquivoSaida, '1', -1, proxByteOffset, nroRegArq, -1);

	// Liberar memória do buffer
	free(buffer);
	buffer = NULL;

	return;
}

/* Dado um arquivo aberto e um criterio, exibi os registros*/
void ExibirRegistrosDadoCriterio(FILE *arquivo, CRITERIO *crit) {
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    // Atualizar ponteiro do arquivo para o início
    fseek(arquivo, 0, SEEK_SET);

    char byteAtual;
    // Se o arquivo for inconsistente, Falha no processamento do arquivo. 
    fread(&byteAtual, sizeof(char), 1, arquivo);
    if(byteAtual == INCONSISTENTE){ 
        DispararErro(ErroProcessamentoArquivo());
        return; 
    }

    // Atualizar ponteiro do arquivo para o início dos registros
    fseek(arquivo, TAM_HEAD, SEEK_SET);

    // Existe registro a ser buscado?
    bool existeRegistro = false;
    
    // Buscar registro sem filtro(todos os não removidos)
    REGISTRO *registroBuscado = NULL;
        
    while((registroBuscado = SELECT_WHERE(arquivo, crit)) != NULL){
        ExibirRegistro(registroBuscado);
        ApagarRegistro(&registroBuscado);
        existeRegistro = true;
        // Se achou por idAttack, acabou a busca(identificador único)
        if(crit != NULL && crit->temIdAttack) break;
    }    

    if(existeRegistro == false) DispararErro(ErroRegistroInexistente());

    return;
}

/* 
Dado seletor no inicio de um registro, 
busca registro dado criterio
*/
REGISTRO *SELECT_WHERE(FILE *arquivo, CRITERIO *criterio){
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    REGISTRO *registroAtual = NULL;
    do {
        if(registroAtual != NULL) ApagarRegistro(&registroAtual);
        registroAtual = LerRegistro(arquivo);
        // Se retorno nulo, chegou-se ao final do arquivo.
        if(registroAtual == NULL) break;
    } while (!SelecionarPorCriterio(criterio, registroAtual));

    return registroAtual;
}

/* Dado um arquivo aberto e um criterio, remove registros */
void RemoverRegistroDadoCriterio(FILE *arquivo, CRITERIO *crit) {
    // Se arquivo nulo, encerra execução.
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    CABECALHO *c = LerCabecalho(&arquivo);
    if(c->status == INCONSISTENTE) {
        ApagarCabecalho(&c);
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    // Buscar registro sem filtro(todos os não removidos)
    REGISTRO *registroBuscado = NULL;
    while (c->nroRegArq != 0 &&
        (registroBuscado = SELECT_WHERE(arquivo, crit)) != NULL) {
        fseek(arquivo, -(registroBuscado->tamanhoRegistro + 5), SEEK_CUR);
        DELETE(arquivo, c, registroBuscado);
        ApagarRegistro(&registroBuscado);
        // Se achou por idAttack, acabou a busca(identificador único)
        if(crit->temIdAttack) break;
    }    

    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);

    return;
}

/* Remove registro e atualiza cabeçalho */
void DELETE(FILE *arquivo, CABECALHO *c, REGISTRO *reg) 
{
    if (arquivo == NULL || reg == NULL) {
        printf("Registro ou arquivo nulos na remoção do registro.\n");
        DispararErro(ErroPonteiroInvalido());
        return;
    }

    // Salvar dados a serem mantidos no registrador
    // e faz o ajuste do lixo decorrente dos dados antigos.
    reg->removido = '1';
    reg->prox = c->topo;
    c->topo = ftell(arquivo);
    c->nroRegArq--;
    c->nroRegRem++;
    EscreverRegistro(&arquivo, reg);
    return;
}

/* Dado um arquivo aberto, inseri um registro */
void InserirRegistro(FILE *arquivo, REGISTRO *reg) {
    // Se arquivo nulo, encerra execução.
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    CABECALHO *c = LerCabecalho(&arquivo);
    if(c->status == INCONSISTENTE) {
        ApagarCabecalho(&c);
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    INSERT(arquivo, c, reg);
    
    c->status = CONSISTENTE;
    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);

    return;
}

/* Inseri um registro e atualiza o cabecalho */
void INSERT(FILE *arquivo, CABECALHO *c, REGISTRO *reg) {
    // Byteoffset do campo Prox do registro Anterior 
    // da lista de registros removidos.
    long int campoProxAnterior = -1; // -1: não há.
    long int succRegRem = c->topo; // Proximo Reg. Removido.
    long int byteInsercao = -1; // Local de inserção do registro.

    while(succRegRem != -1) 
    {
        // Iniciar leitura a partir do campo tamanho.(Pula campo removido)
        fseek(arquivo, succRegRem + sizeof(char), SEEK_SET);
        int tamDisponivel;
        fread(&tamDisponivel, sizeof(int), 1, arquivo); 
        long int regAtual = succRegRem; 
        fread(&succRegRem, sizeof(long int), 1, arquivo);  
        if(tamDisponivel >= reg->tamanhoRegistro)
        {
            // Caso: Registro a ser removido é o primeiro.
            if (campoProxAnterior == -1) c->topo = succRegRem;
            else {
                // Manter ligamento da lista de removidos
                fseek(arquivo, campoProxAnterior, SEEK_SET);
                fwrite(&succRegRem, sizeof(long int), 1, arquivo);
            }

            // Prevalece tamanho do maior registro.
            reg->tamanhoRegistro = tamDisponivel; 
            byteInsercao = regAtual;
            c->nroRegRem--;
            break; 
        }

        // Salvar campo prox do registro atual.
        campoProxAnterior = regAtual +  sizeof(char) + sizeof(int);
    }

    // Caso: Inserção no fim do arquivo
    if(byteInsercao == -1) 
    {
        byteInsercao = c->proxByteOffset;
        c->proxByteOffset += reg->tamanhoRegistro + sizeof(char) + sizeof(int);
    }

    // Inserir registro
    fseek(arquivo, byteInsercao, SEEK_SET);
    EscreverRegistro(&arquivo, reg);

    // Atualizar cabeçalho
    c->nroRegArq++;
    return;
}

/* Dado arquivo aberto, busca registros dado um criterio e atualiza os dados*/
void AtualizarRegistroDadoCriterio(FILE *arquivo, 
                                CRITERIO *criterio, 
                                CRITERIO *valoresAtualizados
) {
    // Se arquivo nulo, encerra execução.
    if(arquivo == NULL) DispararErro(ErroArquivoInvalido());

    CABECALHO *c = LerCabecalho(&arquivo);
    if(c->status == INCONSISTENTE) {
        ApagarCabecalho(&c);
        DispararErro(ErroProcessamentoArquivo());
        return;
    }

    // Buscar registro sem filtro(todos os não removidos)
    REGISTRO *regBuscado = NULL;
    while (c->nroRegArq != 0 &&
        (regBuscado = SELECT_WHERE(arquivo, criterio)) != NULL) {     
        
        UPDATE(arquivo, c, valoresAtualizados, regBuscado);
        ApagarRegistro(&regBuscado);
        
        // Se achou por idAttack, acabou a busca(identificador único)
        if(criterio->temIdAttack) break;
    }    

    EscreverCabecalho(&arquivo, c);
    ApagarCabecalho(&c);

    return;
}

/* 
Atualiza o registro e o cabecalho 
Retorna -1 caso não mude o offset,
ou o offset atual do registro
*/
long int UPDATE(FILE *arquivo, 
            CABECALHO *c, 
            CRITERIO *valoresAtualizados, 
            REGISTRO *regBuscado
) {
    long int offsetInsercao = -1;
    // Atualizar registro encontrado, guardando o tamanho do desse registro.
    int espacoDisponivel = regBuscado->tamanhoRegistro;
    REGISTRO *regAtualizado = CriarRegistroAtualizado(regBuscado, valoresAtualizados);

    // Retornar file pointer para inicio desse espaço.
    fseek(arquivo, -(regBuscado->tamanhoRegistro + 5), SEEK_CUR);
    
    // Se após atualizar dados, verificar se cabe no espaço.
    if(espacoDisponivel >= regAtualizado->tamanhoRegistro){
        regAtualizado->tamanhoRegistro = espacoDisponivel;
        EscreverRegistro(&arquivo, regAtualizado);
    } else {
        // Procurar nova posição para inserir, salvando aonde estava.
        long int byteAtual = ftell(arquivo);
        // Remover registro buscado
        DELETE(arquivo, c, regBuscado);
        // Inserir registro atualizado
        INSERT(arquivo, c, regAtualizado);
        offsetInsercao = ftell(arquivo) - (regAtualizado->tamanhoRegistro + 5);

        fseek(arquivo, byteAtual, SEEK_SET);
    }

    ApagarRegistro(&regAtualizado);
    return offsetInsercao;
}

/* Retorna tamanho em bytes da linha a ser lida(0 caso chegue no final) */
char *_PegarLinha(char **buffer, int tamBuffer,FILE *arquivoAberto){
  int byteOffset = 0;
  char byteAtual;

  // Retorna 1 se conseguiu ler o caracter, se não conseguiu, chegou-se ao final do arquivo.
  while(1 == fread(&byteAtual, sizeof(char), 1, arquivoAberto)){

      // Verifica se o tamanho atual do buffer é suficiente
      if(byteOffset >= tamBuffer){
          // Aumenta tamanho do buffer em 50
          tamBuffer = tamBuffer + 50;
          (*buffer) = (char*) realloc((*buffer), tamBuffer);
      }
      // Não armazenar esses caracteres no buffer
      if(byteAtual != '\r' && byteAtual != '\n'){
          // Byte atual no buffer
          (*buffer)[byteOffset] = byteAtual;
          byteOffset++;
      }

      // Arquivo CSV tem conteúdo encerrado em \n ou \r
      if(byteAtual == '\n') break;
  }
  (*buffer)[byteOffset] = '\0';

  // Retorna nulo se não foi possível ler nada(EOF).
  return byteOffset == 0 ? NULL : (*buffer);
}


/* Função que dado uma linha csv, preenche registro */
void _PreencherRegistro(REGISTRO **reg, char *buffer){
    if (*reg == NULL || buffer == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    // Sumir com os delimitadores
    strtok(buffer, ",");
    while(strtok(NULL, ","));

    // Var. aux que ajuda a verificar campo vazio no buffer
    char *linha = buffer;
    
    // Campo 1: id do ataque(int)
    if(*linha != ','){
        // Armazenar id como inteiro
        (*reg)->idAttack = atoi(linha);
        // Andar tamanho do conteúdo
        linha += strlen(linha);
    }
    // Pula '\0' ou ','
    linha += 1;

    // Campo 2: year(int)
    if(*linha != ','){
        (*reg)->year = atoi(linha);
        // Andar tamanho do conteúdo como string
        linha += strlen(linha);
    }
    // Pula '\0' ou ','
    linha += 1;

    // Campo 3: financialLoss(float)
    if(*linha != ','){
        // Converte string para float e atualiza linha(linha += strlen(linha))
        (*reg)->financialLoss = strtof(linha, &linha);
    }
    // Pula '\0' ou ','
    linha += 1;

    // Armazenar tamanho dos campos variaveis
    int tamCamposVariaveis = 0;

    // Preencher campos variáveis
    _PreencherCampoVariavel(&(*reg)->country, &linha, &tamCamposVariaveis);
    _PreencherCampoVariavel(&(*reg)->attackType, &linha, &tamCamposVariaveis);
    _PreencherCampoVariavel(&(*reg)->targetIndustry, &linha, &tamCamposVariaveis);
    _PreencherCampoVariavel(&(*reg)->defenseMechanism, &linha, &tamCamposVariaveis);

    // Somar o tamanho dos campos das variáveis
    (*reg)->tamanhoRegistro += tamCamposVariaveis;

    return;
}

/* Função para preencher campos string de tamanho variavel dado uma linha CSV */
void _PreencherCampoVariavel(char **campo, char **linha, int *tamVariaveis) {
    if(**linha != ',' && **linha != '\0') {
        int tamString = strlen(*linha);
        char *string = malloc(sizeof(char) * (tamString + 1));
        
        if (string != NULL) {
            strcpy(string, *linha);
            string[tamString] = '\0';
            *campo = string;
            
            *tamVariaveis += tamString + 2;
            *linha += tamString;
        }
    }

    *linha += 1;
}