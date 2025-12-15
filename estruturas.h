/*
	header.h
	Criado por: Samuel
	Data 25/11

	Nota: Tentar manter esse projeto o mais organizado possivel.
	Manter qualquer arquivo extra em .c ou .h, e lembrar de incluir SOMENTE onde vai usar.
	Se não está sendo usado em nenhum lugar, remover o arquivo.

*/



#define	_CRT_SECURE_NO_WARNINGS

#ifndef estruturas_h
#define estruturas_h

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>


//==========================================
//		DEFINIÇÕES
//==========================================

#define MAX_PISO 5
#define MAX_FILA 26 // De A a Z
#define MAX_LUGARES 50
#define MAX_TARIFAS 10 // Limite máximo de planos de tarifas com margem [Samuel]
#define MAX_ESTACIONAMENTOS 50000

//==========================================
//		ENUMERAÇÕES (Objetivo de padronizar estados e codigos)
//==========================================

typedef enum estado_lugar { // Flag para informar se o lugar está ocupado, livre ou indisponível
	LUGAR_LIVRE = 0,
	LUGAR_OCUPADO = 1,
	LUGAR_INDISPONIVEL = 2
} EstadoLugar;

typedef enum leitura {
	LER_OK = 0,
	LER_FALHA_ABRIR,
	LER_FICHEIRO_VAZIO,
	LER_ERRO_FORMATO,
	LER_ERRO_IO,
	LER_DIMENSOES_INVALIDAS,
	LER_CAPACIDADE_EXCEDIDA
} ResultadoLeitura;


//==========================================
//		STRUCTS
//==========================================

typedef struct parque {		// Config da matriz do estacionamento
	int pisos;			// Numero de pisos (max 5)
	int filasPorPiso;	// Numero de filas por piso (max 26)
	int lugaresPorFila;	// Numero de lugares por fila (max 50)

	EstadoLugar mapa[MAX_PISO][MAX_FILA][MAX_LUGARES]; // Matriz que representa o estacionamento -- O enum serve mais para facilitar a leitura do estado do lugar antes do carro entrar ou sair
} PARQUE;

typedef struct parametro_estacionamento {		//Informações dos veículos estacionados
	int id;
	char matricula[11];		/* Padrão europeu */
	char dataEntrada[11];	/* dd/mm/aaaa */
	char horaEntrada[6];	/* hh:mm */
	char dataSaida[11];		/* dd/mm/aaaa */
	char horaSaida[6];		/* hh:mm */

	int andar; // de 0 a MAX_PISO-1
	char fila; // de A = 0 a z = MAX_FILA-1
	int lugar; // de 0 a MAX_LUGARES-1

	EstadoLugar estado;		//LUGAR_LIVRE ou LUGAR_OCUPADO
} VAGAS;

typedef struct tarifario{
	float valor;		//valor da tarifa
	char etiqueta[10];	//etiqueta da tarifa (ex: normal, especial, etc)
} TARIFARIO;

typedef struct sistema {
	PARQUE parque;

	TARIFARIO tarifas[MAX_TARIFAS];
	int totalTarifas;
	VAGAS estacionamentos[MAX_ESTACIONAMENTOS];
	int totalEstacionamentos;

	int ultimoNumEntrada;
} SISTEMA; //tenho de rever esta struct -- Já ta revisada [Samuel]


//=======================================================
//		PROTÓTIPOS
//=======================================================


void configurarParque(PARQUE* p);

//--------------------------------
// Funções que lidam com ficheiros
//--------------------------------
FILE* abrirArquivo(const char* caminho, const char* modo);
ResultadoLeitura primeiraLeitura(SISTEMA* s);
ResultadoLeitura leituraConstante(SISTEMA* s);
// Binario
void guardarBinario(SISTEMA* s);
int carregarBinario(SISTEMA* s);
// Texto
void carregarEstacionamentosDeFicheiro(SISTEMA* s);
void carregarTarifasDeFicheiro(SISTEMA* s);


void inicializarSistema(SISTEMA* s);





#endif /* estruturas_h */
