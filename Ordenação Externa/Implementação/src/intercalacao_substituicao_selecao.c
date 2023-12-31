#include "../headers/intercalacao_substituicao_selecao.h"


//Intercalação por substituição  ----------------------------------------




//Definindo tamanho dos blocos e do heap
#define TAM 10       // heap
#define NUM_BLOCK 20 // numero de blocos

void intercalacao_substituicao_selecao(char argv[], int nro_quantidade, bool imprimir_dados)
{
    // ----- Printf de início de execução.
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "\nIniciando" ANSI_RESET " o processo de intercalacao por ordenacao interna. Aguarde...\n");

    // ----- Arquivos de entrada.
    FILE *arquivo_texto, *arquivo_bin;

    // ----- Conversão do arquivo .txt para .bin.
    arquivo_texto = abrir_arquivo(argv, "r");
    converter_para_binario(arquivo_texto, "arquivo_binario.bin", argv, nro_quantidade);
    fclose(arquivo_texto);

    // ----- Início do Intercalação Ordenação Interna.

    // ----- Início da contagem de tempo.
    clock_t t = clock();

    // ----- Criada tabela das estatisticas de execução.
    Estatistica estatistica = {0, 0, 0, 0, 0.0F};

    // ----- Declaração das fitas.
    Fitas fitas = cria_fitas();

    // ----- Vetores auxiliares de fitas.
    Fita *entrada;
    Fita *saida;
    Fita *aux;

    // ----- Contadores.
    int contador_fitas, i;

    // ----- Tenta abrir o arquivo binário para leitura.
    arquivo_bin = abrir_arquivo("arquivo_binario.bin", "r+b");

    // ----- Cria, ordena e escreve os blocos ordenados nas fitas de entrada.
    criar_blocos_ordenados_heap(arquivo_bin, nro_quantidade, &fitas, &estatistica, imprimir_dados);

    // ----- Executa rewind em todas as fita.
    fitas_rewind(&fitas);

    // ----- Inicializa os vetores auxiliares com
    // ----- os vetores respectivos de fitas.
    entrada = fitas.entrada;
    saida = fitas.saida;

    // ----- Realiza a intercalação, invertendo as fitas
    // ----- de saida e entrada até que a apenas
    // ----- a primeira fita esteja preenchida.
    do
    {
        // Inicia o contador de fitas preenchidas em 0.
        contador_fitas = 0;

        // Realiza a intercalação balanceada, recebendo 2 vetores de Fita.
        intercalacao_balanceada_heap(entrada, saida, nro_quantidade, &estatistica, imprimir_dados);

        // Reinicia os contadores de fita.
        fitas_rewind(&fitas);

        // Troca as fitas de entrada e saida.
        // A de saida vira de entrada.
        // E a entrada vira de saida.
        aux = entrada;
        entrada = saida;
        saida = aux;

        // Conta quantas fitas estão preenchidas.
        for (i = 0; i < NUM_FITAS_ENTRADA; i++)
            contador_fitas += (entrada[i].preenchida) ? 1 : 0;
        
        // Continua enquanto não tiver restando apenas 1 fita
        // preenchida, com todos os registro ordenados.
    } while (contador_fitas != 1);

    // Printf de conclusão da ordenação interna.
    printf(ANSI_BOLD ANSI_COLOR_GREEN "\nArquivo ordenado com sucesso!\n" ANSI_RESET);
    // Printa os valores de resultado
    if (imprimir_dados)
        print_resultado_ordenacao(entrada[0].arquivo);

    // Para o contador de tempo e calcula os segundos passados.
    t = clock() - t;
    double tempo_execucao = ((double)t) / CLOCKS_PER_SEC;
    // Leva os segundos para a tabela de estatistica.
    estatistica.tempo_execucao = tempo_execucao;

    // Printa a tabela de estatistica com os valores adiquiridos.
    print_estatisticas(estatistica.nro_comparacoes_ord_externa, estatistica.nro_comparacoes_ord_interna, estatistica.nro_leituras, estatistica.nro_escritas, estatistica.tempo_execucao);

    // Covnerte a fita da ultima intercalação de .bin para .txt.
    converter_para_txt(entrada[0].arquivo, "RESULTADO.TXT", nro_quantidade);

    // Fecha e remove as fitasdo programa.
    remove_fitas(&fitas);
}

