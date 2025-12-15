/*
	header.h
	Criado por: Bruno
	Data 26/11

	Nota: O núcledo da estrutura principal vai ficar aqui.
	Aqui deve ser incluido as .h e a main.c deve incluir esse arquivo [Samuel]

*/

// Incluido a estruturas.h, e os outros includes estão lá





#include "estruturas.h" // -- Não precisa incluir stdio.h aqui, já está em estruturas.h [Samuel]

// Para definir aonde os .txt estão
#define TARIFAS_PATH			"tarifas.txt"
#define ESTACIONAMENTOS_PATH	"estacionamentos.txt"
#define BIN_PATH				"dados.bin"



//=======================================================
// Utilitários
//=======================================================

void trimTexto(char* s) { //Remove espaços em branco no final de uma string
	
	if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r' || s[n-1] == ' ' || s[n-1] == '\t')) {
        s[--n] = '\0';
	}
}

FILE* abrirArquivo(const char* caminho, const char* modo) {     //Função pra abrir tanto .txt ("r", "w", "a", "r+", "w+", "a+") quanto .bin ("rb", "wb", "ab", "rb+", "wb+", "ab+")
	
    FILE* file = fopen(caminho, modo);
    if (!file) {
		printf("Erro ao abrir o arquivo: %s\n", caminho);
		return NULL;
    }
	return file;
}

int colunaChar_Indice(char letraParaNum) { // Converte os char pra int
    letraParaNum = toupper(letraParaNum);
    if (letraParaNum >= 'A' && letraParaNum <= 'Z') {
        return letraParaNum - 'A';
    }
    return - 1; // Retorna -1 se o caractere não for válido -- honestamente eu n entendi mt bem qual a diferença entre -1 e null, mas oh well [Samuel]
}

char colunaIndice_Char(const PARQUE* p, int intParaChar) {
    int limite = p ? p->filasPorPiso : MAX_FILA; //verifica se p e nulo, se for usa o maximo
    if (intParaChar >= 0 && intParaChar < limite) {
		return 'A' + intParaChar; //Manda de volta pra char -- Pode usar qnd for fazer printf [Samuel]
    }
}

int coordenadaValida(const PARQUE* p, int andar, char, char filaChar, int lugar) {

    if (!p) return 0;

    if (andar < 0 || andar >= p->pisos) return 0;

	int filaIdx = colunaChar_Indice(filaChar); // Mudei a função de converter q tava aqui dentro pra fora, por pura frescura [Samuel]

    //verifica se fila e valida↓
    if (filaIdx < 0 || filaIdx >= p->filasPorPiso) return 0;//verifica se lugar e valido↓
   
    if (lugar < 0 || lugar >= p->lugaresPorFila) return 0;
   
    return 1; //todas as coordenadas sao validas

} //fiz isto as 02:21 da manha, se estiver uma porcaria avisem. vou mas e pra cama -Bruno-   -- Fiz umas mudanças [Samuel]


//=======================================================
// Guardar Info
//=======================================================

void guardarBinario(SISTEMA* s) {

    if (!s) {
        prinft(stderr, "Erro ao guardar dados!!\n");
        return;
    }

    FILE* f = abrirArquivo(BIN_PATH, "wb");
    if (!f) {
        printf(stderr, "Erro ao abrir/criar ficheiro %s!!\n", BIN_PATH);
        fclose(f);
        return;
    }
    if (fclose(f) != 0) {
        printf(stderr, "Erro ao fechar ficheiro %s!!\n", BIN_PATH);
        return;
    }

	printf("Dados guardados com sucesso em: %s\n", BIN_PATH);
}


//=======================================================
// Leituras -- Prioriza ler .bin, caso contrario lê .txt
//=======================================================

