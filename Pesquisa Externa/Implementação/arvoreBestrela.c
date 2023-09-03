#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "base.h"
#include "arvoreBestrela.h"

void inicializaBEstrela(TipoApontadorSec Arvore){
    Arvore = NULL;
}

void pesquisaArvoreBEstrela(TipoRegistro *x, TipoApontadorSec *ap, int *contComparacoes, int *contTransferencias, int *achouRegistro){

    int i;

    (*contComparacoes) += 1;

    TipoApontadorSec pag;
    pag = *ap;

    if((*ap)->Pt == Interna){
        i = 1;
        while((i < pag->UU.U0.ni) && (x->chave > pag->UU.U0.ri[i-1])){ // Pesquisa sequencial na página toda
            (*contComparacoes) += 1;
            i++;
        }
        // recursão em uma das subarvores até chegar na pagina folha
        if(x->chave < pag->UU.U0.ri[i-1]){
            (*contComparacoes) += 1;
            pesquisaArvoreBEstrela(x, &(pag->UU.U0.pi[i-1]), contComparacoes, contTransferencias, achouRegistro);
        }else{
            (*contComparacoes) += 1;
            pesquisaArvoreBEstrela(x, &(pag->UU.U0.pi[i]), contComparacoes, contTransferencias, achouRegistro);
        }
        return;
    }

    i = 1;

    while((i < pag->UU.U1.ne) && (x->chave > pag->UU.U1.re[i-1].chave)){ // Pesquisa sequencial na pagina folha
        (*contComparacoes) += 1;
        i++;
    }

    if(x->chave == pag->UU.U1.re[i-1].chave){ // Condicional para saber se a chave foi achada ou não
        (*contComparacoes) += 1;
        *x = pag->UU.U1.re[i-1];
        *achouRegistro = 1;
    }else{
        *achouRegistro = 0;
    }

}

void insereNapaginaEstrela(TipoApontadorSec ap, TipoRegistro Reg, TipoApontadorSec apDir, int *contComparacoes, int *contTransferencias){

    short naoAchouPosicao = 0;
    int k;

    //Caso a pagina seja externa (folha)
    if(ap->Pt == Externa){
        k = ap->UU.U1.ne;

        if(k > 0)
            naoAchouPosicao = 1;

        while(naoAchouPosicao){
            (*contComparacoes) += 1;
            if (Reg.chave >= ap->UU.U1.re[k-1].chave){
                naoAchouPosicao = 0;
                break;
            }
            ap->UU.U1.re[k] = ap->UU.U1.re[k-1];

            k--;

            if(k < 1)
                naoAchouPosicao = 0;
        }
        ap->UU.U1.re[k] = Reg;
        ap->UU.U1.ne++;
    }

    //Caso a pagina seja interna
    else{
        k = ap->UU.U0.ni;

        if(k > 0){
            naoAchouPosicao = 1;
        }

        while(naoAchouPosicao){
            (*contComparacoes) += 1;
            if (Reg.chave >= ap->UU.U0.ri[k - 1]){
                naoAchouPosicao = 0;
                break;
            }
            ap->UU.U0.ri[k] = ap->UU.U0.ri[k - 1];

            ap->UU.U0.pi[k + 1] = ap->UU.U0.pi[k];

            k--;

            if(k < 1)
                naoAchouPosicao = 0;
        }

        ap->UU.U0.ri[k] = Reg.chave;

        ap->UU.U0.pi[k + 1] = apDir;

        ap->UU.U0.ni++;
    }

    return;

}


