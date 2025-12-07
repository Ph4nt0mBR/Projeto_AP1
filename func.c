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



//=======================================================
// Utilitários
//=======================================================

void trimTexto(char* s) { //Remove espaços em branco no final de uma string
	
	if (!s) return;
    stze_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r' || s[n-1] == ' ' || s[n-1] == '\t')) {
        s[--n] = '\0';
	}
}

FILE* abrirArquivoTexto(const char* caminho, const char* modo) {
	
    FILE* f = fopen(caminho, modo);
    if (!f) {
		printf("Erro ao abrir o arquivo: %s\n", caminho);
		return NULL;
    }
	return f;
}

int coordenadaValida(const Parque* p, int andar, char, filaChar, int lugar) {
    
    if (!p) return 0;
    if (andar < 0 || andar >= p->pisos) return 0;


}


//=======================================================
// Leituras
//=======================================================

int primeiraLeitura() // Aqui a gente vai ler os arquivos 1x, por isso q os .txt tão aqui. dps dessa leitura, o define vai ficar em estruturas.h [Samuel]
{
	FILE* f = NULL;



}


//----------------------------------------------------
// Inicialização e carregamento de dados
//----------------------------------------------------
void configurarParque(Parque* p) {

    if (p == NULL) {
        printf(stderr, "Erro, ponteiro nulo.\n");
        return;
    }

    while (1) {
        printf("Numero de pisos (1-%d): ", MAX_PISO);
        if scanf("%d", &p->pisos) == 1 && p->pisos >= 1 && p->pisos <= MAX_PISO) { //verifica se e valido
            break; //break para sair do while
        }
        fprintf(stderr, "Valor invalido, tente novamente.\n");
    }

    while (1) {
        printf("Numero de filas por piso (1-%d): ", MAX_FILA);
        if scanf("%d", &p->filasPorPiso) == 1 && p->filasPorPiso >= 1 && p->filasPorPiso <= MAX_FILA) { //verifica se e valido
            break; //break para sair do while
        }
        fprintf(stderr, "Valor invalido, tente novamente.\n");
    }

    while (1) {
        printf("Numero de lugares por fila (1-%d): ", MAX_LUGARES);
        if scanf("%d", &p->lugaresPorPiso) == 1 && p->lugaresPorPiso >= 1 && p->lugaresPorPiso <= MAX_LUGARES) { //verifica se e valido
            break; //break para sair do while
        }
        fprintf(stderr, "Valor invalido, tente novamente.\n");
    }


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

void carregarTarifasDeFicheiro(SISTEMA* s)
{
	//verifica se o sistema e valido
    if (s == NULL) {
        fprintf(stderr, "Erro: sistema nulo.\n\n");
        return; //se nao for, nao carrega

    }

	FILE* f = abrirArquivoTexto(TARIFAS_PATH, "r"); //abre o arquivo de tarifas em read only
    if (!f) { 
        return; //se falhar a abrir, sai
    }

    s->totalTarifas = 0;

    char linha[256];
	while (fgets(linha, sizeof(linha), f) != NULL) { //le todas as linhas do arquivo
        //ignora linhas em branco
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

void carregarEstacionamentosDeFicheiro(Sistema* s) {
    FILE* f = fopen("Estacionamentos.txt", "r");
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
        s->estacionamentos[s->totalEstacionamentos++] = e;
    }
    fclose(f);
    printf("Estacionamentos carregados com sucesso: %d\n", s->totalEstacionamentos);

}


// Removi as funções de carregar tarifas e estacionamento. Vou converter em uma função que inicia a primeira leitura dos arquivos .atxt e depois dos arquivos em .bin [Samuel]

void inicializarSistema(SISTEMA* s) {
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
    for (a = 0; a < MAX_ANDAR; a++) {
        for (f = 0; f < MAX_FILA; f++) {
            for (l = 0; l < MAX_LUGARES; l++) {
                s->parque.mapa[a][f][l] = LUGAR_LIVRE;
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
int registarEntradaVeiculo(Sistema* s) {
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
        printf("Não existem lugares disponíveis neste piso.
            ");
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

int atribuirLugar(Sistema* s, int piso, int* filaOut, int* lugarOut)
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
