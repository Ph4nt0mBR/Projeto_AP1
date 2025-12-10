                               //PROPRIEDADE DO LULU QUALQUER ALTERAÇÃO POR FAVOR ENTRAR EM CONTACTU!!!

/*
  main.c
  Ponto de entrada principal — orquestra inicialização, menu e persistência. __Lu
  Centraliza setlocale para Português (com várias tentativas) e regista handler de sinais.
*/

#ifdef _WIN32
  #include <windows.h> /* Incluído apenas no Windows para chamadas específicas se necessário. __Lu */
#endif

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <signal.h>

#include "estruturas.h"
#include "funcoes.h"
#include "menu.h"

/* Flag para indicar que foi pedido encerramento (ex.: Ctrl+C).  */
static volatile sig_atomic_t sinal_saida = 0;

/* Tenta definir locale para Português com vários nomes conhecidos (PT-PT, PT-BR, fallback).  */
static void definir_locale_portugues(void) {
    const char *opcoes[] = {
        "pt_PT.UTF-8",           /* Linux/macOS com UTF-8 — preferível para PT-PT.  */
        "pt_PT",                 /* alternativa PT-PT. __Lu */
        "pt_BR.UTF-8",           /* fallback para PT-BR se PT-PT não existir.  */
        "pt_BR",                 /* alternativa PT-BR. __Lu */
        "Portuguese_Portugal.1252", /* Windows nome possível para Português de Portugal.  */
        "Portuguese_Brazil.1252",   /* Windows nome possível para Português do Brasil.  */
        "Portuguese",            /* alternativa genérica. __Lu */
        ""                       /* herdar do ambiente (último recurso). __Lu */
    };
    for (size_t i = 0; i < sizeof(opcoes) / sizeof(opcoes[0]); ++i) {
        if (setlocale(LC_ALL, opcoes[i]) != NULL) {
            /* Locale definido com sucesso; termina tentativa de fallback.  */
            return;
        }
    }
    /* Se nenhum locale foi aceite, continua sem falhar; mensagens podem perder acentuação. __Lu */
}

/* Handler de SIGINT (Ctrl+C). Marca a variável para o fluxo principal tratar o encerramento.  */
static void handler_sigint(int signo) {
    (void)signo;
    sinal_saida = 1;
}

/* Função que tenta persistir os dados relevantes antes do encerramento. */
static void salvar_e_encerrar(void) {
    /* Tenta gravar os registos de estacionamento em texto.  */
    gravarEstacionamentosTexto();

    /* Tenta gravar uma cópia em binário, se implementado.  */
    guardarDadosEmBinario();

    /* Outros mecanismos de persistência (erros, logs) podem ser chamados aqui.  */
}

/* Função principal — inicializa, executa o menu e garante persistência ao sair.  */
int main(int argc, char *argv[]) {
    (void)argc; (void)argv; /* Arguments currently not used.  */

    /* Definir locale para português (Portugal preferencial). */
    definir_locale_portugues();

    /* Registar handler para Ctrl+C para permitir encerramento controlado.  */
    if (signal(SIGINT, handler_sigint) == SIG_ERR) {
        fprintf(stderr, "Aviso: não foi possível registar handler de sinais.\n"); /*  */
    }

    /* Inicializar sistema: carregar ficheiros (tarifas, estacionamentos), estruturas, etc.  */
    /* Esta função deverá ser implementada em func.c e declarada em funcoes.h.  */
    inicializarSistema();

    /* Delegar a interação para o módulo de menu principal que contém o loop de interação.  */
    /* Espera-se que mostrarMenuPrincipal() bloqueie até o utilizador escolher sair.  */
    mostrarMenuPrincipal();

    /* Se o menu retornou, gravar dados antes de encerrar.  */
    salvar_e_encerrar();

    /* Se foi recebido SIGINT durante a execução, notificar o utilizador de encerramento seguro.  */
    if (sinal_saida) {
        printf("\nInterrupção recebida. Dados gravados e programa encerrado.\n"); /*  */
    }

    return 0;
}
/* Fim de main.c __Lu */
