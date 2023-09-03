#include "../headers/geral.h"
#include "../headers/quicksort_externo.h"

// Função de ordenação Merge Sort
void merge_sort(Registro *vetor, int margem_esquerda, int n, Estatistica *estatistica)
{
    // Caso base: se a margem esquerda for menor que a margem direita
    if (margem_esquerda < n)
    {
        // Calcula a nova margem para dividir o vetor em dois subvetores
        int nova_margem = (margem_esquerda + n) / 2;

        // Chama a função merge_sort recursivamente para ordenar a metade esquerda
        merge_sort(vetor, margem_esquerda, nova_margem, estatistica);

        // Chama a função merge_sort recursivamente para ordenar a metade direita
        merge_sort(vetor, nova_margem + 1, n, estatistica);

        // Combina as metades ordenadas usando a função merge_sort_ascendente
        merge_sort_ascendente(vetor, margem_esquerda, nova_margem, n, estatistica);
    }
}

// Função de merge ascendente utilizada pelo Merge Sort
void merge_sort_ascendente(Registro *vetor, int margem_esquerda, int nova_margem, int n, Estatistica *estatistica)
{
    // Calcula o tamanho dos subvetores esquerdo e direito
    int tamanho_esquerda = (nova_margem - margem_esquerda + 1);
    int tamanho_direita = (n - nova_margem);
    
    // Aloca memória para os subvetores temporários
    Registro *vetor_esquerda = (Registro *)malloc(tamanho_esquerda * sizeof(Registro));
    Registro *vetor_direita = (Registro *)malloc(tamanho_direita * sizeof(Registro));
    int i, j;

    // Copia os elementos dos subvetores originais para os subvetores temporários
    for (i = 0; i < tamanho_esquerda; i++)
        vetor_esquerda[i] = vetor[i + margem_esquerda];

    for (j = 0; j < tamanho_direita; j++)
        vetor_direita[j] = vetor[nova_margem + j + 1];

    i = 0, j = 0;

    // Combinando os subvetores temporários de volta ao vetor original em ordem ascendente
    for (int k = margem_esquerda; k <= n; k++)
    {
        // Verifica se a metade esquerda já foi completamente inserida
        if (i == tamanho_esquerda)
            vetor[k] = vetor_direita[j++];
        // Verifica se a metade direita já foi completamente inserida
        else if (j == tamanho_direita)
            vetor[k] = vetor_esquerda[i++];
        // Compara os elementos das duas metades e insere o menor no vetor original
        else if (vetor_esquerda[i].nota < vetor_direita[j].nota)
        {
            vetor[k] = vetor_esquerda[i++];
            estatistica->nro_comparacoes_ord_interna++;
        }
        else
        {
            vetor[k] = vetor_direita[j++];
            estatistica->nro_comparacoes_ord_interna++;
        }
    }

    // Libera a memória dos subvetores temporários
    free(vetor_direita);
    free(vetor_esquerda);
}



// Função de ordenação Heap Sort
void heapsort(Registro *vet, int n, Estatistica *estatistica) {
	Registro tmp;
    int i;

    // Construção do heap máximo
	for (i = ((n-1)/2); i >= 0; i--) {
		peneira(vet, i, n - 1, estatistica);
	}

    // Extração dos elementos do heap e reconstrução
	for (i = n - 1; i >= 1; i--) {
		// Troca o elemento máximo do heap (raiz) com o último elemento
        tmp = vet[0];
        vet[0] = vet[i];
        vet[i] = tmp;

        // Reconstrói o heap com os elementos restantes
		peneira(vet, 0, i - 1, estatistica);
	}
}


