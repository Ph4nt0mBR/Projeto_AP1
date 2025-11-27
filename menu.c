#include "menu.h"

//estas funções ainda não existem - crialas depois

//Tirei as funções daqui e coloquei em menu.h
//Vamo manter aqui só o q ta sendo usado [Samuel]

//Função para mostrar os lugares disponiveis.

void mostrarLugaresDisponiveis() {

printf("Lugares disponiveis:\n");
printf("Piso 1: %d\n", 23);
printf("Piso 2: %d\n", 41);
printf("Piso 3: %d\n", 18);
printf("Piso 4: %d\n", 50);
printf("Piso 5: %d\n\n", 27);

}
int main() {
    setlocale(LC_ALL, "portuguese");
   int opcao;

    do {
        system("clear"); // no Windows usar "cls"

        //Mostar sempre lugares disponiveis
        mostrarLugaresDisponiveis();

        printf("=== MENU PRINCIPAL ===\n");
        printf("1 - Entrada de veiculo\n");
        printf("2 - Consultar ocupação\n");
        printf("3 - Saida de veiculo + cobrança\n");
        printf("4 - Gestao de lugares indisponiveis\n");
        printf("5 - Mapa de ocupão por piso\n");
        printf("6 - Listagens com paginação\n");
        printf("7 - Extras (Graficos / CSV / tavelas)\n");
        printf("8 - Gravar dados\n");
        printf("9 - Sair\n");
        printf("======================\n");
        printf("Opção: ");

        scanf("%d", &opcao);


        switch(opcao) {

        case 1: entradaVeiculo(); break;
        case 2: consultarOcupacao(); break;
        case 3: saidaVeiculo(); break;
        case 4: gerirLugares(); break;
        case 5: mostrarMapaPiso(); break;
        case 6: listarComPaginacao(); break;
        case 7: extras(); break;
        case 8: gravarDados(); break;

        case 9:
            printf("A sair...\n");
            break;
        default:
            printf("Opção invalida! Tente novamente.\n");
        }
        printf("Pressione ENTER para continuar...");
        getchar(); getchar();
   }    while(opcao != 9);

   return 0;
}