int carregarBinario(SISTEMA* s) {
    if (!s) return 0;

	FILE* f = abrirArquivo(BIN_PATH, "rb");
    if (!f) return 0; //Caso n tenha o .bin


    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return 0;
    }
	long sz = ftell(f);
    if (sz < 0) {
        fclose(f);
        return 0;
    }
    if(fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return 0;
	}
    if (sz != sizeof(SISTEMA)) {    //Ficheiro corrompido ou deu merda na estrutura
        fclose(f);
        printf(stderr, "Aviso: tamanho do dados.bin inesperado (%ld). Lendo os dados de texto.\n", sz);
        return 0;
    }

    size_t lidos = fread(s, sizeof(SISTEMA), 1, f);
    fclose(f);

    if (lidos != 1) {
        printf(stderr, "Erro ao ler dados.bin. Lendo os dados de texto.\n");
        return 0;
    }

	printf("Dados carregados com sucesso do ficheiro binário.\n");
    return 1;
}

ResultadoLeitura leituraConstante(SISTEMA* s) {
    if (!s) return LER_FALHA_ABRIR;

    if (carregarBinario(s)) {// Tentar carregar o binário primeiro
        return LER_OK;
    }
    
	printf("Binário não disponível. Carregando dados de texto.\n"); // Abre o .txt se n tiver o .bin [Samuel]

    carregarTarifasDeFicheiro(s);
    carregarEstacionamentosDeFicheiro(s);

	return LER_OK;
}

