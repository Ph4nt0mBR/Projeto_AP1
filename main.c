#include <stdio.h>
#include "funcoes.h"
#include "menu.h"

int main(void) {
    Sistema s;
    initSistema(&s);

    // tenta carregar estado anterior
    if (carregarBinario(&s, "dados.bin")) {
        printf("Estado carregado de dados.bin\n");
    } else {
        printf("Primeira execucao / sem dados.bin\n");
        printf("=== INSTALACAO ===\n");
        if (!configurarParque(&s)) {
            printf("Falha a configurar parque.\n");
            freeSistema(&s);
            return 1;
        }
        if (!carregarTarifasTxt(&s, "tarifas.txt")) {
            printf("Aviso: não consegui carregar tarifas.txt (confirma o ficheiro).\n");
        }
        carregarEstacionamentosTxt(&s, "estacionamentos.txt");
        printf("Instalacao concluida.\n");
    }

    menuPrincipal(&s);

    freeSistema(&s);
    return 0;
}
