#ifndef FUNCOES_H
#define FUNCOES_H

#include "estruturas.h"

//-----------------------------------------------
//  Inicialização e carregamento de dados
//-----------------------------------------------
void configurarParque(PARQUE* p);
ResultadoLeitura primeiraLeitura(SISTEMA* s);
ResultadoLeitura leituraConstante(SISTEMA* s);
void inicializarSistema(SISTEMA* s);

//-----------------------------------------------
//  Ficheiros (helpers)
//-----------------------------------------------
FILE* abrirArquivo(const char* caminho, const char* modo);

// Binário
int guardarBinario(SISTEMA* s);
int carregarBinario(SISTEMA* s);

// Texto
void carregarTarifasDeFicheiro(SISTEMA* s);
void carregarEstacionamentosDeFicheiro(SISTEMA* s);

//-----------------------------------------------
//  Menus (se precisarem do sistema, passem ponteiro)
//-----------------------------------------------
void mostrarMenuPrincipal(SISTEMA* s);
void mostrarOcupacaoPisos(SISTEMA* s);

//-----------------------------------------------
//  Gestão da entrada de veículos
//-----------------------------------------------
int registarEntradaVeiculo(SISTEMA* s);
int atribuirLugar(SISTEMA* s, int piso, int* filaOut, int* lugarOut);
void mostrarTicketEntrada(int numEntrada);

//-----------------------------------------------
//  Gestão de saídas de veículos
//-----------------------------------------------
int registarSaidaVeiculo(void);
float calcularValorAPagar(int numEntrada);
void mostrarTicketSaida(int numEntrada);
void alterarSaida(int numEntrada);
void anularSaida(int numEntrada);

//-----------------------------------------------
//  Consulta, alteração e eliminação de registos
//-----------------------------------------------
void consultarEstacionamento(int numEntrada);
void alterarEstacionamento(int numEntrada);
void eliminarEstacionamento(int numEntrada);

//-----------------------------------------------
//  Gestão de lugares indisponíveis
//-----------------------------------------------
void marcarLugarIndisponivel(void);
void reverterLugarIndisponivel(void);

//-----------------------------------------------
//  Mapa do piso
//-----------------------------------------------
void mostrarMapaPiso(int piso);

//-----------------------------------------------
//  Persistência (texto/logs)
//-----------------------------------------------
void gravarEstacionamentosTexto(SISTEMA* s);
void gravarErros(SISTEMA* s);

//-----------------------------------------------
//  Listagens e paginação
//-----------------------------------------------
void listarEstacionamentos(void);
void avancarPagina(void);
void recuarPagina(void);
void mostrarPagina(int pagina);
void gravarListagemTXT(void);

//-----------------------------------------------
//  Funcionalidades adicionais
//-----------------------------------------------
void funcionalidadeExtra1(void);
void funcionalidadeExtra2(void);
void funcionalidadeExtra3(void);

//-----------------------------------------------
//  Extras opcionais
//-----------------------------------------------
void gerarGrafico(void);
void gerarTabelaDinamica(void);
void gerarCSV(void);

#endif /* FUNCOES_H */





