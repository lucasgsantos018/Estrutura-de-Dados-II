#ifndef base_h
#define base_h

#include <stdio.h>
#include <time.h>

typedef struct TipoRegistro{
    int chave;
    long int dado1;
    char dado2[5000];
} TipoRegistro;

void imprimeRegistro(TipoRegistro registro);

void imprimeChaves(FILE *arquivo, int quantidade);

void imprimirTotal(int contComparacoes, int contTransferencias, clock_t tempo_pesquisa, clock_t tempo_processamento);

#endif