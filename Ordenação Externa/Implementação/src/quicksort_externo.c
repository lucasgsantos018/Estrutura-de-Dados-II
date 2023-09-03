#include "../headers/geral.h"
#include "../headers/quicksort_externo.h"


//QUICK SORT EXTERNO (COM BASE NO CÓDIGO FORNECIDO PELO PROFESSOR)


Estatistica estatistica = {0, 0, 0, 0, 0.0F};

// Função principal para executar o Quicksort Externo
void quicksort_externo_main(char argv[], int nro_quantidade, bool imprimir_dados)
{
    FILE *ArqLi, *ArqEi, *ArqLEs, *ArqTXT;

    // Abrir o arquivo de entrada no modo leitura
    ArqTXT = abrir_arquivo(argv, "r");

    // Converter o arquivo de entrada para formato binário
    converter_para_binario(ArqTXT, "arquivo_binario.bin", argv, nro_quantidade);

    // Fechar o arquivo de texto
    fclose(ArqTXT);

    // Início do Quicksort Externo
    ArqLi = abrir_arquivo("arquivo_binario.bin", "r+b");
    ArqEi = abrir_arquivo("arquivo_binario.bin", "r+b");
    ArqLEs = abrir_arquivo("arquivo_binario.bin", "r+b");

    // Imprimir informações sobre a ordenação
    print_ordenacao(imprimir_dados);

    // Iniciar a contagem do tempo de execução
    clock_t t = clock();

    // Chamar a função de Quicksort Externo
    quicksort_externo(&ArqLi, &ArqEi, &ArqLEs, 1, nro_quantidade, imprimir_dados);

    // Calcular o tempo de execução
    t = clock() - t;
    estatistica.tempo_execucao = ((double)t) / CLOCKS_PER_SEC;

    // Fim do Quicksort Externo

    // Imprimir o resultado da ordenação se a opção de imprimir dados estiver ativada
    if (imprimir_dados)
        print_resultado_ordenacao(ArqLi);

    // Imprimir as estatísticas da ordenação
    print_estatisticas(estatistica.nro_comparacoes_ord_interna, estatistica.nro_comparacoes_ord_externa, estatistica.nro_leituras, estatistica.nro_escritas, estatistica.tempo_execucao);

    // Converter o resultado para formato de texto
    converter_para_txt(ArqLi, "RESULTADO.TXT", nro_quantidade);

    // Fechar os arquivos
    fclose(ArqLi);
    fclose(ArqEi);
    fclose(ArqLEs);

    // Remover o arquivo binário temporário
    remove("arquivo_binario.bin");
}

// Função para executar o Quicksort Externo
void quicksort_externo(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir, bool imprimir_dados)
{
    int i, j;
    TipoArea Area;

    // Verifica se a quantidade de registros é válida para a ordenação
    if (Dir - Esq < 1)
        return;

    // Cria uma área auxiliar vazia para realizar a intercalação
    FAVazia(&Area);

    // Realiza a partição dos registros no arquivo
    Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j, imprimir_dados);

    // Decide qual parte ordenar primeiro, otimizando as chamadas recursivas
    if (i - Esq < Dir - j)
    {
        quicksort_externo(ArqLi, ArqEi, ArqLEs, Esq, i, imprimir_dados);
        quicksort_externo(ArqLi, ArqEi, ArqLEs, j, Dir, imprimir_dados);
    }
    else
    {
        quicksort_externo(ArqLi, ArqEi, ArqLEs, j, Dir, imprimir_dados);
        quicksort_externo(ArqLi, ArqEi, ArqLEs, Esq, i, imprimir_dados);
    }
}


