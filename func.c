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

int primeiraLeitura() // Aqui a gente vai ler os aruivos 1x, por isso q os .txt tão aqui. dps dessa leitura, o define vai ficar em estruturas.h [Samuel]
{
	FILE* f = NULL;



}


//----------------------------------------------------
// Inicialização e carregamento de dados
//----------------------------------------------------
void configurarParque(Parque* p) {    //eu amanha documento isto melhor maltinha. (bruno)


    do {
        printf("Tamanho de pisos(1-5):\n");
        scanf("%d", &p->pisos);

    } while (p->pisos<1 || p->pisos > MAX_PISOS); //verifica se o numero de pisos e valido(maior q um, menor q 5
    do {
        printf("Numero de filas por piso (1-26): \n");
        scanf("%d", &p->filasPorPiso);
    } while (p->filasPorPiso < 1 || p->filasPorPiso > MAX_FILAS);

    do {
        printf("Numero de lugares por cada fila(1-50):\n");
        scanf("%d", &p->lugaresPorFila);
    } (p->lugaresPorFila < 1 || p->lugaresPorFila > MAX_LUGARES);

} //acho q thats it. qualquer cena volta se ca no troublleshoot


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
int registarEntradaVeiculo() {

    return 0;
}

int atribuirLugar(int piso) {

    return 0;
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

void alterarSaida(int numEntrada) {

}

void anularSaida(int numEntrada) {

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
