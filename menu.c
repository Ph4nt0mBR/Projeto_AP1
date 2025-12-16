#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define PISOS 5
#define VAGAS_POR_PISO 50
#define MAX_VEICULOS (PISOS * VAGAS_POR_PISO)

// ===== STRUCT PARA VEÍCULOS =====
typedef struct {
    char matricula[10];
    int piso;
    int vaga;
    int horaEntrada;
    int minutoEntrada;
    int ativo; // 1 = no parque, 0 = saiu
} Veiculo;

// ===== ARRAYS DO SISTEMA =====
int vagas[PISOS][VAGAS_POR_PISO]; // 0 = livre, 1 = ocupada
Veiculo veiculos[MAX_VEICULOS];
int totalVeiculos = 0;

// ===== FUNÇÕES =====
void mostrarLugaresDisponiveis(void) {
    printf("Lugares disponiveis:\n");

    for (int i = 0; i < PISOS; i++) {
        int livres = 0;
        for (int j = 0; j < VAGAS_POR_PISO; j++) {
            if (vagas[i][j] == 0)
                livres++;
        }
        printf("Piso %d: %d\n", i + 1, livres);
    }
    printf("\n");
}

void mostrarMenuPrincipal(void) {
    int opcao;

    do {
        system("clear"); // no Windows usar "cls"

        mostrarLugaresDisponiveis();

        printf("=== MENU PRINCIPAL ===\n");
        printf("1 - Entrada de veiculo\n");
        printf("2 - Consultar ocupação\n");
        printf("3 - Saida de veiculo + cobrança\n");
        printf("4 - Gestao de lugares indisponiveis\n");
        printf("5 - Mapa de ocupação por piso\n");
        printf("6 - Listagens com paginação\n");
        printf("7 - Extras (Graficos / CSV / tabelas)\n");
        printf("8 - Gravar dados\n");
        printf("9 - Sair\n");
        printf("======================\n");
        printf("Opção: ");

        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 1: /* entradaVeiculo(); */ break;
            case 2: /* consultarOcupacao(); */ break;
            case 3: /* saidaVeiculo(); */ break;
            case 4: /* gerirLugares(); */ break;
            case 5: /* mostrarMapaPiso(); */ break;
            case 6: /* listarComPaginacao(); */ break;
            case 7: /* extras(); */ break;
            case 8: /* gravarDados(); */ break;
            case 9:
                printf("A sair...\n");
                break;
            default:
                printf("Opção invalida!\n");
        }

        printf("Pressione ENTER para continuar...");
        getchar();

    } while (opcao != 9);
}

