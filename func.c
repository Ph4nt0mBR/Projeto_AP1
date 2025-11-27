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
// Leituras
//=======================================================

int primeiraLeitura() // Aqui a gente vai ler os aruivos 1x, por isso q os .txt tão aqui. dps dessa leitura, o define vai ficar em estruturas.h [Samuel]
{
	FILE* f = NULL;



}


//----------------------------------------------------
// Inicialização e carregamento de dados
//----------------------------------------------------
void configurarParque() {

}

void carregarTarifasDeFicheiro() {

}

void carregarEstacionamentosDeFicheiro() {

}

void inicializarSistema() {

}

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
