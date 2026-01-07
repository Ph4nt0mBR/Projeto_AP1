#ifndef MENU_H
#define MENU_H

#define	_CRT_SECURE_NO_WARNINGS

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
