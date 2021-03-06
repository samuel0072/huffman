/*
*Modulo das estruturas
*/

/*Definicoes de tipos*/

//tipo exportado binario
typedef struct binario binario;

//tipo exportado dicionario
typedef struct dicionario dicionario;

//tipo exportado NO
typedef struct NO NO;

//Tipo exportado FILA
typedef struct FILA FILA;

//Definicao da estrutura binario
struct binario{
	char* byte;
}; 

//Definicao da estrutura dicionario
struct dicionario{
	void* bytes[256];
};

//Definicao da estrutura NO
struct NO{
	unsigned char byte;
	int freq;
	short caracter_controle;
	void *proximo;
	NO *esquerda, *direita;
};

//Definicao da estrutura FILA
struct FILA{
	void *cabeca;
};