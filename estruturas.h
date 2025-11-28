/*
	header.h
	Criado por: Samuel
	Data 25/11

	Nota: Tentar manter esse projeto o mais organizado possivel.
	Manter qualquer arquivo extra em .c ou .h, e lembrar de incluir SOMENTE onde vai usar.
	Se não está sendo usado em nenhum lugar, remover o arquivo.

*/



#define	_CRT_SECURE_NO_WARNINGS

#ifndef estruturas.h
#define estruturas.h

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>


//==========================================
//		DEFINIÇÕES
//==========================================

#define MAX_ANDAR 5
#define MAX_FILA 26 // De A a Z
#define MAX_LUGARES 50

#define MAX_ESTACIONAMENTOS 6500 // Limite máximo de vagas no estacionamento
#define MAX_TARIFAS 10 // Limite máximo de planos de tarifas com margem [Samuel]

//==========================================
//		ENUMERAÇÕES (Objetivo de padronizar estados e codigos)
//==========================================

typedef enum estado_lugar { // Flag para informar se o lugar está ocupado, livre ou indisponível
	LUGAR_LIVRE = 0,
	LUGAR_OCUPADO = 1,
	LUGAR_INDISPONIVEL = 2
} EstadoLugar;

typedef enum estado_registro { // Estado do registro de estacionamento
	ESTADO_ATIVO = 0,
	ESTADO_FINALIZADO = 1
} EstadoRegistro;

typedef enum tarifas_codigo {
	TARIFA_T1 = 0, /* 08:00-21:59 */
	TARIFA_T2 = 1, /* 22:00-07:59 */
	TARIFA_T3 = 2, /* Dia completo (00:00-23:59) */
	TARIFA_T4 = 3  /* Varios dias (tecnicamente falando é T3>=2)*/
} CodigoTarifa;

typedef enum tipo_tarifa { // Tipo de tarifa: por hora ou por dia (Para ficar mais facil de identificar)
	TIPO_TARIFA_HORA = 0,
 	TIPO_TARIFA_DIA = 1
} TipoTarifa;

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

typedef struct posicao_estacionamento {		//(X, Y, Z) para a localização do veículo
	int andar; // de 0 a MAX_ANDAR-1
	char fila; // de A = 0 a z = MAX_FILA-1
	int lugar; // de 0 a MAX_LUGARES-1
} VAGAS;

typedef struct condicao_estacionamento {	// Verificar se a vaga está ocupada ou livre
	int disponivel = 1; // 1 para disponivel, 0 para ocupado
	VAGAS localizacao;
} ESTACIONAMENTO;

typedef struct dados_carros {
	int hora_entrada;
	int hora_saida;
	int dias;
} REGISTRO;

typedef struct tarifas {
	float valor_T1;
	float valor_T2;
	float valor_T3;
	float valor_T4;
} TARIFA_AGRUPADA; 

typedef struct tarifario{
	TipoTarifa tipo;	//tipo de tarifa (hora ou dia)
	CodigoTarifa codigo;//codigo da tarifa (T1, T2, T3, T4)
	int horaCheg;		//Horario de entrada dos veículos
	int horaSai;		//Horario de saida dos veículos
	float valor;		//valor da tarifa
	char etiqueta[10];	//etiqueta da tarifa (ex: normal, especial, etc)
} TARIFARIO;

typedef struct lugar {
	EstadoLugar estado;		//LUGAR_LIVRE ou LUGAR_OCUPADO
} LUGAR;

typedef struct parque {		// Config da matriz do estacionamento
	int pisos;			// Numero de pisos (max 5)
	int filasPorPiso;	// Numero de filas por piso (max 26)
	int lugaresPorFila;	// Numero de lugares por fila (max 50)

	LUGAR mapa[MAX_ANDAR][MAX_FILA][MAX_LUGARES]; // Matriz que representa o estacionamento
} PARQUE;

typedef struct registro_estacionamento {
	int id;
	char matricula[10];
	char dataEntrada[11]; /* dd/mm/aaaa */
	char horaEntrada[6];  /* hh:mm */
	char dataSaida[11];   /* dd/mm/aaaa */
	char horaSaida[6];    /* hh:mm */

	int andar;
	char fila;
	int lugar;

	EstadoRegistro estado; // ESTADO_ATIVO ou ESTADO_FINALIZADO
} REGISTRO_ESTACIONAMENTO;

typedef struct sistema {
	PARQUE parque;

	TARIFA_AGRUPADA tarifasBase;
	TARIFARIO tarifas[MAX_TARIFAS];
	int totalTarifas;

	REGISTRO_ESTACIONAMENTO estacionamentos[MAX_ESTACIONAMENTOS];
	int totalEstacionamentos;

	int ultimoNumEntrada;

} SISTEMA; //tenho de rever esta struct -- Já ta revisada [Samuel]


//=======================================================
//		PROTÓTIPOS
//=======================================================

ResultadoLeitura primeiraLeitura(SISTEMA* s);
ResultadoLeitura leituraConstante(SISTEMA* s);
void inicializarSistema(SISTEMA* s);
void configurarParque(PARQUE* p);

#endif /* estruturas.h */
