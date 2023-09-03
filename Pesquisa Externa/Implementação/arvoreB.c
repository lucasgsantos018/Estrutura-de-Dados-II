#include <stdio.h>
#include <stdlib.h>

#include "arvoreB.h"

void inicializa(TipoApontador *Arvore)
{
    Arvore = NULL;
}

void PesquisaArvoreB(TipoRegistro *x, TipoApontador Ap, int *contComparacoes, int *achouRegistro)
{
    long i = 1;
    if (Ap == NULL)
    {
        // printf("Este registro nao esta presente na arvore\n");

        return;
    }

    (*contComparacoes)++;
    while ((i < Ap->n) && (x->chave > Ap->r[i - 1].chave))
    {
        i++;
        (*contComparacoes)++;
    }

    (*contComparacoes)++;
    if (x->chave == Ap->r[i - 1].chave)
    {
        *x = Ap->r[i - 1];
        *achouRegistro = 1;

        return;
    }

    (*contComparacoes)++;
    if (x->chave < Ap->r[i].chave)
        PesquisaArvoreB(x, Ap->p[i - 1], contComparacoes, achouRegistro);
    else
        PesquisaArvoreB(x, Ap->p[i], contComparacoes, achouRegistro);
    return;
}

void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir, int *contComparacoes)
{
    int NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);

    while (NaoAchouPosicao)
    { // Verificando se tem espaço na pagina para inserir
        (*contComparacoes)++;
        if (Reg.chave >= Ap->r[k - 1].chave)
        {
            NaoAchouPosicao = 0;
            break;
        }
        Ap->r[k] = Ap->r[k - 1]; // Arredando os itens e ponteiros
        Ap->p[k + 1] = Ap->p[k];
        k--;
        if (k < 1)
            NaoAchouPosicao = 0;
    }
    Ap->r[k] = Reg;
    Ap->p[k + 1] = ApDir;
    Ap->n++;
}

void Ins(TipoRegistro Registro, TipoApontador Ap, int *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno, int *contComparacoes)
{ // Função que controla as inserções
    int i = 1, j;
    TipoApontador ApontadorAux;

    if (Ap == NULL)
    { // Atualizando o cresceu, que indica se é preciso fazer uma divisão naquele nivel, aumentando a altura
        *Cresceu = 1;
        (*RegRetorno) = Registro;
        (*ApRetorno) = NULL;
        return;
    }

    (*contComparacoes)++;
    while (i < Ap->n && Registro.chave > Ap->r[i - 1].chave)
    {
        i++;
        (*contComparacoes)++;
    } // Verificando os itens que estao numa pagina

    (*contComparacoes)++;
    if (Registro.chave == Ap->r[i - 1].chave)
    { // Tentando inserir um item repetido
        *Cresceu = 0;
        return;
    }

    (*contComparacoes)++;

    // Chama recursivamente até encontrar um nó folha
    if (Registro.chave < Ap->r[i - 1].chave)
    {
        i--;
        (*contComparacoes)++;
    }
    Ins(Registro, Ap->p[i], Cresceu, RegRetorno, ApRetorno, contComparacoes);
    if (!*Cresceu)
        return;

    if (Ap->n < 2 * M)
    { // Pagina tem espaço
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno, contComparacoes);
        *Cresceu = 0;
        return;
    }

    // Overflow: Pagina tem que ser dividida
    ApontadorAux = (TipoApontador)malloc(sizeof(TipoPagina));
    ApontadorAux->n = 0;
    ApontadorAux->p[0] = NULL;

    if (i < M + 1)
    {
        InsereNaPagina(ApontadorAux, Ap->r[2 * M - 1], Ap->p[2 * M], contComparacoes);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno, contComparacoes);
    }
    else
        InsereNaPagina(ApontadorAux, *RegRetorno, *ApRetorno, contComparacoes);

    for (j = M + 2; j <= 2 * M; j++)
        InsereNaPagina(ApontadorAux, Ap->r[j - 1], Ap->p[j], contComparacoes);

    // Salva os valores para serem tratados recursivamente
    Ap->n = M;
    ApontadorAux->p[0] = Ap->p[M + 1];
    *RegRetorno = Ap->r[M];
    *ApRetorno = ApontadorAux;
}

void Insere(TipoRegistro Registro, TipoApontador *Ap, int *contComparacoes)
{ // Funçao que começa o processo de inserção
    int Cresceu;
    TipoRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApAux;

    Ins(Registro, *Ap, &Cresceu, &RegRetorno, &ApRetorno, contComparacoes);
    if (Cresceu) // Arvore crescendo um nivel
    {
        ApAux = (TipoPagina *)malloc(sizeof(TipoPagina));
        ApAux->n = 1;
        ApAux->r[0] = RegRetorno;
        ApAux->p[1] = ApRetorno;
        ApAux->p[0] = *Ap;
        *Ap = ApAux;
    }
}

void ArvoreB(TipoApontador *Arvore, FILE *arquivo, int quantidade, TipoRegistro *registro, int *contComparacoes, int *contTransferencias, int *achouRegistro)
{
    TipoRegistro registroLido;

    rewind(arquivo);

    inicializa(Arvore);

    for (int i = 0; i < quantidade; i++)
    {
        fread(&registroLido, sizeof(TipoRegistro), 1, arquivo);
        (*contTransferencias)++;

        Insere(registroLido, Arvore, contComparacoes);
    }

    // PesquisaArvoreB(registro, Arvore, contComparacoes, achouRegistro);
}
