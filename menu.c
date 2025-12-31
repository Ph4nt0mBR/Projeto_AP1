#include "menu.h"
#include "funcoes.h"
#include <stdio.h>

void menuPrincipal(Sistema *s) {
    int op;
    while (1) {
        mostrarDisponiveisPorPiso(s);
        printf("\n===== MENU =====\n");
        printf("1) Registar Entrada\n");
        printf("2) Registar Saida\n");
        printf("3) Consultar/Alterar/Eliminar Registo\n");
        printf("4) Mostrar Mapa de Piso\n");
        printf("5) Marcar Lugar Indisponivel\n");
        printf("6) Reverter Indisponivel\n");
        printf("7) Listar Estacionamentos (paginado)\n");
        printf("8) Guardar (dados.bin)\n");
        printf("0) Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &op) != 1) { limparBuffer(); continue; }
        limparBuffer();

        if (op == 1) registarEntrada(s);
        else if (op == 2) registarSaida(s);
        else if (op == 3) consultarAlterarEliminar(s);
        else if (op == 4) {
            int piso = 1;
            printf("Piso: ");
            scanf("%d", &piso);
            limparBuffer();
            imprimirMapaPiso(s, piso);
        }
        else if (op == 5) marcarIndisponivel(s);
        else if (op == 6) reverterIndisponivel(s);
        else if (op == 7) listarEstacionamentosPaginado(s);
        else if (op == 8) {
            if (guardarBinario(s, "dados.bin")) printf("Guardado com sucesso.\n");
            else printf("Erro ao guardar.\n");
        }
        else if (op == 0) {
            // Guarda obrigatoriamente ao sair (enunciado)
            guardarBinario(s, "dados.bin");
            printf("Adeus!\n");
            break;
        }
    }
}
