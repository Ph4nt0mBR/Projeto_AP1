/*
	header.h
	Criado por: Samuel
	Data 25/11

	Nota: Tentar manter esse projeto o mais organizado possivel.
	Manter qualquer arquivo extra em .c ou .h, e lembrar de incluir SOMENTE onde vai usar.
	Se não está sendo usado em nenhum lugar, remover o arquivo.

*/

#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#define	_CRT_SECURE_NO_WARNINGS

// Caminhos padrão. Não vão mudar durante a execução do programa.
#define TARIFAS_PATH			"tarifas.txt"
#define ESTACIONAMENTOS_PATH	"estacionamentos.txt"
#define BIN_PATH				"dados.bin"

#include <stdio.h>

//==========================================
//		DEFINIÇÕES
//==========================================
#define MAX_PISO 5
#define MAX_FILA 26 // De A a Z
#define MAX_LUGARES 50
#define MAX_TARIFAS 10 // Limite máximo de planos de tarifas com margem [Samuel]

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

	EstadoLugar estado;		//Estado atual da vaga

	int temSaida; // 0 = ainda no parque, 1 = saída registada
	double valorPago; // total calculado na saída
	char lugarCod[8]; // Ex: 1A05 (piso 1, fila A, lugar 05)
} VAGA;

typedef struct tarifario {
	float valor;		//valor da tarifa
	char etiqueta[10];	//etiqueta da tarifa (ex: normal, especial, etc)
} TARIFARIO;

typedef struct sistema {
	PARQUE parque;
	TARIFARIO tarifas[MAX_TARIFAS];
	int totalTarifas;

	int totalEstacionamentos;
	VAGAS* estacionamentos; // <- mudar para ponteiro dinâmico

	int ultimoNumEntrada;
} SISTEMA; //tenho de rever esta struct -- Já ta revisada [Samuel]

#endif /* estruturas_h */
