#define	_CRT_SECURE_NO_WARNINGS

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
        printf("4 - Alterar estacionamento\n");
        printf("5 - Listar estacionamentos\n");
		printf("6 - Mostrar mapa de piso\n");
		printf("7 - Listar todos os estacionamentos\n");
        printf("8 - Gravar dados\n");
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

        case 2: {
            int id = 0;
            printf("Numero de entrada (Ticket):");
            if (scanf("%d", &id) != 1) { while (getchar() != '\n'); id = 0; } // validacao -- Aplicado em funções q o usuario escreve input 
            while (getchar() != '\n');                         // limpa o enter (buffer)
            registarSaidaVeiculo(s, id);
            break;
        }

        case 3: {
            int id = 0;
            printf("Numero de entrada (ticket): ");
            if (scanf("%d", &id) != 1) { while (getchar() != '\n'); id = 0; }
            while (getchar() != '\n');
            consultarEstacionamento(s, id);
            break;
        }

        case 4: {
            int id = 0;
            printf("Numero de entrada (ticket) a alterar: ");
            if (scanf("%d", &id) != 1) { while (getchar() != '\n'); id = 0; }
            while (getchar() != '\n');
            alterarEstacionamento(s, id);
            break;
        }

        case 5: {
            int id = 0;
            printf("Numero de entrada (ticket) a eliminar: ");
            if (scanf("%d", &id) != 1) { while (getchar() != '\n'); id = 0; }
            while (getchar() != '\n');
            eliminarEstacionamento(s, id);
            break;
        }

        case 6: {
            int piso = 0;
            printf("Piso a mostrar (1 a %d): ", s->parque.pisos);
            if (scanf("%d", &piso) != 1) { while (getchar() != '\n'); piso = 0; }
            while (getchar() != '\n');
            mostrarMapaPiso(s, piso - 1);  // 0-based
            break;
        }

        case 7: 
            listarEstacionamentos(s);
			break;
       
        case 8:
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