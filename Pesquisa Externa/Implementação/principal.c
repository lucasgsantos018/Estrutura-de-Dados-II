#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "base.h"
#include "asi.h"
#include "arvoreB.h"
#include "ae.h"
#include "arvoreBestrela.h"


int main (int argc, const char *argv[]) {

    int metodo = atoi(argv[1]);
    long int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);

    int contComparacoes = 0;
    int contTransferencias = 0;
    int controlePrintRegistro = 0;

    clock_t tempo_pesquisa;
    clock_t tempo_processamento;

    char nomeArquivo[5];

    if(quantidade <= 0){
        printf("ERRO: O arquivo deve ter no mínimo UM registro\n");
        exit(1);
    }

    if (argc < 5){
        printf("ERRO: Poucos comandos digitados\n");
        exit(1);
    }

    if (argc > 6){
        printf("ERRO: Muitos comandos digitados\n");
        exit(1);
    }

    if (metodo < 1 || metodo > 4){
        printf ("ERRO: Método inválido!\n");
        printf ("O método deve ser 1 para sequencial indexado, 2 para árvore binária, 3 para árvore B e 4 para árvore B*\n");
        exit(1);
    }

    if (situacao < 1 || situacao > 3){
        printf("ERRO: Situação inválida!\n");
        printf("A situação deve ser 1 para arquivo arquivo ordenado ascendentemente , 2 para arquivo ordenado descendentemente e 3 para arquivo desordenado aleatoriamente\n");
        exit(1);
    }

    if(argc == 6){
        if(strcmp(argv[5],"[-P]") == 0)
            controlePrintRegistro = 1;
        else{
            printf("ERRO: Argumento opcional deve ser digitado como [-P]\n.");
            exit(1);
        }
    }

    FILE *arquivo = NULL;

    switch (situacao){
        case 1:
            strcpy(nomeArquivo, "asce");

            if((arquivo = fopen(nomeArquivo, "rb")) == NULL){
                printf("ERRO: Arquivo ascendente não abriu.\n");
                return 0;
            }
            break;

        case 2:
            strcpy(nomeArquivo, "desc");

            if((arquivo = fopen(nomeArquivo, "rb")) == NULL){
                printf("ERRO: Arquivo descendente não abriu.\n");
                return 0;
            }
            break;
        
        case 3:
            strcpy(nomeArquivo, "alea");

            if((arquivo = fopen(nomeArquivo, "rb")) == NULL){
                printf("ERRO: Arquivo aleatorio não abriu.\n");
                return 0;
            }
            break;
    }

    TipoRegistro x, r;

    r.chave = chave;
    tipoindice tabela[MAXTABELA];

    int achouRegistro = 0,pos=0;
    int quanti=quantidade;
    TipoApontador Arvore = NULL;

    TipoApontadorSec apb = NULL;

    switch (metodo){
        case 1: // Sequencial Indexado

            if (situacao != 1){
                printf("Erro: Acesso sequencial indexado precisa de um arquivo ordenado ascendentemente\n");
                exit(1);
            }
            tempo_processamento = clock();

            while ((fread(&x, sizeof(x), 1, arquivo)) == 1 && (quanti>0)){
                contTransferencias++; // Nao há um ++ fora do while pois são apenas transferencias e não checagens;
				quanti--;
                fseek(arquivo, (ITENSPAGINA - 1) * sizeof(TipoRegistro), SEEK_CUR);
				tabela[pos].chave = x.chave;
				tabela[pos].posicao = pos+1;
				pos++;

            }
            tempo_processamento = clock() - tempo_processamento;
            
            achouRegistro = pesquisaSequencial(tabela, pos, &r, arquivo, controlePrintRegistro, &contComparacoes, &contTransferencias);
            
            tempo_pesquisa = clock();
            if(chave > quantidade){
                printf("Não há registro com essa chave\n");
            }
            else if(achouRegistro){
                printf("Registro encontrado\n");
                imprimeRegistro(r);
            }else{
                printf("Não há registro com essa chave\n");
            }
             tempo_pesquisa = clock() - tempo_pesquisa;
            break;

        case 2: // Árvore binária
            criaArvoreExt(arquivo, chave, quantidade, &contComparacoes, &contTransferencias, &achouRegistro, &tempo_pesquisa, &tempo_processamento);
            break;

        case 3: // Árvore B

            tempo_processamento = clock();

            ArvoreB(&Arvore, arquivo, quantidade, &r, &contComparacoes, &contTransferencias, &achouRegistro);

            tempo_processamento = clock() - tempo_processamento;

            tempo_pesquisa = clock();
            PesquisaArvoreB(&r, Arvore, &contComparacoes, &achouRegistro);
            if (achouRegistro)
            {
                printf("Registro encontrado\n");
                imprimeRegistro(r);
            }
            else
            {
                printf("Nao ha registro com essa chave\n");
            }

            tempo_pesquisa = clock() - tempo_pesquisa;

            break;

        case 4: // Árvore B*

            tempo_processamento = clock();

            criaArvoreBEstrela(arquivo, &apb, quantidade, &contComparacoes, &contTransferencias);

            tempo_processamento = clock() - tempo_processamento;

            tempo_pesquisa = clock();

            pesquisaArvoreBEstrela(&r, &apb, &contComparacoes, &contTransferencias, &achouRegistro);
            if (achouRegistro)
            {
                printf("Registro encontrado\n");
                imprimeRegistro(r);
            }
            else
            {
                printf("Nao ha registro com essa chave\n");
            }

            tempo_pesquisa = clock() - tempo_pesquisa;
            break;
    }

    if(controlePrintRegistro == 1){
        imprimeChaves(arquivo, quantidade);
    }

   imprimirTotal(contComparacoes, contTransferencias, tempo_pesquisa, tempo_processamento);

   fclose(arquivo);

   return 0;
}