// Função de peneira utilizada no Heap Sort
void peneira(Registro *vet, int raiz, int fundo, Estatistica *estatistica) {
    // Armazena o valor da raiz atual e calcula o índice do primeiro filho
    Registro aux = vet[raiz];
    int j = raiz * 2 + 1;

    // Percorre o subvetor a partir da raiz até o fundo
    while (j <= fundo) {
        // Verifica se existe um segundo filho e realiza comparações
        if (j < fundo) {
            estatistica->nro_comparacoes_ord_interna++;
            // Compara o elemento atual com o próximo filho e verifica se deve trocar
            if ((vet[j].nota < vet[j + 1].nota || vet[j].f == 1) && vet[j].f == 0) {
                j = j + 1;
            }
        }

        // Compara o valor da raiz com o valor do filho escolhido
        estatistica->nro_comparacoes_ord_interna++;
        if ((aux.nota < vet[j].nota || vet[j].f == 1) && aux.f == 0) {
            // Troca o elemento da raiz pelo filho e atualiza os índices
            vet[raiz] = vet[j];
            raiz = j;
            j = 2 * raiz + 1;
        } else {
            // Caso contrário, termina a peneiração
            j = fundo + 1;
        }
    }
    
    // Coloca o valor armazenado anteriormente na posição correta
    vet[raiz] = aux;
}


// Função de ordenação Insertion Sort
void insertion_sort(Registro *array, int n, Estatistica *estatistica)
{
    int i, j;
    Registro aux;

    // Percorre o vetor da segunda posição até o final
    for (i = 1; i < n; i++)
    {
        // Armazena o elemento atual para inserção na posição correta
        aux = array[i];
        j = i - 1;

        // Move os elementos maiores que 'aux' para a direita
        while (j >= 0 && array[j].nota > aux.nota)
        {
            estatistica->nro_comparacoes_ord_interna++;
            array[j + 1] = array[j];
            j--;
        }

        // Insere 'aux' na posição correta
        array[j + 1] = aux;
    }
}

// Função de ordenação Selection Sort
void selection_sort(Registro *array, int n, Estatistica *estatistica)
{
    Registro aux;
    int min;

    // Percorre o vetor do início ao penúltimo elemento
    for (int i = 0; i < n - 1; i++)
    {
        // Encontra o índice do menor elemento não ordenado
        min = i;
        for (int j = i + 1; j < n; j++)
        {
            estatistica->nro_comparacoes_ord_interna++;
            // Compara os elementos e atualiza o índice do menor
            if (array[j].nota < array[min].nota)
                min = j;
        }
        
        // Troca o menor elemento com o elemento na posição atual, se necessário
        if (i != min)
        {
            aux = array[i];
            array[i] = array[min];
            array[min] = aux;
        }
    }
}


// Função para criar e retornar um registro nulo
Registro novo_reg_nulo(){
    Registro nulo;

    // Atribui valores padrão ao registro nulo
    strcpy(nulo.estado_cidade_curso, "nulo"); // Define o campo estado_cidade_curso como "nulo"
    nulo.nota = -1.0F; // Define o campo nota como -1.0
    nulo.inscricao = -1; // Define o campo inscrição como -1
    nulo.f = -1; // Define o campo f como -1

    return nulo; // Retorna o registro nulo criado
}

// Função para verificar a validade dos parâmetros de entrada
void verificar_parametros(int argc, int nro_metodo, int nro_quantidade, int nro_situacao)
{
    // Verifica se o número de argumentos passados é insuficiente
    if (argc < 4)
    {
        printf("ERRO : Parâmetros insuficientes para a execução\n.");
        exit(1);
    }
    
    // Verifica se o número de método fornecido está dentro das opções válidas
    if (nro_metodo != 1 && nro_metodo != 2 && nro_metodo != 3)
    {
        printf("ERRO : Método fornecido não corresponde ao enunciado do trabalho. Por favor, corrija os parâmetros de execução.\n");
        exit(1);
    }
    
    // Verifica se a quantidade de registros fornecida está dentro dos limites estabelecidos
    if (nro_quantidade < 100 || nro_quantidade > 471705)
    {
        printf("ERRO : Quantidade de registros fornecida não corresponde ao enunciado do trabalho. Por favor, corrija os parâmetros de execução.\n");
        exit(1);
    }
    
    // Verifica se o número de situação fornecido está dentro das opções válidas
    if (nro_situacao != 1 && nro_situacao != 2 && nro_situacao != 3 && nro_situacao != 4)
    {
        printf("ERRO : Situação fornecida não corresponde ao enunciado do trabalho. Por favor, corrija os parâmetros de execução.\n");
        exit(1);
    }
}

