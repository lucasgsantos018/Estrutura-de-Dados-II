#ifndef arvoreBestrela_h
#define arvoreBestrela_h

#include "arvoreB.h"

typedef struct TipoPaginaSec* TipoApontadorSec;

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPaginaSec{
    TipoIntExt Pt;
    union {
        struct { //Pagina interna
            int ni;
            TipoChave ri[MM];
            TipoApontadorSec pi[MM + 1];
        } U0;
        struct { //Pagina externa (folha)
            int ne;
            TipoRegistro re[2 * MM];
        } U1;
    } UU;
}TipoPaginaSec;

void inicializaBEstrela(TipoApontadorSec Arvore);

void pesquisaArvoreBEstrela(TipoRegistro *x, TipoApontadorSec *ap, int *contComparacoes, int *contTransferencias, int *achouRegistro);

void insereNapaginaEstrela(TipoApontadorSec ap, TipoRegistro Reg, TipoApontadorSec apDir, int *contComparacoes, int *contTransferencias);

void insrecuBEstrela(TipoRegistro Reg, TipoApontadorSec ap, short *cresceu, TipoRegistro *RegRetorno, TipoApontadorSec *ApRetorno, int *contComparacoes, int *contTransferencias);

void insereArvoreBEstrela(TipoRegistro Reg, TipoApontadorSec *ap, int *contComparacoes, int *contTransferencias);

void criaArvoreBEstrela(FILE *arqReg, TipoApontadorSec *ap, int quantidade, int *contComparacoes, int *contTransferencias);

#endif
