#ifndef asi_h
#define asi_h
#define ITENSPAGINA 100
#define MAXTABELA 12000

#include "base.h"
#include <stdbool.h>

typedef struct{
	int chave;
	int posicao;
}tipoindice;

bool pesquisaSequencial(tipoindice *, int, TipoRegistro *, FILE *, bool, int *, int *);

#endif
