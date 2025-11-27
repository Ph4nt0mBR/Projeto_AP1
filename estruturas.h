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

//=======================================================
//		PROTÓTIPOS
//=======================================================

int primeiraLeitura();

int leituraConstante();

//=======================================================
//		STRUCTS
//=======================================================

typedef struct posicao_estacionamento {
	int andar; // de 0 a MAX_ANDAR-1
	char fila; // de A = 0 a z = MAX_FILA-1
	int lugar; // de 0 a MAX_LUGARES-1
}VAGAS;

typedef struct condicao_estacionamento {
	int disponivel = 1; // 1 para disponivel, 0 para ocupado
	struct VAGAS localizacao;
} ESTACIONAMENTO;

typedef struct dados_carros {
	int hora_entrada;
	int hora_saida;
	int dias;

}REGISTRO;

typedef struct tarifas {
	float valor_normal;
	float valor_noturno;
	float valor_dia;
};\ 

typedef struct {
    int ocupado;  //1 é true, ou seja 0 = livre, 1 = ocupado
    char indisponivel;
} Lugar;

typedef struct{
    char tipoTarifa; //Meter H para hora, D para Dia
    char codigo[10];  //CT1, CT2, CT3, CT4. bastava codigo[4]. meti 10 caso se adicionem mais planos de tarifa
    int horaInf; //das 00:00 as 22:00
    int horaSup; //das 22:00 as 00:00
    float valor;
} Tarifario;

typedef struct {
    Tarifario tarifas[10];          //tarifas carregadas do ficheiro. mesma coisa de antes. nao era necessario 10
    int totalTarifas;

    Estacionamento estacionamentos[6500]; //maximo de estacionamentos e 6500. deixa se um pouco a mais caso se construam mais pisos -- Não. O limite máximo q os professores defiriram é 6500
    int totalEstacionamentos;

    int ultimoNumeroEntrada;

    Parque parque;

} Sistema; //tenho de rever esta struct


#endif /* estruturas.h */