// Função para criar blocos ordenados usando o algoritmo Heapsort
void criar_blocos_ordenados_heap(FILE *arquivo_binario, int num_registros, Fitas *fitas, Estatistica *estatistica, bool imprimir_dados)
{
    Registro mem_interna[TAM_BLOCO];
    rewind(arquivo_binario);

    int num_reg_marcados = 0;
    int num_total_reg_lidos = 0;
    int num_total_reg_escritos = 0;
    int num_atual_reg_escritos = 0;

    Registro anterior;
    Registro nulo = novo_reg_nulo();

    int k, i, j = 0;

    // Mensagem de início da criação de blocos ordenados
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "\nCriando" ANSI_RESET " blocos ordenados. Aguarde...\n");

    // Lê um bloco de registros do arquivo binário
    fread(mem_interna, sizeof(Registro), TAM_BLOCO, arquivo_binario);
    if (imprimir_dados)
        for (k = 0; k < TAM_BLOCO; k++)
            PrintFRead(&mem_interna[k]);

    num_total_reg_lidos += TAM_BLOCO;

    // Marca os registros como não marcados (f = 0)
    for (i = 0; i < TAM_BLOCO; i++)
        mem_interna[i].f = 0;

    // Enquanto houver registros não lidos
    while (num_total_reg_lidos < num_registros)
    {
        num_reg_marcados = 0;
        num_atual_reg_escritos = 0;

        // Marca os registros do bloco como não marcados (f = 0)
        for (i = 0; i < TAM_BLOCO; i++)
            mem_interna[i].f = 0;

        // Enquanto houver registros não marcados no bloco
        while (num_reg_marcados < TAM_BLOCO && num_total_reg_lidos < num_registros)
        {
            // Ordena o bloco internamente usando o Heapsort
            heapsort(mem_interna, TAM_BLOCO, estatistica);

            // Guarda o registro anterior para comparação
            anterior = mem_interna[0];

            // Escreve o menor registro na fita de entrada
            fwrite(&mem_interna[0], sizeof(Registro), 1, fitas->entrada[j].arquivo);
            if (imprimir_dados) // Imprime instrução de fwrite se imprimir_dados for verdadeiro
                PrintFWrite(&mem_interna[0]);

            num_atual_reg_escritos++;
            num_total_reg_escritos++;

            // Lê o próximo registro do arquivo binário
            fread(&mem_interna[0], sizeof(Registro), 1, arquivo_binario);
            if (imprimir_dados)
                PrintFRead(&mem_interna[0]);

            num_total_reg_lidos++;
            mem_interna[0].f = 0;

            // Marca o registro atual se for menor ou igual ao anterior
            if (mem_interna[0].nota <= anterior.nota)
            {   
                num_reg_marcados++;
                mem_interna[0].f = 1;
            }
        }

        // Atualiza as informações da fita de entrada
        fitas->entrada[j].quant_itens += num_atual_reg_escritos;
        fitas->entrada[j].tam_ultimo_bloco = num_atual_reg_escritos;
        fitas->entrada[j].preenchida = true;

        // Escreve o registro nulo no final da fita de entrada
        fwrite(&nulo, sizeof(Registro), 1, fitas->entrada[j].arquivo);

        // Avança para a próxima fita de entrada circularmente
        j = (j + 1) % NUM_FITAS_ENTRADA;
    }

    // Se todos os registros foram lidos, ordena e escreve o último bloco
    if (num_total_reg_lidos == num_registros)
    {
        // Marca os registros do último bloco como não marcados (f = 0)
        for (i = 0; i < TAM_BLOCO; i++)
            mem_interna[i].f = 0;

        // Ordena o último bloco internamente usando o Heapsort
        heapsort(mem_interna, TAM_BLOCO, estatistica);

        // Escreve o último bloco ordenado na fita de entrada
        fwrite(&mem_interna, sizeof(Registro), TAM_BLOCO, fitas->entrada[j].arquivo);
        if (imprimir_dados)
            for (k = 0; k < TAM_BLOCO; k++)
                PrintFWrite(&mem_interna[k]);

        // Atualiza as informações da fita de entrada
        fitas->entrada[j].quant_itens += TAM_BLOCO;
        fitas->entrada[j].tam_ultimo_bloco = TAM_BLOCO;
        fitas->entrada[j].preenchida = true;

        // Escreve o registro nulo no final da fita de entrada
        fwrite(&nulo, sizeof(Registro), 1, fitas->entrada[j].arquivo);
    }

    // Atualiza as estatísticas de leituras e escritas
    estatistica->nro_leituras += num_registros;
    estatistica->nro_escritas += num_registros;

    // Mensagem de conclusão da criação de blocos ordenados
    printf(ANSI_BOLD ANSI_COLOR_RED "\nBlocos" ANSI_RESET " ordenados com " ANSI_COLOR_GREEN "sucesso!\n" ANSI_RESET);
}


