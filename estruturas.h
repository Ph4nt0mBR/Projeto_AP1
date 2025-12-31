#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>

#define MAX_PISOS 5
#define MAX_FILAS 26
#define MAX_LUGARES 50

#define MAX_TARIFAS 10
#define MAX_COD_TARIFA 8

typedef enum {
    LUGAR_LIVRE = 0,
    LUGAR_OCUPADO = 1,
    LUGAR_INDISPONIVEL = 2
} EstadoLugar;

typedef struct {
    EstadoLugar estado;   // livre, ocupado, indisponível
    char motivo;          // i/o/r/m se indisponível, '-' caso contrário
    int numEntrada;       // se ocupado, nº do registo de estacionamento; senão -1
} Lugar;

typedef struct {
    int pisos;            // 1..5
    int filas;            // 1..26  (A..)
    int lugaresPorFila;   // 1..50  (01..)
    Lugar *lugares;       // array linear: [pisos*filas*lugaresPorFila]
} Parque;

typedef struct {
    char tp;                       // 'H' ou 'D'
    char cod[MAX_COD_TARIFA];      // "CT1", "CT2", "CT3", "CT4"
    int hInf, mInf;                // para H (CT1/CT2)
    int hSup, mSup;                // para H (CT1/CT2)
    double valor;                  // €/hora (H) ou €/dia (D)
} Tarifa;

typedef struct {
    int numE;
    char matricula[16];

    int anoE, mesE, diaE, horaE, minE;
    char lugarCod[8]; // ex: "3C05" (ou "1A21")

    int temSaida;     // 0/1
    int anoS, mesS, diaS, horaS, minS;
    double valorPago;
    char obs[64];
} Estacionamento;

typedef struct {
    Parque parque;

    Tarifa tarifas[MAX_TARIFAS];
    int nTarifas;

    Estacionamento *ests;
    int nEsts;
    int capEsts;

    int ultimoNumEntrada; // para gerar numE sequencial
} Sistema;

#endif
