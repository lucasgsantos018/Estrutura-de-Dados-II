#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "base.h"

void imprimeRegistro(TipoRegistro registro){
    printf("Chave: %d\n", registro.chave);
}

void imprimeChaves(FILE *arquivo, int quantidade){
    TipoRegistro temp;

    int contadorChave = 1;

    rewind(arquivo);

    while(1){
        fread(&temp, sizeof (TipoRegistro), 1, arquivo);
        printf("%dº Chave: %d\n", contadorChave, temp.chave);
        contadorChave++;
        if(contadorChave == quantidade + 1)
            break;
    }

    rewind(arquivo);
}

void imprimirTotal(int contComparacoes, int contTransferencias, clock_t tempo_pesquisa, clock_t tempo_processamento){
    double tmpPreProcessamento = (((double)tempo_processamento)/1000.0);
    double tmpPesquisa = tempo_pesquisa;
    double tmpTotal = ((tempo_pesquisa + tempo_processamento)/1000.0);
    
    printf("Transferências: %d\n", contTransferencias);
    printf("Comparações: %d\n", contComparacoes);
    printf("Tempo Pré Processamento: %.3lf s\n", tmpPreProcessamento);
    printf("Tempo Pesquisa: %.3lf milissegundos\n", tmpPesquisa);
    printf("Tempo Total: %.3lf s\n", tmpTotal);
}