// Função para realizar a intercalação balanceada usando o algoritmo de Heapsort
void intercalacao_balanceada_heap(Fita *origem, Fita *destino, int nro_registros, Estatistica *estatistica, bool imprimir_dados)
{   
    static int count = 0;
    count++;

    // Mensagem de início da intercalação de blocos
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "\nIniciando" ANSI_RESET " a %d° intercalacao de blocos. Aguarde...\n", count);
    
    int i, j = 0;

    int num_total_leituras = 0;
    int num_atual_escritas = 0;

    int pos_menor = -1;
    int quant_validos = 0;
    
    // Criação de um registro nulo para comparações
    Registro nulo = novo_reg_nulo();
    Registro mem_interna[TAM_BLOCO] = {nulo};

    bool fitas_preenchidas[NUM_FITAS_ENTRADA] = {false};
    bool fitas_disponiveis[NUM_FITAS_ENTRADA] = {false};

    // Enquanto ainda houver registros para ler
    while (num_total_leituras < nro_registros)
    {    
        num_atual_escritas = 0;

        // Inicializa os arrays de fitas preenchidas e disponíveis
        for (i = 0; i < NUM_FITAS_ENTRADA; i++)
        {
            fitas_preenchidas[i] = (origem[i].preenchida);
            fitas_disponiveis[i] = (origem[i].preenchida);
        }

        // Lê um registro de cada fita de entrada e armazena em memória interna
        for (i = 0; i < NUM_FITAS_ENTRADA; i++)
        {
            if (fitas_preenchidas[i])
            {
                fread(&mem_interna[i], sizeof(Registro), 1, origem[i].arquivo);
                if (imprimir_dados)
                    PrintFRead(&mem_interna[i]);
                estatistica->nro_leituras++;
                origem[pos_menor].quant_itens--;
                num_total_leituras++;
            }
            else
                mem_interna[i] = nulo;
        }
        
        // Enquanto houver fitas disponíveis para leitura
        while (alguma_fita_disponivel(fitas_disponiveis, NUM_FITAS_ENTRADA))
        {   
            // Encontra a posição do menor registro válido na memória interna
            pos_menor = acha_posicao_menor_reg_heap(mem_interna, TAM_BLOCO, &quant_validos);
            estatistica->nro_comparacoes_ord_externa += quant_validos;
            if(pos_menor == -1)
                break;

            // Escreve o menor registro na fita de destino
            fwrite(&mem_interna[pos_menor], sizeof(Registro), 1, destino[j].arquivo);
            if (imprimir_dados)
                PrintFWrite(&mem_interna[pos_menor]);
            estatistica->nro_escritas++;
            num_atual_escritas++;

            // Se a fita de origem está disponível, lê um novo registro
            if(fitas_disponiveis[pos_menor]){
                fread(&mem_interna[pos_menor], sizeof(Registro), 1, origem[pos_menor].arquivo);
                estatistica->nro_leituras++;
                if (imprimir_dados)
                    PrintFRead(&mem_interna[pos_menor]);
                num_total_leituras++;
                origem[pos_menor].quant_itens--;

                // Se o registro lido for o nulo, marca a fita como indisponível
                if(mem_interna[pos_menor].nota == nulo.nota)
                {   
                    estatistica->nro_leituras--;
                    origem[pos_menor].quant_itens++;
                    num_total_leituras--;
                    fitas_disponiveis[pos_menor] = false;

                    // Se a fita estiver vazia, marca como não preenchida
                    if(origem[pos_menor].quant_itens == 0){
                        origem[pos_menor].preenchida = false;
                        fitas_preenchidas[pos_menor] = false;
                    }
                }

            } else {
                estatistica->nro_leituras--;
                
                // Se a fita estiver vazia, marca como não preenchida
                if(origem[pos_menor].quant_itens == 0){
                    origem[pos_menor].preenchida = false;
                    fitas_preenchidas[pos_menor] = false;
                }
            }
        }

        // Atualiza as informações da fita de destino
        destino[j].preenchida = true;
        destino[j].quant_itens = num_atual_escritas;
        destino[j].tam_ultimo_bloco = num_atual_escritas;

        // Escreve o registro nulo no final da fita de destino
        fwrite(&nulo, sizeof(Registro), 1, destino[j].arquivo);

        // Avança para a próxima fita de destino circularmente
        j = (j+1) % NUM_FITAS_ENTRADA;
    }

    // Marca todas as fitas de origem como não preenchidas
    for (i = 0; i < NUM_FITAS_ENTRADA; i++)
        origem[i].preenchida = false;

    // Mensagem de conclusão da intercalação de blocos
    printf(ANSI_BOLD ANSI_COLOR_RED "\nIntercalacao" ANSI_RESET " realizada com " ANSI_COLOR_GREEN "sucesso!\n" ANSI_RESET);
}

// Função para encontrar a posição do menor registro válido no vetor usando o algoritmo de Heapsort
int acha_posicao_menor_reg_heap(Registro *vetor, int tam, int *quant_fitas)
{
    int i, m = -1;
    float menor = __FLT_MAX__;
    (*quant_fitas) = 0;

    for (i = 0; i < tam; i++)
    {
        if (vetor[i].nota >= 0.0F)
        {   
            (*quant_fitas)++;
            if(vetor[i].nota < menor)
            {   
                m = i;
                menor = vetor[i].nota;
            }
        }
    }

    return m;
}

// Função para verificar se pelo menos uma fita está disponível para leitura
bool alguma_fita_disponivel(bool *vetor, int tam)
{
    int i;
    for (i = 0; i < tam; i++)
        if (vetor[i])
            return true;

    return false;
}
