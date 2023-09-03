#include <stdio.h>
#include <stdlib.h>
#include "asi.h"


bool pesquisaSequencial(tipoindice tab[], int tam, TipoRegistro *item, FILE *arq, bool aux, int *contadorCompara, int *contadorMem){
    
    TipoRegistro pagina[ITENSPAGINA];
	int i = 0, quantItens;
	long desloc;
	//Busca a página onde o item pode ser localizado.
	while (i < tam && tab[i].chave <= item->chave){
		(*contadorCompara)++; // comparação entre chaves durante a pesquisa;
		if (aux)
			printf("%d, ", pagina[i].chave);
		i++;
	}
	(*contadorCompara)++;
	if (i == 0) 
		return false;
	else{
		if (i < tam){ // a ultima página pode não estar completa
			quantItens = ITENSPAGINA;
		}
		else{
			fseek(arq, 0, SEEK_END);
			quantItens = (ftell(arq)/sizeof(TipoRegistro))%ITENSPAGINA;
			if (quantItens == 0)
				quantItens = ITENSPAGINA;
		}
		
		// lê a página desejada do arquivo		
		desloc = (tab[i-1].posicao-1)*ITENSPAGINA*sizeof(TipoRegistro);
		fseek (arq, desloc, SEEK_SET);
		fread (&pagina, sizeof(TipoRegistro), quantItens, arq);
		(*contadorMem)++;
		
		// pesquisa sequencial na página lida
		for (int j = 0; j < quantItens; j++) {
            if (aux)
                printf("%d, ", pagina[j].chave);
            (*contadorCompara)++;
            if (pagina[j].chave == item->chave) {
                *item = pagina[j];
                return true;
            }
        }
		return false;
	}
}