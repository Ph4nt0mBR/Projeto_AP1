#ifndef FUNCOES_H
#define FUNCOES_H

#include "estruturas.h"

/* --- Parque / Sistema --- */
void initSistema(Sistema *s);
void freeSistema(Sistema *s);

int configurarParque(Sistema *s);
void mostrarDisponiveisPorPiso(const Sistema *s);

/* --- Lugares --- */
int idxLugar(const Sistema *s, int piso, int fila, int lugar);
int contarLivresNoPiso(const Sistema *s, int piso);
void imprimirMapaPiso(const Sistema *s, int piso);

/* --- Tarifas --- */
int carregarTarifasTxt(Sistema *s, const char *fname);
double obterValorTarifa(const Sistema *s, const char *cod);

/* --- Estacionamentos --- */
int carregarEstacionamentosTxt(Sistema *s, const char *fname);
void listarEstacionamentosPaginado(const Sistema *s);
void exportarListagemTxt(const Sistema *s, const char *fname);

/* --- Operações --- */
void registarEntrada(Sistema *s);
void registarSaida(Sistema *s);
void consultarAlterarEliminar(Sistema *s);

/* --- Indisponibilidades --- */
void marcarIndisponivel(Sistema *s);
void reverterIndisponivel(Sistema *s);

/* --- Persistência binária --- */
int guardarBinario(const Sistema *s, const char *fname);
int carregarBinario(Sistema *s, const char *fname);

/* --- Utilitários --- */
void limparBuffer(void);

#endif
