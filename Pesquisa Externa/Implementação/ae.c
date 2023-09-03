#include "ae.h"

int insereArvExt(FILE *arvoreExt, tipoindice indice, int posicaoPai, int *contComparacoes, int *contTransferencias){
    Nodo nodo, nodoAux;
    nodo.indice = indice;
    nodo.esq = -1;
    nodo.dir = -1;
    int vazio = fread(&nodoAux, sizeof(Nodo), 1, arvoreExt) == 0 ? 1 : 0;
    (*contTransferencias)++;
    if (vazio && posicaoPai == -1)
    {
        (*contComparacoes)++;
        fwrite(&nodo, sizeof(Nodo), 1, arvoreExt);
        return 1;
    }
    if (vazio){
        (*contComparacoes)++;
        int posicaoFilho = ftell(arvoreExt) / sizeof(Nodo);
        fwrite(&nodo, sizeof(Nodo), 1, arvoreExt);
        
        fseek(arvoreExt, posicaoPai * sizeof(Nodo), SEEK_SET);
        fread(&nodoAux, sizeof(Nodo), 1, arvoreExt);
        (*contTransferencias)++;
        if(nodoAux.indice.chave < nodo.indice.chave)
        {
            (*contComparacoes)++;
            nodoAux.dir = posicaoFilho;
        }else
        {
            (*contComparacoes)++;
            nodoAux.esq = posicaoFilho;
        }
        fseek(arvoreExt, posicaoPai * sizeof(Nodo), SEEK_SET);
        fwrite(&nodoAux, sizeof(Nodo), 1, arvoreExt);
        return 1;
    }
    
    if(nodoAux.indice.chave == nodo.indice.chave){
        (*contComparacoes)++;
        return 0;
    }
    if (nodoAux.indice.chave < nodo.indice.chave)
    {
        (*contComparacoes)++;
        if (nodoAux.dir != -1)
        {
            (*contComparacoes)++;
            fseek(arvoreExt, nodoAux.dir * sizeof(Nodo), SEEK_SET);
        }else
        {
            (*contComparacoes)++;
            posicaoPai = ftell(arvoreExt) / sizeof(Nodo) - 1;
            fseek(arvoreExt, 0, SEEK_END);
        }
        insereArvExt(arvoreExt, indice, posicaoPai, contComparacoes, contTransferencias);
    }
    if (nodoAux.indice.chave > nodo.indice.chave)
    {
        (*contComparacoes)++;
        if (nodoAux.esq != -1)
        {
            (*contComparacoes)++;
            fseek(arvoreExt, nodoAux.esq * sizeof(Nodo), SEEK_SET);
        }else
        {
            (*contComparacoes)++;
            posicaoPai = ftell(arvoreExt) / sizeof(Nodo) - 1;
            fseek(arvoreExt, 0, SEEK_END);
        }
        insereArvExt(arvoreExt, indice, posicaoPai, contComparacoes, contTransferencias);
    }
    
    return 1;
}

void criaArvoreExt(FILE *fp, int chave, int tamanho,int *contComparacoes, int *contTransferencias, int *achouRegistro, clock_t *tempo_pesquisa, clock_t *tempo_processamento){
    TipoRegistro aux;
    tipoindice indiceAux;
    *tempo_processamento = clock();
    FILE *arvoreExt = fopen("arvoreExt.bin", "w+b");
    for (int i = 0; i < tamanho; i++){
        fread(&aux, sizeof(TipoRegistro), 1, fp);
        (*contTransferencias) += 1;
        indiceAux.chave = aux.chave;
        indiceAux.posicao = i;
        fseek(arvoreExt, 0, SEEK_SET);
        insereArvExt(arvoreExt, indiceAux, -1, contComparacoes, contTransferencias);
    }
    *tempo_processamento = clock() - *tempo_processamento;

    *tempo_pesquisa = clock();
    int posicaoAchada = pesquisa(arvoreExt, chave, 0, achouRegistro, contComparacoes, contTransferencias);
    
    if(*achouRegistro) {
        printf("Registro Encontrado\n");
        fseek(arvoreExt, posicaoAchada * sizeof(TipoRegistro), SEEK_SET);
        fread(&aux, sizeof(TipoRegistro), 1, arvoreExt);
        (*contTransferencias)++;
        imprimeRegistro(aux);
        *tempo_pesquisa = clock() - *tempo_pesquisa;
    }else
    {
        printf("Não há registro com essa chave\n");
    }
    

    fclose(arvoreExt);
}

int pesquisa(FILE *fp, int valor, int posicao, int *achouRegistro, int *contComparacoes, int *contTransferencias) {
  Nodo nodo;
  fseek(fp, posicao * sizeof(Nodo), SEEK_SET);
  fread(&nodo, sizeof(nodo), 1, fp);
  (*contTransferencias)++;
  if (nodo.indice.chave == valor) {
    *achouRegistro = 1;
    return nodo.indice.posicao;
  }
  
  if (nodo.indice.chave > valor && nodo.esq != -1) {
    (*contComparacoes) += 1;
    return pesquisa(fp, valor, nodo.esq, achouRegistro, contComparacoes, contTransferencias);
  } else if (nodo.dir != -1) {
    (*contComparacoes) += 1;
    return pesquisa(fp, valor, nodo.dir, achouRegistro, contComparacoes, contTransferencias);
  } else {
    (*contComparacoes) += 1;
    return -1;
  }
}