// Função para realizar a partição no Quicksort Externo
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j, bool imprimir_dados)
{
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq;
    double Linf = INT_MIN, Lsup = INT_MAX;
    bool OndeLer = true;
    Registro UltLido, R;

    // Posiciona os ponteiros dos arquivos e inicializa variáveis
    fseek(*ArqLi, (Li - 1) * sizeof(Registro), SEEK_SET);
    fseek(*ArqEi, (Ei - 1) * sizeof(Registro), SEEK_SET);
    *i = Esq - 1;
    *j = Dir + 1;

    while (Ls >= Li)
    {
        // Verifica se há espaço na área para leitura direta
        if (Area.nro_cels_ocupadas < TAM_AREA - 1)
        {
            if (OndeLer)
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, imprimir_dados);
            else
                LeInf(ArqLi, &UltLido, &Li, &OndeLer, imprimir_dados);

            InsereItem(&UltLido, &Area);

            continue;
        }

        // Seleciona qual registro ler e realizar a partição
        if (Ls == Es)
        {
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, imprimir_dados);
        }
        else if (Li == Ei)
        {
            LeInf(ArqLi, &UltLido, &Li, &OndeLer, imprimir_dados);
        }
        else if (OndeLer)
        {
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, imprimir_dados);
        }
        else
            LeInf(ArqLi, &UltLido, &Li, &OndeLer, imprimir_dados);

        // Realiza comparações e particiona
        estatistica.nro_comparacoes_ord_externa++;
        if (UltLido.nota > Lsup)
        {
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es, imprimir_dados);
            continue;
        }
        estatistica.nro_comparacoes_ord_externa++;
        if (UltLido.nota < Linf)
        {
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei, imprimir_dados);
            continue;
        }

        InsereItem(&UltLido, &Area);

        // Realiza o balanceamento da área
        if (Ei - Esq < Dir - Es)
        {
            RetiraPrimeiro(&Area, &R);
            EscreveMin(ArqEi, R, &Ei, imprimir_dados);
            Linf = R.nota;
        }
        else
        {
            RetiraUltimo(&Area, &R);
            EscreveMax(ArqLEs, R, &Es, imprimir_dados);
            Lsup = R.nota;
        }
    }
    
    // Realiza balanceamento final da área
    while (Ei <= Es)
    {
        RetiraPrimeiro(&Area, &R);
        EscreveMin(ArqEi, R, &Ei, imprimir_dados);
    }
}


//Funções de leitura -------------------------------------

void LeSup(FILE **ArqLEs, Registro *UltLido, int *Ls, bool *OndeLer, bool imprimir_dados)
{
    fseek(*ArqLEs, (*Ls - 1) * sizeof(Registro), SEEK_SET);
    estatistica.nro_leituras++;
    fread(UltLido, sizeof(Registro), 1, *ArqLEs);
    if (imprimir_dados)
        PrintFRead(UltLido);
    (*Ls)--;
    *OndeLer = false;
}

void LeInf(FILE **ArqLi, Registro *UltLido, int *Li, bool *OndeLer, bool imprimir_dados)
{
    estatistica.nro_leituras++;
    fread(UltLido, sizeof(Registro), 1, *ArqLi);
    if (imprimir_dados)
        PrintFRead(UltLido);
    (*Li)++;
    *OndeLer = true;
}


//Funções de inserção e retirada ----------------------------------
void InsereItem(Registro *UltLido, TipoArea *Area)
{
    Area->array[Area->nro_cels_ocupadas] = *UltLido;
    Area->nro_cels_ocupadas++;
    insertion_sort(Area->array, Area->nro_cels_ocupadas, &estatistica);
}

void RetiraUltimo(TipoArea *Area, Registro *R)
{
    *R = Area->array[Area->nro_cels_ocupadas - 1];
    Area->array[Area->nro_cels_ocupadas - 1].nota = INT_MAX;
    Area->nro_cels_ocupadas--;
}

void RetiraPrimeiro(TipoArea *Area, Registro *R)
{
    *R = Area->array[0];
    Area->array[0].nota = INT_MAX;
    insertion_sort(Area->array, Area->nro_cels_ocupadas, &estatistica);
    Area->nro_cels_ocupadas--;
}


//Funções de escrita -------------------------------------------------------------------

void EscreveMax(FILE **ArqLEs, Registro R, int *Es, bool imprimir_dados)
{
    fseek(*ArqLEs, (*Es - 1) * sizeof(Registro), SEEK_SET);
    estatistica.nro_escritas++;
    fwrite(&R, sizeof(Registro), 1, *ArqLEs);
    if (imprimir_dados)
        PrintFWrite(&R);
    (*Es)--;
}

void EscreveMin(FILE **ArqEi, Registro R, int *Ei, bool imprimir_dados)
{
    estatistica.nro_escritas++;
    fwrite(&R, sizeof(Registro), 1, *ArqEi);
    if (imprimir_dados)
        PrintFWrite(&R);
    (*Ei)++;
}

void FAVazia(TipoArea *Area)
{
    for (int i = 0; i < TAM_AREA; i++)
        Area->array[i].nota = INT_MAX;

    Area->nro_cels_ocupadas = 0;
}

//Funções de impressão-------------------------------------------------------------------------

void PrintArea(TipoArea Area)
{
    printf("\n     - Area -\n");
    for (int i = 0; i < TAM_AREA; i++)
        printf("%08ld %05.1f%s\n", Area.array[i].inscricao, Area.array[i].nota, Area.array[i].estado_cidade_curso);
    printf("Nº de cels ocupadas: %d\n\n", Area.nro_cels_ocupadas);
}

void print_ordenacao(bool imprimir_dados)
{
    printf(ANSI_COLOR_YELLOW "Ordenando" ANSI_RESET " o arquivo binário. Aguarde...\n");
    if (imprimir_dados)
        printf("\n");
}