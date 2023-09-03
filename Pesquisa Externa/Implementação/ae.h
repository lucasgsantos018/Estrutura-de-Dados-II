#ifndef ae_h
#define ae_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "base.h"
#include "asi.h"

typedef struct{
    tipoindice indice;
    int esq;
    int dir;
}Nodo;

void criaArvoreExt(FILE *fp, int chave, int tamanho,int *contComparacoes, int *contTransferencias, int *achouRegistro, clock_t *tempo_pesquisa, clock_t *tempo_processamento);
int pesquisa(FILE *fp, int valor, int posicao, int *achouRegistro, int *contComparacoes, int *contTransferencias);


#endif