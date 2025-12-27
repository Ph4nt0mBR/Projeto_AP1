#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "funcoes.h"  // Incluir para ter acesso às funções do sistema

static void limparEcra(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void pausa(void) {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void mostrarMenuPrincipal(SISTEMA* s) {
    int opcao = 0;

    if (!s) {
        fprintf(stderr, "Erro: sistema nulo no menu.\n");
        return;
    }

    do {
        limparEcra();

        printf("=================================\n");
        printf("        SISTEMA DE PARQUE         \n");
        printf("=================================\n");
        printf("Pisos: %d | Filas: %d | Lugares: %d\n",
            s->parque.pisos,
            s->parque.filasPorPiso,
            s->parque.lugaresPorFila);
        printf("---------------------------------\n");

        printf("1 - Entrada de veiculo\n");
        printf("2 - Saida de veiculo\n");
        printf("3 - Consultar estacionamento\n");
        printf("4 - Mostrar mapa de um piso\n");
        printf("5 - Listar estacionamentos\n");
        printf("6 - Gravar dados\n");
        printf("9 - Sair\n");
        printf("---------------------------------\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // limpa buffer
            opcao = 0;
        }
        while (getchar() != '\n'); // remove ENTER

        switch (opcao) {
        case 1:
            // Chama a função de entrada de veículo
            registarEntradaVeiculo(s);
            break;

        case 2:
            printf("Saida de veiculo (a implementar)\n");
            break;

        case 3: {
            int id = 0;                                       //n ticket
            printf("Numero de entrada (ticket): ");
            if (scanf("%d", &id) != 1) { while (getchar() != '\n'); id = 0; } // validacao
            while (getchar() != '\n');                         // limpa o enter (buffer)
            consultarEstacionamento(s, id);
            break;
        }

        case 4:
            printf("Mapa do piso (a implementar)\n");
            break;

        case 5:
            printf("Listagem de estacionamentos (a implementar)\n");
            break;

        case 6:
            printf("A gravar dados...\n");
            guardarBinario(s);
            break;

        case 9:
            printf("A sair do programa...\n");
            break;

        default:
            printf("Opcao invalida!\n");
        }

        if (opcao != 9)
            pausa();

    } while (opcao != 9);
}
