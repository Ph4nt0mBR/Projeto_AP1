/*
	menu.h
	Criado por: Samuel
	Data 27/11

	Nota: Usar isso daqui pra manter o menu mais organizado.
	Puxar aquele monte de declarações de funções do menu.c pra cá. [Samuel]
*/


#ifndef MENU_H
#define MENU_H

#include "estruturas.h"

//=======================================
// PROTÓTIPOS
//=======================================

void mostrarMenuPrincipal(SISTEMA* s);

void entradaVeiculo(SISTEMA* s);
void consultarOcupacao(SISTEMA* s);
void saidaVeiculo(SISTEMA* s);
void gerirLugares(SISTEMA* s);
void mostrarMapaPiso(SISTEMA* s);
void listarComPaginacao(SISTEMA* s);
void extras(SISTEMA* s);
void gravarDados(SISTEMA* s);


#endif /* menu_h */
