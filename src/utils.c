#include "utils.h"

#include<stdlib.h>
#include<string.h>
#include<ctype.h>

// Função para ler e processar campos numéricos inteiros
int LerCampoInteiro(void) {
    char* campo = LerString();
	int valor = -1; // Valor NULO

	// Preencher inteiro 
    if (strcmp(campo, "NULO") != 0) {
		valor = atoi(campo);
	}

    free(campo);
	campo = NULL;

	return valor;
}

// Função para ler e processar campos numéricos float
float LerCampoFloat(void) {
    char* campo = LerString();
	float valor = -1; // Valor NULO

	// Preencher float 
    if (strcmp(campo, "NULO") != 0) {
		valor = strtof(campo, NULL);
	}

    free(campo);
	campo = NULL;

	return valor;
}

// Função para ler e processar campos de string com aspas
char *LerCampoStringComAspas(void) {
	char* campo = LerStringComAspas();
	if (strcmp(campo, "") == 0) {
		free(campo);
		campo = NULL;
	}
	
	return campo;
}

// Função lê uma string sem desperdício de memória
char *LerString(void){
  char buffer[256];
  char *stringDinamica;

  scanf(" %s", buffer);
  // Retorna tamanho da string sem o \0
  int tamString = strlen(buffer);

  // Alocar espaço na heap para a tamString + \0
  stringDinamica = (char*) malloc(sizeof(char)*(tamString + 1));
  strcpy(stringDinamica, buffer);
  // Garante \0 no final da string
  stringDinamica[tamString] = '\0';

  return stringDinamica;
}

char *LerStringComAspas(void){
	char buffer[256];
	char *stringDinamica;

	scan_quote_string(buffer);

	// Retorna NULL se string vazia
	if(strcmp(buffer, "") == 0) return NULL;

	// Retorna tamanho da string sem o \0
	int tamString = strlen(buffer);

	// Alocar espaço na heap para a tamString + \0
	stringDinamica = (char*) malloc(sizeof(char)*(tamString + 1));
	strcpy(stringDinamica, buffer);
	// Garante \0 no final da string
	stringDinamica[tamString] = '\0';

	return stringDinamica;

}

// Fecha um arquivo verificando se é NULL
void FecharArquivo(FILE **arquivo) {
	if (*arquivo == NULL) {
		return;
	}

	fclose(*arquivo);
	*arquivo = NULL;
}

/*
Função para imprimir dados salvos no arquivo em binário
(util para comparar saida no run codes)
*/
void binarioNaTela(char *nomeArquivoBinario) { 

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}
/* ---------------- EXTRA ----------------

OPCIONAL: dicas sobre scanf() e fscanf():

scanf("%[^,]", string) -> lê até encontrar o caractere ',', não incluindo o mesmo na leitura.

Exemplo de entrada: "Oi, esse é um exemplo."
Nesse caso, o scanf("%[^,]") tem como resultado a string "Oi";

scanf("%[^\"]", string) -> lê até encontrar o caractere '"', não incluindo o mesmo na leitura.
scanf("%[^\n]", string) -> lê até encontrar o fim da linha, não incluindo o '\n' na leitura.

scanf("%*c") --> lê um char e não guarda em nenhuma variável, como se tivesse ignorado ele

*/