ResultadoLeitura primeiraLeitura(SISTEMA* s){
    if (!s) return LER_FALHA_ABRIR;

	configurarParque(&s->parque); // Configura o parque (pede os valores pro usuario)

    //Abre os .txt
    carregarTarifasDeFicheiro(s);
    carregarEstacionamentosDeFicheiro(s);

    if (!guardarBinario(s) {
		printf(stderr, "Erro: Não foi possivel criar %s!\n", BIN_PATH);
		return LER_ERRO_IO;
    }
	return LER_OK;
}


//----------------------------------------------------
// Inicialização e carregamento de dados
//----------------------------------------------------
void configurarParque(PARQUE* p) { //Bruno
    //Verifica se o ponteiro e valido↓
    if (p == NULL) {
        fprintf(stderr, "Erro, ponteiro nulo.\n");
        return;
    }

	while (1) { //pergunta e ve se e valido a quantidade de pisos. usa um loop ate user dar un vakor valido, estes 3 blocos de codigo fazem da mesma forma
        printf("Numero de pisos (1-%d): ", MAX_PISO);
        if (scanf("%d", &p->pisos) == 1 && p->pisos >= 1 && p->pisos <= MAX_PISO) { //verifica se e valido
            break; //break para sair do while
        }
        fprintf(stderr, "Valor invalido, tente novamente.\n");
    }

	while (1) { //pergunta quantidade de filas por piso
        printf("Numero de filas por piso (1-%d): ", MAX_FILA);
        if (scanf("%d", &p->filasPorPiso) == 1 && p->filasPorPiso >= 1 && p->filasPorPiso <= MAX_FILA) { //verifica se e valido
            break; //break para sair do while
        }
        fprintf(stderr, "Valor invalido, tente novamente.\n");
    }

	while (1) { //pergunta quantidade de lugares por fila
        printf("Numero de lugares por fila (1-%d): ", MAX_LUGARES);
        if (scanf("%d", &p->lugaresPorFila) == 1 && p->lugaresPorFila >= 1 && p->lugaresPorFila <= MAX_LUGARES) { //verifica se e valido
            break; //break para sair do while
        }
        fprintf(stderr, "Valor invalido, tente novamente.\n");
    }

    //Pra todas as posicoes ate ao MAX, definimos o estado inicial.
    //Se a posição existe marca se como LUGAR_LIVRE.
    //Caso contrario, marcamos como LUGAR_INDISPONIVEL
    for (int a = 0; a < MAX_PISO; a++) {
        for (int f = 0; f < MAX_FILA; f++) {
            for (int l = 0; l < MAX_LUGARES; l++) { //percorre todos os pisos filas e lugares
                if (a < p->pisos && f < p->filasPorPiso && l < p->lugaresPorFila) {
                    p->mapa[a][f][l] = LUGAR_LIVRE;
                }
                else {
                    p->mapa[a][f][l] = LUGAR_INDISPONIVEL;
                } //Verifica se o lugar existe e marca como livre ou indisponivel   

            }
        }
    }
}

void carregarTarifasDeFicheiro(SISTEMA* s) //Bruno
{
	//verifica se o sistema e valido
    if (s == NULL) {
        fprintf(stderr, "Erro: sistema nulo.\n\n");
        return; //se nao for, nao carrega

    }

	FILE* f = abrirArquivo(TARIFAS_PATH, "r"); //abre o arquivo de tarifas em read only
    if (!f) { 
        return; //se falhar a abrir, sai
    }

    s->totalTarifas = 0;

    char linha[256];
	while (fgets(linha, sizeof(linha), f) != NULL) { //le todas as linhas do arquivo
        //ignora linhas em branco↓
        if (linha[0] == '\0' || linha[0] == '\n')
            continue;

		//etiqueta e valor (tirados do tarifas.txt)
        char valorStr[32] = { 0 };
        char etiqueta[16] = { 0 };

		if (sscanf(linha, "%15s %31s", etiqueta, valorStr) != 2) { //separa os dois valores diferentes, etiqueta e valor
			fprintf(stderr, "Linha de tarifa invalida: %s\n", linha); //mensagem de erro caso a linha nao esteja no formato correto
            continue; //passa pra a proxima linha
        }

        //vai converter a string para um float
        char* endptr = NULL;
        float valor = strtof(valorStr, &endptr);

		if (endptr == valorStr) { //verifica se o strtof falhou (strtof é o que converte string para float)
            fprintf(stderr, "Valor de tarifa invalido: %s", linha);
            continue;

        }

		//mensagem de erro se passar do maximo
        if (s->totalTarifas >= MAX_TARIFAS) {
            fprintf(stderr, "Limite MAX_TARIFAS atingido. Ignorando restante.\n");
            break;
        }

        //guarda
        TARIFARIO t;
        t.valor = valor;
        //garante que a string termina em nulk
        strncpy(t.etiqueta, etiqueta, sizeof(t.etiqueta) - 1);
        t.etiqueta[sizeof(t.etiqueta) - 1] = '\0';

        s->tarifas[s->totalTarifas++] = t;
    }

    fclose(f);
    printf("Tarifas carregadas com sucesso: %d\n", s->totalTarifas);
}

void carregarEstacionamentosDeFicheiro(SISTEMA* s) { //Bruno

    FILE* f = abrirArquivo(ESTACIONAMENTOS_PATH, "r");

    
    //verifica se o sistema e valido
    if (s == NULL) {
        fprintf(stderr, "Erro: sistema nulo.\n\n");
        return; //se nao for, nao carrega

    }
    FILE* f = abrirArquivo("estacionamentos.txt", "r");

    if (!f) {
        printf("Houve um erro ao abrir o ficheiro.\n");
        return;
    }

    s->totalEstacionamentos = 0;
    Estacionamento e;
    while (fscanf(f, "%d %s %s %d %d %d %d %f\n",
        &e.numEntrada,
        e.matricula,
        e.tipoVeiculo,
        &e.piso,
        &e.fila,
        &e.lugar,
        &e.horaEntrada,
        &e.valorPago) == 8) {
        if (s->totalEstacionamentos >= MAX_ESTACIONAMENTOS) {
            fprintf(stderr, "Limite MAX_ESTACIONAMENTOS ultrapassado.\n");
            break;

        }
        s->estacionamentos[s->totalEstacionamentos++] = e; //// adiciona 'e' na posição atual e incrementa o counter
    }
    fclose(f);
    printf("Estacionamentos carregados com sucesso: %d\n", s->totalEstacionamentos);

}


// Removi as funções de carregar tarifas e estacionamento. Vou converter em uma função que inicia a primeira leitura dos arquivos .atxt e depois dos arquivos em .bin [Samuel]

void inicializarSistema(SISTEMA* s) { //Bruno
    int a, f, l;

    //reset geral dos contadores
    s->totalTarifas = 0;
    s->totalEstacionamentos = 0;
    s->ultimoNumEntrada = 0;

    //configuração inicial (valores minimos validos)
    s->parque.pisos = 1;
    s->parque.filasPorPiso = 1;
    s->parque.lugaresPorFila = 1;

    //inicializar matriz do parque: todos os lugares livres
    for (a = 0; a < MAX_PISO; a++) {
        for (f = 0; f < MAX_FILA; f++) {
            for (l = 0; l < MAX_LUGARES; l++) {
                s->parque.mapa[a][f][l] = LUGAR_LIVRE;
            }
        }
    }

    //reset das tarifas agrupadas
    s->tarifasBase.valor_T1 = 0.0f;
    s->tarifasBase.valor_T2 = 0.0f;
    s->tarifasBase.valor_T3 = 0.0f;
    s->tarifasBase.valor_T4 = 0.0f;

    //reset das tarifas individuais
    for (a = 0; a < MAX_TARIFAS; a++) {
        s->tarifas[a].valor = 0.0f;
        strcpy(s->tarifas[a].etiqueta, "");
    }

    //reset geral da lista de estacionamentos
    for (a = 0; a < MAX_ESTACIONAMENTOS; a++) {
        s->estacionamentos[a].id = 0;
        s->estacionamentos[a].estado = LUGAR_LIVRE;
        strcpy(s->estacionamentos[a].matricula, "");
        strcpy(s->estacionamentos[a].dataEntrada, "");
        strcpy(s->estacionamentos[a].horaEntrada, "");
        strcpy(s->estacionamentos[a].dataSaida, "");
            }
        strcpy(s->estacionamentos[a].horaSaida, "");
    }

    // Carregar ficheiros de dados
    primeiraLeitura(s);
    leituraConstante(s);
} //esta funcao e basicamente "formatar" o sistema. Tudo volta ao "estado inicial"

