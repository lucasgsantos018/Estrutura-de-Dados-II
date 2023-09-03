#ifndef arvoreB_h
#define arvoreB_h

#include "base.h"

#define M 5
#define MM 2 * M + 1

typedef long TipoChave;

typedef struct TipoPagina *TipoApontador;

typedef struct TipoPagina
{
    short n; // quantidade de posições do vetor
    TipoRegistro r[MM];
    TipoApontador p[MM + 1];
} TipoPagina;

void inicializa(TipoApontador *Arvore);

void PesquisaArvoreB(TipoRegistro *x, TipoApontador Ap, int *contComparacoes, int *achouRegistro);

void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir, int *contComparacoes);

void Ins(TipoRegistro Registro, TipoApontador Ap, int *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno, int *contComparacoes);

void Insere(TipoRegistro Registro, TipoApontador *Ap, int *contComparacoes);

void ArvoreB(TipoApontador *Arvore, FILE *arquivo, int quantidade, TipoRegistro *registro, int *contComparacoes, int *contTransferencias, int *achouRegistro);

#endif