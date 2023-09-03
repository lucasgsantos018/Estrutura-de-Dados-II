#ifndef GERAL_H
#define GERAL_H

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Definições de cores ANSI para formatação de texto
#define ANSI_BOLD "\x1b[1m"
#define ANSI_COLOR_RED "\e[1;91m"
#define ANSI_COLOR_YELLOW "\e[1;93m"
#define ANSI_RESET "\x1b[0m"
#define ANSI_COLOR_BLUE "\e[1;94m"
#define ANSI_COLOR_GREEN "\e[1;92m"

// Estrutura de dados para armazenar informações de um registro
typedef struct registro
{
    long int inscricao;
    float nota;
    char estado_cidade_curso[87];
    int f;
} Registro;

// Estrutura de dados para armazenar estatísticas da execução
typedef struct {
    int nro_comparacoes_ord_interna;
    int nro_comparacoes_ord_externa;
    int nro_leituras;
    int nro_escritas;
    double tempo_execucao;
} Estatistica;

// Função para verificar parâmetros de entrada
void verificar_parametros(int argc, int nro_metodo, int nro_quantidade, int nro_situacao);

// Função para abrir um arquivo com um determinado modo de abertura
FILE *abrir_arquivo(char nome_arquivo[], char modo_abertura[]);

// Funções para conversão entre formato de arquivo de texto e binário
void converter_para_binario(FILE *arquivo_texto, char *nome_binario, char *nome_txt, int nro_quantidade);
void converter_para_txt(FILE *arquivo_binario, char *argv, int nro_registros);

// Funções de ordenação
void merge_sort(Registro *vetor, int margem_esquerda, int n, Estatistica *estatistica);
void merge_sort_ascendente(Registro *vetor, int margem_esquerda, int nova_margem, int n, Estatistica *estatistica);
void heapsort(Registro *vet, int n, Estatistica *estatistica);
void peneira(Registro *vet, int raiz, int fundo, Estatistica *estatistica);
void selection_sort(Registro *array, int n, Estatistica *estatistica);
void insertion_sort(Registro *array, int n, Estatistica *estatistica);

// Função para imprimir estatísticas
void print_estatisticas(int nro_comparacoes_ord_interna, int nro_comparacoes_ord_externa, int nro_leituras, int nro_escritas, double tempo_execucao);

// Funções para impressão de registros
void PrintFRead(Registro *R);
void PrintFWrite(Registro *R);

// Função para imprimir resultado da ordenação
void print_resultado_ordenacao(FILE *arquivo_binario);

// Função para criar um novo registro nulo
Registro novo_reg_nulo();

#endif // GERAL_H