//----------------------------------------------------
// Menus
//----------------------------------------------------
void mostrarMenuPrincipal() {

}

void mostrarOcupacaoPisos() {

}

//----------------------------------------------------
// Entrada de veículos
//----------------------------------------------------
int registarEntradaVeiculo(Sistema* s) { //Bruno
	if (!s) return -1;

    Estacionamento novo;

	novo.numEntrada = ++s->ultimoNumEntrada;

	printf("Matriculas do veiculo: ");
	scanf("%s", novo.matricula);

	printf("Tipo de veiculo (ex: 'Carro', 'Moto', 'Camiao'): ");)
    scanf("%s", novo.tipoVeiculo);


    
    int piso; //escolher piso
    printf("Piso pretendido (0 a %d): ", s->parque.pisos - 1);
    scanf("%d", &piso);


    
    if (piso < 0 || piso >= s->parque.pisos) { //validação piso
        printf("Piso inválido!
            ");
            return -1;
    }


	// Procurar lugar livre no piso escolhido
    int fila, lugar;
    int encontrou = 0;


    for (fila = 0; fila < s->parque.filasPorPiso && !encontrou; fila++) {
        for (lugar = 0; lugar < s->parque.lugaresPorFila && !encontrou; lugar++) {
            if (s->parque.mapa[piso][fila][lugar] == LUGAR_LIVRE) {
                encontrou = 1;
            }
        }
    }


    if (!encontrou) {
        printf("Não existem lugares disponíveis neste piso.");
            return -1;
    }


    //guardar lugar atribuido
    novo.piso = piso;
    novo.fila = fila - 1;
    novo.lugar = lugar - 1;


	//registar data e hora de entrada
    printf("Hora de entrada (HH:MM): ");
    scanf("%s", novo.horaEntrada);


	//estado inicial do estacionamento
    novo.estado = LUGAR_OCUPADO;


    //atualizar mapa
    s->parque.mapa[piso][novo.fila][novo.lugar] = LUGAR_OCUPADO;


	//adicionar a lista de estacionamentos
    s->estacionamentos[s->totalEstacionamentos++] = novo;


    printf("Entrada registada com sucesso! Ticket Nº %d
        ", novo.numEntrada);


        return novo.numEntrada;
}

int atribuirLugar(Sistema* s, int piso, int* filaOut, int* lugarOut) //Bruno
{
    //valida entradas
    if (s == NULL || filaOut == NULL || lugarOut == NULL)
		return 1;

    //verifica validade piso
    if (piso < 0 || piso >= s->parque.pisos)
        return 1; //caso o piso seja invalido return 1

	const int filas = s->parque.filasPorPiso;
    const int lugares = s->parque.LugaresPorFila;

    if (filas <= 0 || lugares <= 0) {
        fprintf(stderr, "Nao tem capacidade definida: (filas = %d, lugares = %d", filas, lugares);


        return 2;
    }

    static int ultimoFilaPorPiso[MAX_PISO] = { 0 };
    static int ultimoLugarPorPiso[MAX_PISO] = { 0 };
    int startFila = ultimoFilaPorPiso[piso] % filas;
    int startLugar = ultimoLugarPorPiso[piso] % lugares;

    for (int offsetF = 0; offsetF < filas; ++offsetF) {
        int f = (startFila + offsetF) % filas;

        // Varre de startLugar ao fim
        for (int l = startLugar; l < lugares; ++l) {
            if (s->parque.mapa[piso][f][l] == LUGAR_LIVRE) {
                *filaOut = f;
                *lugarOut = l;
                ultimoFilaPorPiso[piso] = f;
                ultimoLugarPorPiso[piso] = (l + 1) % lugares;
                return 0;
            }
        }
        // Varre do inicio até startLugar-1
        for (int l = 0; l < startLugar; ++l) {
            if (s->parque.mapa[piso][f][l] == LUGAR_LIVRE) {
                *filaOut = f;
                *lugarOut = l;
                ultimoFilaPorPiso[piso] = f;
                ultimoLugarPorPiso[piso] = (l + 1) % lugares;
                return 0;
            }
        }
    }

    fprintf(stderr, "Nenhum lugar disponivel.\n");
    return 2;
}

void mostrarTicketEntrada(int numEntrada) {

}

//----------------------------------------------------
// Saída de veículos
//----------------------------------------------------
int registarSaidaVeiculo() {

    return 0;
}

float calcularValorAPagar(int numEntrada) {

    return 0.0;
}

void mostrarTicketSaida(int numEntrada) {

}

void alterarSaida(int numEntrada) {                                          //Objetivo: Alterar a hora registada de saída de um veículo
    if (numEntrada < 0 || parqueRegisto[numEntrada].ativo == 1) {            //numEntrada → índice/registo do veículo no parque
        printf("Erro: Veiculo ainda no parque ou ID invalido!\n");
        return;
    }

    printf("Nova hora de saida (hh mm): ");
    scanf("%d %d", &parqueRegisto[numEntrada].saida.hora,
                   &parqueRegisto[numEntrada].saida.minuto);

    printf("Saida alterada com sucesso!\n");

}


}

