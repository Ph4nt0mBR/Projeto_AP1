/*
	menu.h
	Criado por: Samuel
	Data 27/11

	Nota: Usar isso daqui pra manter o menu mais organizado.
	Puxar aquele monte de declarações de funções do menu.c pra cá. [Samuel]
*/

#define _CRT_SECURE_NO_WARNINGS

#ifndef menu_h
#define menu_h
#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>


//=======================================
// PROTÓTIPOS
//=======================================

void mostrarMenuPrincipal(SISTEMA* s);

void entradaVeiculo(SISTEMA* s);
void consultarOcupacao(SISTEMA* s);
void consultarEstacionamento(SISTEMA* s, int numEntrada);
void saidaVeiculo(SISTEMA* s);
void gerirLugares(SISTEMA* s);
void mostrarMapaPiso(SISTEMA* s);
void listarComPaginacao(SISTEMA* s);
void extras(SISTEMA* s);
void gravarDados(SISTEMA* s);


#endif /* menu_h */
