   //Criei o código pra funcionar como o start do programa, qualquer dúvida entrar em contato (main.c é a minha parte do projeto i think) __Luca

/*
  main.c
  Ponto de entrada principal — orquestra inicialização, menu e persistência.

  Nota: Este ficheiro assume que os protótipos em 'funcoes.h' e 'menu.h' 
  estão ajustados para receberem 'SISTEMA* s' quando necessário.
*/

#ifdef _WIN32
  #include <windows.h> 
#endif

#include <stdio.h>          // A maioria desses includes tão nas .h. N precisa repetir aqui [Samuel]
#include <stdlib.h>
#include <locale.h>
#include <signal.h>

#include "estruturas.h"
#include "funcoes.h"
#include "menu.h"

/* Flag para indicar que foi pedido encerramento (ex.: Ctrl+C).  */
static volatile sig_atomic_t sinal_saida = 0;

/* Tenta definir locale para Português com vários nomes conhecidos.  */
static void definir_locale_portugues(void) {        //Eu acho q isso daqui é meio exagerado. Acho q pode só colocar o setlocale dentro do main [Samuel]
    const char *opcoes[] = {
        "pt_PT.UTF-8",
        "pt_PT",
        "pt_BR.UTF-8",
        "pt_BR",
        "Portuguese_Portugal.1252",
        "Portuguese_Brazil.1252",
        "Portuguese",
        ""
    };
    for (size_t i = 0; i < sizeof(opcoes) / sizeof(opcoes[0]); ++i) {
        if (setlocale(LC_ALL, opcoes[i]) != NULL) {
            return;
        }
    }
}

/* Handler de SIGINT (Ctrl+C). Marca a variável para o fluxo principal tratar o encerramento.  */
static void handler_sigint(int signo) {
    (void)signo;
    sinal_saida = 1;
}

/* Função que tenta persistir os dados relevantes antes do encerramento (Requisito 9). */
/* Recebe agora o ponteiro para a estrutura principal SISTEMA. */
static void salvar_e_encerrar(SISTEMA* s) {
	printf("\nA gravar dados e a encerrar a aplicação...\n");       // Esse tipo de função tem q estar no func.c [Samuel]
    
    // 1. Gravar dados principais em BINÁRIO (Obrigatório, Requisito 9)
    // Assume que esta função guarda a estrutura SISTEMA completa ou os arrays dinâmicos.
    guardarDadosEmBinario(s); 

    // 2. Gravar os registos de estacionamento em TEXTO (Opcional, mas útil/requerido para logs)
    // Assume que é uma função de listagem que também grava.
    gravarEstacionamentosTexto(s); 
    
    // 3. Gravar o ficheiro de logs de erro (Requisito 9 e 2.c)
    // Assume que esta função guarda o log de erros acumulado (se existir).
    gravarErros(s); 

    printf("Persistência de dados concluída.\n");
}

/* Função principal — inicializa, executa o menu e garante persistência ao sair.  */
int main(int argc, char *argv[]) {
    (void)argc; (void)argv; /* Argumentos atualmente não usados.  */

    /* Declarar a estrutura central de dados. */
    SISTEMA sistema; 
    
    /* Definir locale para português. */
    definir_locale_portugues();

    /* Registar handler para Ctrl+C para permitir encerramento controlado.  */
    if (signal(SIGINT, handler_sigint) == SIG_ERR) {
        fprintf(stderr, "Aviso: não foi possível registar handler de sinais.\n"); 
    }

    /* Inicializar sistema (Passando a referência).
      Esta função deve:
      1. Tentar carregar dados binários da última sessão (Uso Normal - Requisito 2).
      2. Se falhar, executar a 'Instalação' (pedir conf. do parque, ler .txt) (Requisito 1).
    */
    inicializarSistema(&sistema);

    /* Delegar a interação para o módulo de menu principal (Passando a referência).
      O menu precisa do sistema para mostrar a ocupação (Requisito 3).
    */
    mostrarMenuPrincipal(&sistema);

    /* Se o menu retornou, garantir que os dados são salvos antes de encerrar.  */
    salvar_e_encerrar(&sistema);

    /* Se foi recebido SIGINT durante a execução, notificar o utilizador de encerramento seguro.  */
    if (sinal_saida) {
        printf("Interrupção recebida. Dados gravados e programa encerrado.\n"); 
    }

	//=======================================================
	//   REFERENTE AO enum ResultadoLeitura
	//=======================================================
    ResultadoLeitura r = leituraConstante(&sistema);
    switch (r) {
    case LER_OK:
        printf("Leitura concluída.\n");
        break;
    case LER_FALHA_ABRIR:
        fprintf(stderr, "Erro: não foi possível abrir um ficheiro requerido.\n");
        return 1;
    case LER_FICHEIRO_VAZIO:
        fprintf(stderr, "Aviso: ficheiro sem dados úteis.\n");
        break;
    case LER_ERRO_FORMATO:
        fprintf(stderr, "Erro: dados de texto com formato inválido.\n");
        return 1;
    case LER_ERRO_IO:
        fprintf(stderr, "Erro: falha de I/O ao ler/gravar.\n");
        return 1;
    case LER_DIMENSOES_INVALIDAS:
        fprintf(stderr, "Erro: dimensões do parque inválidas.\n");
        return 1;
    case LER_CAPACIDADE_EXCEDIDA:
        fprintf(stderr, "Erro: capacidade máxima excedida.\n");
        return 1;
    }


    return 0;
}

// Se cada um já tiver atualizado seus arquivos com as instrucoes q mandei no grupo, 
// este arquivo main.c ja deve estar a funcionar perfeitamente para o estacionamento qualquer duvida so me chamar __Lu