void anularSaida(int numEntrada) {                                   
    if (numEntrada < 0) return;                                      //O veiculo ja deve ter saida registada (ativo == 0)
                                                                     //Se for inválido, a função não faz
    parqueRegisto[numEntrada].ativo = 1;
    printf("Saida anulada!\n");

}

//----------------------------------------------------
// Consulta, alteração e eliminação de registos
//----------------------------------------------------
void consultarEstacionamento(int numEntrada) {

}

void alterarEstacionamento(int numEntrada) {

}

void eliminarEstacionamento(int numEntrada) {

}

//----------------------------------------------------
// Lugares indisponíveis
//----------------------------------------------------
void marcarLugarIndisponivel() {

}

void reverterLugarIndisponivel() {

}

//----------------------------------------------------
// Mapa do piso
//----------------------------------------------------
void mostrarMapaPiso(int piso) {

}

//----------------------------------------------------
// Persistência
//----------------------------------------------------
void guardarDadosEmBinario() {

}

void gravarEstacionamentosTexto() {

}

void gravarErros() {

}

//----------------------------------------------------
// Listagens e paginação
//----------------------------------------------------
void listarEstacionamentos() {

}

void avancarPagina() {

}

void recuarPagina() {

}

void mostrarPagina(int pagina) {

}

void gravarListagemTXT() {

}

//----------------------------------------------------
// Funcionalidades adicionais obrigatórias (3 à escolha)
//----------------------------------------------------
void funcionalidadeExtra1() {

}

void funcionalidadeExtra2() {

}

void funcionalidadeExtra3() {

}

//----------------------------------------------------
// Extras opcionais (E1, E2, E3)
//----------------------------------------------------
void gerarGrafico() {

}

void gerarTabelaDinamica() {

}

void gerarCSV() {

}
