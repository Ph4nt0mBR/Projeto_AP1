#ifndef funcoes.h
#define funcoes.h

//-----------------------------------------------
//  Inicialização e carregamento de dados
//-----------------------------------------------
void configurarParque(); 
void carregarTarifasDeFicheiro();
void carregarEstacionamentosDeFicheiro();
void inicializarSistema();

//-----------------------------------------------
//  Menus
//-----------------------------------------------
void mostrarMenuPrincipal();
void mostrarOcupacaoPisos();

//-----------------------------------------------
//  Gestão da entrada de veículos
//-----------------------------------------------
int registarEntradaVeiculo();
int atribuirLugar(int piso);
void mostrarTicketEntrada(int numEntrada);

//-----------------------------------------------
//  Gestão de saídas de veículos
//-----------------------------------------------
int registarSaidaVeiculo();
float calcularValorAPagar(int numEntrada);
void mostrarTicketSaida(int numEntrada);
void alterarSaida(int numEntrada);
void anularSaida(int numEntrada);

//-----------------------------------------------
//  Consulta, alteração e eliminação de registos
//-----------------------------------------------
void consultarEstacionamento(int numEntrada);
void alterarEstacionamento(int numEntrada);
void eliminarEstacionamento(int numEntrada);

//-----------------------------------------------
//  Gestão de lugares indisponíveis
//-----------------------------------------------
void marcarLugarIndisponivel();
void reverterLugarIndisponivel();

//-----------------------------------------------
//  Mapa de ocupação por piso
//-----------------------------------------------
void mostrarMapaPiso(int piso);

//-----------------------------------------------
//  Persistência
//-----------------------------------------------
void guardarDadosEmBinario();
void gravarEstacionamentosTexto();
void gravarErros();

//-----------------------------------------------
//  Listagens com paginação
//-----------------------------------------------
void listarEstacionamentos();
void avancarPagina();
void recuarPagina();
void mostrarPagina(int pagina);
void gravarListagemTXT();

//-----------------------------------------------
//  Funcionalidades adicionais (alínea 11)
//-----------------------------------------------
void funcionalidadeExtra1();
void funcionalidadeExtra2();
void funcionalidadeExtra3();

//-----------------------------------------------
//  Extras opcionais (E1, E2, E3)
//-----------------------------------------------
void gerarGrafico();
void gerarTabelaDinamica();
void gerarCSV();

#endif /* funcoes.h */
