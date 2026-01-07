#ifndef FUNCOES_H
#define FUNCOES_H

#include "estruturas.h"

// Inicializacao e Persistencia
void inicializarSistema(SISTEMA* s);
int guardarBinario(SISTEMA* s);
int carregarBinario(SISTEMA* s);
int leituraConstante(SISTEMA* s);
int primeiraLeitura(SISTEMA* s);

// Configuração do parque
void configurarParque(PARQUE* p);

// Carregamento de arquivos .txt
void carregarTarifasDeFicheiro(SISTEMA* s);
void carregarEstacionamentosDeFicheiro(SISTEMA* s);

// Entrada e saída de veículos
int atribuirLugar(SISTEMA* s, int piso, int* filaOut, int* lugarOut);
int registarEntradaVeiculo(SISTEMA* s);

// Funções auxiliares
void trimTexto(char* s);
FILE* abrirArquivo(const char* caminho, const char* modo);
int colunaChar_Indice(char letraParaNum);
char colunaIndice_Char(const PARQUE* p, int intParaChar);
int coordenadaValida(const PARQUE* p, int andar, char filaChar, int lugar);

// Consultas e alterações
void consultarEstacionamento(SISTEMA* s, int numEntrada);
void alterarEstacionamento(SISTEMA* s, int numEntrada);


// Funções vazias (a implementar)
int registarSaidaVeiculo(SISTEMA* s, int numEntrada);
void gerirLugaresIndisponiveis(SISTEMA* s);
void mostrarMapaPiso(SISTEMA* s);
void listarEstacionamentos(SISTEMA* s);
void funcionalidadeExtra1(SISTEMA* s);
void gerarGrafico(SISTEMA* s);

#endif