void insrecuBEstrela(TipoRegistro Reg, TipoApontadorSec ap, short *cresceu, TipoRegistro *RegRetorno, TipoApontadorSec *ApRetorno, int *contComparacoes, int *contTransferencias){
    long i = 1;
    long j;
    TipoApontadorSec ApTemp;

    //Caso a pagina seja externa (folha)
    if(ap->Pt == Externa){
        (*contComparacoes) += 1;
        while(i < ap->UU.U1.ne && Reg.chave > ap->UU.U1.re[i-1].chave){
            (*contComparacoes) += 1;
            i++;
        }

        (*contComparacoes) += 1;
        if(Reg.chave == ap->UU.U1.re[i-1].chave){
            
            *cresceu = 0;
            return;
        }

        (*contComparacoes) += 1;
        if(Reg.chave < ap->UU.U1.re[i-1].chave){
            i--;
        }

        //Página tem espaço
        if(ap->UU.U1.ne < (2 * MM)){
            *RegRetorno = Reg;

            insereNapaginaEstrela(ap, *RegRetorno, *ApRetorno, contComparacoes, contTransferencias);

            *cresceu = 0;
            return;
        }
        //Página tem que ser dividida
        ApTemp = (TipoPaginaSec*) malloc(sizeof(TipoPaginaSec));
        ApTemp->Pt = Externa;
        ApTemp->UU.U1.ne = 0;

        *RegRetorno = Reg; //retorno recebe o item
        *ApRetorno = NULL; //Apontador recebe NULL
        
        //Emprestimo
        if(i < (M + 1)){
            insereNapaginaEstrela(ApTemp, ap->UU.U1.re[(2 * MM) - 1], *ApRetorno, contComparacoes, contTransferencias);
            ap->UU.U1.ne--;
            insereNapaginaEstrela(ap, *RegRetorno, *ApRetorno, contComparacoes, contTransferencias);
        }
        //Inserção direta
        else{
            insereNapaginaEstrela(ApTemp, *RegRetorno, *ApRetorno, contComparacoes, contTransferencias);
        }

        //Inserindo a segunda metade do TipoApontadorSec na pagina criada
        for (j = 1; j <= M; j++){
            insereNapaginaEstrela(ApTemp, ap->UU.U1.re[(2 * MM) - j], *ApRetorno, contComparacoes, contTransferencias);
        }
        
        ap->UU.U1.ne = M;
        *RegRetorno = ap->UU.U1.re[M];
        *ApRetorno = ApTemp;
        *cresceu = 1;
        return;
    }
    //Caso a pagina seja interna
    else{
        (*contComparacoes) += 1;
        while(i < ap->UU.U0.ni && Reg.chave > ap->UU.U0.ri[i-1]){
            (*contComparacoes) += 1;
            i++;
        }

        (*contComparacoes) += 1;
        if(Reg.chave == ap->UU.U0.ri[i-1]){
            *cresceu = 0;
            return;
        }

        (*contComparacoes) += 1;
        if(Reg.chave < ap->UU.U0.ri[i-1]){
            i--;
        }
        
        insrecuBEstrela(Reg, ap->UU.U0.pi[i], cresceu, RegRetorno, ApRetorno, contComparacoes, contTransferencias);

        if(!*cresceu)
            return;

        //Caso a página tenha espaço
        if(ap->UU.U0.ni < MM){
            insereNapaginaEstrela (ap, *RegRetorno, *ApRetorno, contComparacoes, contTransferencias);

            *cresceu = 0;

            return;
        }
        
        //Página tem que ser dividida
        ApTemp = (TipoPaginaSec*) malloc(sizeof(TipoPaginaSec));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 0;
        ApTemp->UU.U0.pi[0] = NULL;

        TipoRegistro RegAux;

        //Emprestimo
        if(i < M + 1){
            RegAux.chave = ap->UU.U0.ri[MM-1];
            insereNapaginaEstrela(ApTemp, RegAux, ap->UU.U0.pi[MM], contComparacoes, contTransferencias);
            ap->UU.U0.ni--;
            insereNapaginaEstrela(ap, *RegRetorno, *ApRetorno, contComparacoes, contTransferencias);
        }
        //Insercao direta
        else{
            insereNapaginaEstrela(ApTemp, *RegRetorno, *ApRetorno, contComparacoes, contTransferencias);
        }

        for (j = (M + 2); j <= MM; j++){
            RegAux.chave = ap->UU.U0.ri[j - 1];
            insereNapaginaEstrela(ApTemp, RegAux, ap->UU.U0.pi[j], contComparacoes, contTransferencias);
        }

        ap->UU.U0.ni = M;

        ApTemp->UU.U0.pi[0] = ap->UU.U0.pi[M+1];

        RegRetorno->chave = ap->UU.U0.ri[M];

        *ApRetorno = ApTemp;

        return;
    }

}

void insereArvoreBEstrela(TipoRegistro Reg, TipoApontadorSec *ap, int *contComparacoes, int *contTransferencias){

    short cresceu;
    TipoRegistro RegRetorno;
    TipoPaginaSec *ApRetorno;
    TipoPaginaSec *ApTemp;
    
    //Inserção do primeiro item em uma arvore B* vazia
    if(*ap == NULL){
        ApTemp = (TipoPaginaSec*) malloc(sizeof(TipoPaginaSec));
        ApTemp->Pt = Externa;
        ApTemp->UU.U1.ne = 1;
        ApTemp->UU.U1.re[0] = Reg;
        *ap = ApTemp;
        return;
    }
    //Inserção dos demais itens
    else{
        insrecuBEstrela(Reg, *ap, &cresceu, &RegRetorno, &ApRetorno, contComparacoes, contTransferencias);
        //Arvore cresceu na altura pela raiz
        if (cresceu){
            ApTemp = (TipoPaginaSec*) malloc(sizeof(TipoPaginaSec));
            ApTemp->Pt = Interna;
            ApTemp->UU.U0.ni = 1;
            ApTemp->UU.U0.ri[0] = RegRetorno.chave;
            ApTemp->UU.U0.pi[1] = ApRetorno;
            ApTemp->UU.U0.pi[0] = *ap;
            *ap = ApTemp;
        }
    }
}

// Esse método é responsável por criar a árvore B* na memória principal tendo ap como o ponteiro para a raiz dessa árvore
void criaArvoreBEstrela(FILE *arqReg, TipoApontadorSec *ap, int quantidade, int *contComparacoes, int *contTransferencias){
    TipoRegistro reg;

    rewind(arqReg);
    inicializaBEstrela(*ap);

    for(int i = 0; i < quantidade; i++){

        fread(&reg, sizeof(TipoRegistro), 1, arqReg);
        
        (*contTransferencias) += 1;

        insereArvoreBEstrela(reg, ap, contComparacoes, contTransferencias);
    }
}



