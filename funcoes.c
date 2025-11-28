#include <stdio.h>
#include "funcoes.h"
//Çuca

//----------------------------------------------------
// Inicialização e carregamento de dados
//----------------------------------------------------
void configurarParque(Parque *p) {    //eu amanha documento isto melhor maltinha. (bruno)


    do{
        printf("Tamanho de pisos(1-5):\n");
        scanf("%d", &p->pisos);

    }while (p->pisos<1 || p->pisos > MAX_PISOS); //verifica se o numero de pisos e valido(maior q um, menor q 5
        do{
            printf("Numero de filas por piso (1-26): \n");
            scanf("%d", &p->filasPorPiso);
        } while (p->filasPorPiso < 1 || p->filasPorPiso > MAX_FILAS);

        do{
            printf("Numero de lugares por cada fila(1-50):\n");
            scanf("%d", &p->lugaresPorFila);
        } (p->lugaresPorFila < 1 || p->lugaresPorFila > MAX_LUGARES);

} //acho q thats it. qualquer cena volta se ca no troublleshoot

void carregarTarifasDeFicheiro(Sistema *s) {
    FILE *f = fopen("Tarifario.txt", "r");
    if (!f) {
        printf("Erro ao abrir o ficheito Tarefario.txt.\n");
        return;
    }

    s->totalTarifas = 0;
    
    char tipo;
    char codigo[10];
    char hInf[10];
    char hSup[10];
    char valorStr[20];
    
    while (fscanf(f, " %c %s %s %s %s", &tipo, codigo, hInf, hSup, valorStr) == 5)
    {
        
        Tarifario t;
        t.tipoTarifa = tipo;
        strcpy(t.codigo, codigo);
        
        int hora, min;
        
        sscanf(hInf, "%d:%d", &hora, &min);
        t.horaInf = hora * 100 + min;
        
        sscanf(hSup, "%d:%d", &hora, &min);
        t.horaInf = hora * 100 + min;
        valorStr[strcspn(valorStr,"€")] = "\0";
        
        t.valor = atof(valorStr);
        
        s->tarifas[s->totalTarifas++] = t;
    }
    
    fclose(f);
    
    printf("Tarifas carregadas com sucesso: %d\n", s->totalTarifas);
        
}

void carregarEstacionamentosDeFicheiro() {

}

void inicializarSistema() {

}

//----------------------------------------------------
// Menus
//----------------------------------------------------
void mostrarMenuPrincipal() {

}

void mostrarOcupacaoPisos() {

}

//----------------------------------------------------
// Entrada de veículos
//----------------------------------------------------
int registarEntradaVeiculo() {

    return 0;
}

int atribuirLugar(int piso) {

    return 0;
}

void mostrarTicketEntrada(int numEntrada) {

}

//----------------------------------------------------
// Saída de veículos
//----------------------------------------------------
int registarSaidaVeiculo() {

    return 0;
}

float calcularValorAPagar(int numEntrada) {

    return 0.0;
}

void mostrarTicketSaida(int numEntrada) {

}

void alterarSaida(int numEntrada) {

}

void anularSaida(int numEntrada) {

}

//----------------------------------------------------
// Consulta, alteração e eliminação de registos
//----------------------------------------------------
void consultarEstacionamento(int numEntrada) {

}

void alterarEstacionamento(int numEntrada) {

}

void eliminarEstacionamento(int numEntrada) {

}

//----------------------------------------------------
// Lugares indisponíveis
//----------------------------------------------------
void marcarLugarIndisponivel() {

}

void reverterLugarIndisponivel() {

}

//----------------------------------------------------
// Mapa do piso
//----------------------------------------------------
void mostrarMapaPiso(int piso) {

}

//----------------------------------------------------
// Persistência
//----------------------------------------------------
void guardarDadosEmBinario() {

}

void gravarEstacionamentosTexto() {

}

void gravarErros() {

}

//----------------------------------------------------
// Listagens e paginação
//----------------------------------------------------
void listarEstacionamentos() {

}

void avancarPagina() {

}

void recuarPagina() {

}

void mostrarPagina(int pagina) {

}

void gravarListagemTXT() {

}

//----------------------------------------------------
// Funcionalidades adicionais obrigatórias (3 à escolha)
//----------------------------------------------------
void funcionalidadeExtra1() {

}

void funcionalidadeExtra2() {

}

void funcionalidadeExtra3() {

}

//----------------------------------------------------
// Extras opcionais (E1, E2, E3)
//----------------------------------------------------
void gerarGrafico() {

}

void gerarTabelaDinamica() {

}

void gerarCSV() {

}

