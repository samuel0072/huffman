#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "estruturas.h"
#include "func_fila.h"
#include "func_arvore.h"
#include "hash.h"
#include "comprimir.h"
#include "descomprimir.h"

int is_bit_i_set(unsigned char c, int i){
	unsigned char mask = 1 << i;
	return mask & c;
}

NO* criar_no_arv(unsigned char c){
	
	NO *novo = (NO*) malloc(sizeof(NO));

	novo->byte = c;		
	novo->esquerda = novo->direita = NULL;

	return novo;
}

NO* criar_arv(NO* bt, unsigned short *i, unsigned short tam, FILE *compr){
	if(*i == tam) return bt;
	(*i)++;
	unsigned char item;
	
	fread(&item, sizeof(item), 1, compr);

	if(item == 92){
		fread(&item, sizeof(item), 1, compr);		
		(*i)++;
		return criar_no_arv(item);
	}
	
	bt = criar_no_arv(item);

	if(item == 42){	
		bt->esquerda = criar_arv(bt->esquerda, i, tam, compr);		
		bt->direita = criar_arv(bt->direita, i, tam, compr);
	}

	return bt;
}

void descomprimir(FILE* compr)
{
	NO* arvore = NULL;

	unsigned char byte;
	unsigned short cabecalho;
	unsigned short tam_lixo = 0;
	unsigned short tam_arvore = 0;
	unsigned short indice = 0;
	
	fread(&byte, sizeof(byte), 1, compr);
	cabecalho = byte<<8;
	fread(&byte, sizeof(byte), 1, compr);
	cabecalho = cabecalho | (byte<<8)>>8;
	printf("%hu\n", cabecalho);

	tam_lixo = cabecalho>>13;
	tam_arvore = (cabecalho << 3);
	tam_arvore = tam_arvore >>3;
	printf("lixo = %hu, tam_arvore = %hu", tam_lixo,  tam_arvore);

	arvore = criar_arv(arvore, &indice, tam_arvore, compr);

	//mostrar_arvore(arvore);

	descompressao(tam_lixo, tam_arvore, arvore, compr);
}

void descompressao(unsigned short tam_lixo, unsigned short tam_arvore, NO* arvore, FILE* arq_comp)
{
	int tam_arquivo = 0, bytes_lidos = 1, indice = 7;
	unsigned char byte;

	NO* raiz = arvore;
	FILE* arq_descomp = fopen("descomprimido", "wb");

	fpos_t pos;

	fgetpos(arq_comp, &pos);

	while(!feof(arq_comp))
	{
		fread(&byte, sizeof(byte), 1, arq_comp);
		tam_arquivo++;
	}
	tam_arquivo--;
	fsetpos(arq_comp, &pos);

	fread(&byte, sizeof(byte), 1, arq_comp);

	while(bytes_lidos < tam_arquivo)
	{
		if(is_bit_i_set(byte, indice))
		{
			arvore = arvore->direita;
			indice--;
		}
		else
		{
			arvore = arvore->esquerda;
			indice--;
		}
		if(arvore->esquerda == NULL && arvore->direita == NULL)
		{
			fwrite(&(arvore->byte), sizeof(arvore->byte), 1, arq_descomp);
			arvore = raiz;
		}
		if(indice < 0)
		{
			indice = 7;
			bytes_lidos++;
			fread(&byte, sizeof(byte), 1, arq_comp);
		}
	}
	indice = 7;
	while(indice >= tam_lixo)
	{
		if(is_bit_i_set(byte, indice))
		{
			arvore = arvore->direita;
			indice--;
		}
		else
		{
			arvore = arvore->esquerda;
			indice--;
		}
		if(arvore->esquerda == NULL && arvore->direita == NULL)
		{
			fwrite(&(arvore->byte), sizeof(arvore->byte), 1, arq_descomp);
			arvore = raiz;
		}
	}
	fclose(arq_descomp);
}