FILE *abrir_arquivo(char nome_arquivo[], char modo_abertura[]) //Função para abrir um arquivo e retornar se foi possivel ou não
{
    FILE *abrir_arquivo = fopen(nome_arquivo, modo_abertura);
    if (abrir_arquivo == NULL)
    {
        printf("ERRO : Impossível abrir arquivo '%s' informado na execução.\n", nome_arquivo);
        exit(1);
    }

    return abrir_arquivo;
}

void converter_para_binario(FILE *arquivo_texto, char *nome_binario, char *nome_txt, int nro_quantidade) //Função para realizar a converção de um arquivo de txt para .bin
{
    Registro aux;

    printf(ANSI_BOLD "\nArquivo: " ANSI_COLOR_YELLOW "%s\n" ANSI_RESET, nome_txt);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "Convertendo" ANSI_RESET " o arquivo .txt para binário. Aguarde...\n");

    FILE *arquivo_binario = abrir_arquivo(nome_binario, "w+b");

    while (nro_quantidade--)
    {
        fscanf(arquivo_texto, "%ld %f", &aux.inscricao, &aux.nota);
        fgets(aux.estado_cidade_curso, 87, arquivo_texto);
        aux.estado_cidade_curso[85] = '\0';
        aux.f = 0;
        fwrite(&aux, sizeof(Registro), 1, arquivo_binario);
    }

    fclose(arquivo_binario);
}

void converter_para_txt(FILE *arquivo_binario, char *argv, int nro_registros)//Função para converter para .txt
{
    Registro aux;
    rewind(arquivo_binario);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "Convertendo" ANSI_RESET " o arquivo binário para .txt. Aguarde...\n");

    FILE *arquivo_txt = abrir_arquivo(argv, "w+");

    while (nro_registros--)
    {
        fread(&aux, sizeof(Registro), 1, arquivo_binario);
        fprintf(arquivo_txt, "%08ld %05.1f%s\n", aux.inscricao, aux.nota, aux.estado_cidade_curso);
    }

    printf(ANSI_BOLD "Resultado da ordenação impresso em: " ANSI_RESET ANSI_COLOR_YELLOW "RESULTADO.TXT" ANSI_RESET "\n\n");

    fclose(arquivo_txt);
}


//Funções para imprimir os resultados na tela ---------------------------------------------------------

void print_estatisticas(int nro_comparacoes_ord_interna, int nro_comparacoes_ord_externa, int nro_leituras, int nro_escritas, double tempo_execucao)
{
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "\n- ESTATÍSTICAS DA EXECUÇÃO -     \n" ANSI_RESET);
    printf(ANSI_BOLD "\nNº de comparações: %d (E : %d " ANSI_COLOR_YELLOW "&" ANSI_RESET ANSI_BOLD " I : %d)" ANSI_RESET "\n", nro_comparacoes_ord_externa + nro_comparacoes_ord_interna, nro_comparacoes_ord_externa, nro_comparacoes_ord_interna);
    printf(ANSI_BOLD "Nº de leituras(fread): %d" ANSI_RESET "\n", nro_leituras);
    printf(ANSI_BOLD "Nº de escritas(fwrite): %d" ANSI_RESET "\n", nro_escritas);
    printf(ANSI_BOLD "Tempo de execução: " ANSI_COLOR_YELLOW "%f" ANSI_RESET ANSI_BOLD " segundos" ANSI_RESET "\n\n", tempo_execucao);
}

void PrintFRead(Registro *R)
{
    printf(ANSI_BOLD ANSI_COLOR_BLUE "fread" ANSI_RESET "  : %08ld %05.1f%s\n", R->inscricao, R->nota, R->estado_cidade_curso);
}

void PrintFWrite(Registro *R)
{
    printf(ANSI_BOLD ANSI_COLOR_RED "fwrite" ANSI_RESET " : %08ld %05.1f%s\n", R->inscricao, R->nota, R->estado_cidade_curso);
}

void print_resultado_ordenacao(FILE *arquivo_binario){
    Registro aux;

    rewind(arquivo_binario);

    printf(ANSI_BOLD ANSI_COLOR_YELLOW "\n- RESULTADO DA ORDENAÇÃO -     \n\n" ANSI_RESET);

    while (fread(&aux, sizeof(Registro), 1, arquivo_binario) && aux.nota >= 0.0F){
        printf("%08ld %05.1f%s\n", aux.inscricao, aux.nota, aux.estado_cidade_curso);
    }
}