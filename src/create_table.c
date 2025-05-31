#include "create_table.h"
#include "erros.h"

#include<stdlib.h>
#include<string.h>

// Protótipos
char *_PegarLinha(char **buffer, int tamBuffer,FILE *arquivoAberto);
void LerCsvParaBinario(FILE *arquivoEntrada, FILE *arquivoSaida);
void _PreencherCampoVariavel(char **campo, char **linha, int *tamVariaveis);
void _PreencherRegistro(REGISTRO **reg, char *buffer);

// Ler arquivo CSV e escreve em binário.
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

// Retorna tamanho em bytes da linha a ser lida(0 caso chegue no final)
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


// Função que dado uma linha csv, preenche registro
void _PreencherRegistro(REGISTRO **reg, char *buffer){
    if (*reg == NULL || buffer == NULL) {
        DispararErro(ErroPonteiroInvalido());
    }

    /*
    Manejo strok() - tirado site da IBM
    conteudo = strtok(linha do csv, delimitador).
    OBS: a cada chamada substitui o delimitador por um caracter nulo.
    Exemplo de execução de 3 strtoks:
    buffer("juan,20,santos") -> buffer("juan\020\0santos\0")
    Para chamadas em que a linha é nula,
    continua da onde parou no argumento não nulo mais recente.
    OBS: se o conteúdo é nulo(",,"), ele pula para o próximo.
    Caso não ache o delimitador(exemplo no final), outro caracter 
    de parada é o '\0'.
    OBS: ele ignora conteudo vazios e busca conteudos.
    */
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

// Função para preencher campos string de tamanho variavel dado uma linha CSV
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

