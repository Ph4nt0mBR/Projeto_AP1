

/*
    header.h
    Criado por: Bruno
    Data 26/11

    Nota: O núcledo da estrutura principal vai ficar aqui.
    Aqui deve ser incluido as .h e a main.c deve incluir esse arquivo [Samuel]

*/

// Incluido a estruturas.h, e os outros includes estão lá



#include "funcoes.h" // -- Não precisa incluir stdio.h aqui, já está em estruturas.h [Samuel]
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>


// Debug logging (enable by defining DEBUG = 1)
#ifndef DEBUG
#define DEBUG 1
#endif

#if DEBUG
#define LOG(fmt, ...) fprintf(stderr, "[DBG] %s:%d %s() - " fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_MSG(msg)  fprintf(stderr, "[DBG] %s:%d %s() - %s\n", __FILE__, __LINE__, __func__, msg)
#else
#define LOG(fmt, ...)  ((void)0)
#define LOG_MSG(msg)   ((void)0)
#endif

//=======================================================
// Utilitários
//=======================================================

void trimTexto(char* s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r' || s[n - 1] == ' ' || s[n - 1] == '\t')) {
        s[--n] = '\0';
    }
}

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int clamp(int valor, int min, int max) {
    if (valor < min) return min;
    if (valor > max) return max;
    return valor;
}

FILE* abrirArquivo(const char* caminho, const char* modo) {     //Função pra abrir tanto .txt ("r", "w", "a", "r+", "w+", "a+") quanto .bin ("rb", "wb", "ab", "rb+", "wb+", "ab+")
    return fopen(caminho, modo);
}

int colunaChar_Indice(char letraParaNum) { // Converte os char pra int
    letraParaNum = (char)toupper((unsigned char)letraParaNum);  //  o unsigned char evita problemas com chars acentuados, o char garante que só char entre no toupper [Samuel]
    if (letraParaNum >= 'A' && letraParaNum <= 'Z') {
        return letraParaNum - 'A';
    }
    return -1; // Retorna -1 se o caractere não for válido -- honestamente eu n entendi mt bem qual a diferença entre -1 e null, mas oh well [Samuel]
}

char colunaIndice_Char(const PARQUE* p, int intParaChar) { // Converte int -> letra com validação contra PARQUE ou MAX_FILA
    int limite = p ? p->filasPorPiso : MAX_FILA; //verifica se p e nulo, se for usa o maximo
    if (intParaChar >= 0 && intParaChar < limite) {
        return (char)('A' + intParaChar); //Manda de volta pra char -- Pode usar qnd for fazer printf [Samuel]
    }
    return '?'; // Retorna '?' se o int não for válido
}

int coordenadaValida(const PARQUE* p, int andar, char filaChar, int lugar) {  //Verifica se o lugar está dentro dos limites do estacionamento
    if (!p) return 0;
    if (andar < 0 || andar >= p->pisos) return 0;

    int filaIdx = colunaChar_Indice(filaChar); // Mudei a função de converter q tava aqui dentro pra fora, por pura frescura [Samuel]
    //verifica se fila e valida↓
    if (filaIdx < 0 || filaIdx >= p->filasPorPiso) return 0;//verifica se lugar e valido↓
    if (lugar < 0 || lugar >= p->lugaresPorFila) return 0;
    return 1; //todas as coordenadas sao validas
} //fiz isto as 02:21 da manha, se estiver uma porcaria avisem. vou mas e pra cama -Bruno-   -- Fiz umas mudanças [Samuel]


//=======================================================
// Guardar Info
//=======================================================

int guardarBinario(SISTEMA* s) {
    if (!s) return 0;
    FILE* fp = abrirArquivo(BIN_PATH, "wb");
    if (!fp) {
        fprintf(stderr, "Erro ao abrir/criar ficheiro %s!!\n", BIN_PATH);
        return 0;
    }

    const int versao = 1;
    if (fwrite(&versao, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }

    // Configurações do parque -- necessário para reconstruir o mapa
    if (fwrite(&s->parque.pisos, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    if (fwrite(&s->parque.filasPorPiso, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    if (fwrite(&s->parque.lugaresPorFila, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }

    // Mapa 3D do parque -- Já diria Todd Howard "it just works"
    const size_t mapaGerador = (size_t)MAX_PISO * (size_t)MAX_FILA * (size_t)MAX_LUGARES;
    if (fwrite(s->parque.mapa, sizeof(EstadoLugar), mapaGerador, fp) != mapaGerador) {
        fclose(fp);
        return 0;
    }

	// Tarifas
    if (fwrite(&s->totalTarifas, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    if (s->totalTarifas < 0 || s->totalTarifas > MAX_TARIFAS) {
        fclose(fp);
        return 0;
    }
    if (s->totalTarifas > 0) {
        if (fwrite(s->tarifas, sizeof(TARIFARIO), (size_t)s->totalTarifas, fp) != (size_t)s->totalTarifas) {
            fclose(fp);
            return 0;
        }
    }

	// Estacionamentos
    if (fwrite(&s->ultimoNumEntrada, sizeof(int), 1, fp) != 1) {
        fclose(fp);
		return 0;
    }
    if (fwrite(&s->totalEstacionamentos, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    if (s->totalEstacionamentos > 0) {
        if (fwrite(s->estacionamentos, sizeof(VAGAS), (size_t)s->totalEstacionamentos, fp) != (size_t)s->totalEstacionamentos) {
            fclose(fp);
            return 0;
        }
    }

	fflush(fp);
    fclose(fp);
    printf("Dados guardados com sucesso em: %s\n", BIN_PATH);
	return 1;
}

//=======================================================
// Leituras -- Prioriza ler .bin, caso contrario lê .txt
//=======================================================

int carregarBinario(SISTEMA* s) {
    if (!s) return 0;

    FILE* fp = abrirArquivo(BIN_PATH, "rb");
    if (!fp) return 0; //Caso n tenha o .bin

    int versao = 0;
    if (fread(&versao, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    if (versao != 1) {
		fclose(fp);
		return 0;
    }
    
	// Configurações do parque
	int pisos = 0, filas = 0, lugares = 0;
    if (fread(&pisos, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    if (fread(&filas, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    if (fread(&lugares, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }

	s->parque.pisos = clamp(pisos, 1, MAX_PISO);
	s->parque.filasPorPiso = clamp(filas, 1, MAX_FILA);
    s->parque.lugaresPorFila = clamp(lugares, 1, MAX_LUGARES);

    // Mapa 3D do parque
	const size_t mapaGerador = (size_t)MAX_PISO * (size_t)MAX_FILA * (size_t)MAX_LUGARES;
    if(fread(s->parque.mapa, sizeof(EstadoLugar), mapaGerador, fp) != mapaGerador) {
        fclose(fp);
        return 0;
	}

    // Tarifas
    if (fread(&s->totalTarifas, sizeof(int), 1, fp) != 1) {
        fclose(fp);
		return 0;
    }
    if(s->totalTarifas < 0 || s->totalTarifas> MAX_TARIFAS) {
        fclose(fp);
        return 0;
	}
    if (s->totalTarifas > 0) {
        if(fread(s->tarifas, sizeof(TARIFARIO), (size_t)s->totalTarifas, fp) != (size_t)s->totalTarifas) {
            fclose(fp);
            return 0;
		}
    }

	// Estacionamentos
    if (fread(&s->ultimoNumEntrada, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
	int ttlEsts = 0;
    if (fread(&ttlEsts, sizeof(int), 1, fp) != 1) {
		fclose(fp);
		return 0;
    }
    if (ttlEsts < 0) {
        fclose(fp);
		return 0;
    }
    if (ttlEsts == 0) {
        free(s->estacionamentos);
		s->estacionamentos = NULL;
		s->totalEstacionamentos = 0;
    }
    else {
        VAGAS* nova = (VAGAS*)realloc(s->estacionamentos, sizeof(VAGAS) * (size_t)ttlEsts);
        if (!nova) {
			fclose(fp);
			return 0;
        }
		s->estacionamentos = nova;
		s->totalEstacionamentos = ttlEsts;

        if (fread(s->estacionamentos, sizeof(VAGAS), (size_t)ttlEsts, fp) != (size_t)ttlEsts) {
            fclose(fp);
            return 0;
		}
    }

	fclose(fp);
    printf("Dados carregados com sucesso de: %s\n", BIN_PATH);
	return 1;
}

int leituraConstante(SISTEMA* s) {
    if (!s) {
		fprintf(stderr, "Erro: sistema nulo.\n");
        return 0;
    }
    if (!carregarBinario(s)) {// Só carrega dados.bin se existir
        fprintf(stderr, "Aviso: ficheiro binário não disponível ou inválido.\n");
        return 0;
    }
    return 1;
}

int primeiraLeitura(SISTEMA* s) {
    if (!s) {
        fprintf(stderr, "Erro: sistema nulo.\n");
        return 0;
    }
    configurarParque(&s->parque);   //1° Configura o parque

    carregarTarifasDeFicheiro(s);   //2° carrega os ficheiros txt
    carregarEstacionamentosDeFicheiro(s);

    /*  Copilot sugeriu isso daqui. Avaliar posteriormente se vale a pena
        if (s->totalTarifas == 0 && s->totalEstacionamentos == 0) return LER_FICHEIRO_VAZIO;    */

    if (!guardarBinario(s)) {
        fprintf(stderr, "Erro: Não foi possivel criar/guardar %s!\n", BIN_PATH);
        return 0;
    }

    return 1;
}

//====================================================
// Texto: tarifas e estacionamentos
//====================================================

void carregarTarifasDeFicheiro(SISTEMA* s) {//Bruno -- Fiz um cado de modificações [Samuel]

    if (!s) {   //verifica se o sistema e valido
        fprintf(stderr, "Erro: sistema nulo.\n\n");
        return; //se nao for, nao carrega
    }

    FILE* f = abrirArquivo(TARIFAS_PATH, "r"); //abre o arquivo de tarifas em read only
    if (!f) {
        fprintf(stderr, "Erro ao abrir o ficheiro %s.\n ", TARIFAS_PATH);
        return; //se falhar a abrir, sai
    }

    s->totalTarifas = 0;
    char linha[30];

    while (fgets(linha, sizeof(linha), f) != NULL) { //le todas as linhas do arquivo
        if (linha[0] == '\0' || linha[0] == '\n') continue; //ignora linhas em branco

        if(linha[0] == '<' || strstr(linha, "TpTarifa") != NULL) {
            continue; //ignora linhas de cabeçalho ou comentários
		}
        //etiqueta e valor (tirados do tarifas.txt)
        char tipo;              // H para por hora e D para diaria 
        char codigo[8] = { 0 }; // CT1-4
		char horaInf[6] = { 0 };// hora entrada
		char horaSup[6] = { 0 };// hora saída
		char valorStr[10] = { 0 }; //string temporaria para guardar o valor

        if (sscanf(linha, " %c %7s %5s %5s %9s", &tipo, codigo, horaInf, horaSup, valorStr) != 5) { //separa os dois valores diferentes, etiqueta e valor
            fprintf(stderr, "Linha de tarifa invalida: %s\n", linha); //mensagem de erro caso a linha nao esteja no formato correto
            continue; //passa pra a proxima linha
        }

        valorStr[strcspn(valorStr, "€")] = '\0'; //remove o simbolo de euro se existir
        trimTexto(valorStr);

        //vai converter a string para um float
        char* endptr = NULL;
        float valorHora = strtof(valorStr, &endptr);
        if (endptr == valorStr) { //verifica se o strtof falhou (strtof é o que converte string para float)
            fprintf(stderr, "Valor de tarifa invalido: %s", linha);
            continue;
        }

        //mensagem de erro se passar do maximo
        if (s->totalTarifas >= MAX_TARIFAS) {
            fprintf(stderr, "Limite MAX_TARIFAS atingido. Ignorando restante.\n");
            break;
        }

        //guarda
        TARIFARIO t;
		memset(&t, 0, sizeof(TARIFARIO)); //zera a struct
		t.tipo = tipo;
        strncpy(t.codigo, codigo, sizeof(t.codigo) - 1);
		strncpy(t.horaInf, horaInf, sizeof(t.horaInf) - 1);
        strncpy(t.horaSup, horaSup, sizeof(t.horaSup) - 1);
		t.valor = valorHora;

		s->tarifas[s->totalTarifas++] = t; //adiciona a tarifa ao sistema
    }
    fclose(f);
    printf("Tarifas carregadas com sucesso: %d\n", s->totalTarifas);
}

void carregarEstacionamentosDeFicheiro(SISTEMA* s){ // Bruno
    // verifica se o sistema é válido
    if (!s) {
        fprintf(stderr, "Erro: sistema nulo.\n");
        return;
    }

    FILE* f = abrirArquivo(ESTACIONAMENTOS_PATH, "r");
    if (!f) {
        fprintf(stderr, "Houve um erro ao abrir o ficheiro %s.\n", ESTACIONAMENTOS_PATH);
        return;
    }

    s->totalEstacionamentos = 0;

    char linha[55];
    while (fgets(linha, sizeof(linha), f) != NULL) {
        if (linha[0] == '\0' || linha[0] == '\n' || linha[0] == '\r') continue; //ignora linhas em branco

        VAGAS e; // FIX: Initialize all fields of 'e' to zero/default
        memset(&e, 0, sizeof(VAGAS)); //zera todos os bytes da struct, garantindo que todos os campos iniciem com valores padrão (0 ou string vazia).

        if (sscanf(linha, "%d %10s %10s %d %c %d %5s %d",
            &e.id,
            e.matricula,
            e.dataEntrada,   // mantido como estava logicamente (campo existente)
            &e.andar,
            &e.fila,
            &e.lugar,
            e.horaEntrada,
            (int*)&e.estado) != 8) {// valor lido, mesmo que depois seja ajustado
            fprintf(stderr, "Linha de estacionamento invalida: %s\n", linha);
            continue;
        }

        if (s->totalEstacionamentos >= MAX_ESTACIONAMENTOS) {
            fprintf(stderr, "Limite MAX_ESTACIONAMENTOS ultrapassado.\n");
            break;
        }
        e.fila = (char)toupper((unsigned char)e.fila);

        if (!coordenadaValida(&s->parque, e.andar, e.fila, e.lugar)) {
            fprintf(stderr, "Registro ignorado: Coordenadas inválidas (Andar: %d, Fila: %c, Lugar: %d).\n",
				e.andar, e.fila, e.lugar);
        }

        int filaIdx = colunaChar_Indice(e.fila);
        if (filaIdx < 0) {
			fprintf(stderr, "Registro ignorado: Fila inválida '%c'.\n", e.fila);
            continue;
        }

        switch (e.estado) { //Basicamente lida cm o enum do lugar
        case LUGAR_OCUPADO:
            s->parque.mapa[e.andar][filaIdx][e.lugar] = LUGAR_OCUPADO;
			break;
        case LUGAR_INDISPONIVEL:
			s->parque.mapa[e.andar][filaIdx][e.lugar] = LUGAR_INDISPONIVEL;
			break;
        default:
            s->parque.mapa[e.andar][filaIdx][e.lugar] = LUGAR_LIVRE;
			break;
        }
        s->estacionamentos[s->totalEstacionamentos++] = e;
    }

    fclose(f);
    printf("Estacionamentos carregados com sucesso: %d\n", s->totalEstacionamentos);
}


//====================================================
// Inicialização e carregamento de dados
//====================================================

void inicializarSistema(SISTEMA* s) {
    if (!s) return;

	memset(s, 0, sizeof(SISTEMA)); //zera todos os campos da struct sistema

	//Alocação dinâmica para os estacionamentos
	s->estacionamentos = malloc(sizeof(VAGAS) * MAX_ESTACIONAMENTOS);
    if (!s->estacionamentos) {
        fprintf(stderr, "Erro ao alocar memoria para estacionamentos!\n");
        return;
    }
	memset(s->estacionamentos, 0, sizeof(VAGAS) * MAX_ESTACIONAMENTOS); //zera a memoria alocada

	// Configuração padrão do parque
    s->parque.pisos = 1;
	s->parque.filasPorPiso = 1;
	s->parque.lugaresPorFila = 1;
}


void configurarParque(PARQUE* p) { //Bruno
    //Verifica se o ponteiro e valido↓
    if (p == NULL) {
        fprintf(stderr, "Erro, ponteiro nulo.\n");
        return;
    }

	limparBuffer(); //limpa o buffer do teclado

    while (1) { //pergunta e ve se e valido a quantidade de pisos. usa um loop ate user dar un vakor valido, estes 3 blocos de codigo fazem da mesma forma
        printf("Numero de pisos (1-%d): ", MAX_PISO);
        if (scanf("%d", &p->pisos) == 1 && p->pisos >= 1 && p->pisos <= MAX_PISO) { //verifica se e valido
			limparBuffer();
            break; //break para sair do while
        }
        fprintf(stderr, "Valor invalido, tente novamente.\n");
		limparBuffer();
    }

    while (1) { //pergunta quantidade de filas por piso
        printf("Numero de filas por piso (1-%d): ", MAX_FILA);
        if (scanf("%d", &p->filasPorPiso) == 1 && p->filasPorPiso >= 1 && p->filasPorPiso <= MAX_FILA) { //verifica se e valido
			limparBuffer();
            break; //break para sair do while
        }
        fprintf(stderr, "Valor invalido, tente novamente.\n");
		limparBuffer();
    }

    while (1) { //pergunta quantidade de lugares por fila
        printf("Numero de lugares por fila (1-%d): ", MAX_LUGARES);
        if (scanf("%d", &p->lugaresPorFila) == 1 && p->lugaresPorFila >= 1 && p->lugaresPorFila <= MAX_LUGARES) { //verifica se e valido
			limparBuffer();
            break; //break para sair do while
        }
        fprintf(stderr, "Valor invalido, tente novamente.\n");
		limparBuffer();
    }

    // Inicializa apenas dentro dos limites do utilizador
    // O mapa já está zerado (LUGAR_LIVRE == 0) pelo memset em inicializarSistema
    // Aqui só garantimos que está correto
    for (int a = 0; a < p->pisos; a++) {
        for (int f = 0; f < p->filasPorPiso; f++) {
            for (int l = 0; l < p->lugaresPorFila; l++) { //percorre todos os pisos filas e lugares
                p->mapa[a][f][l] = LUGAR_LIVRE;
            }
        }
    }

    printf("Parque configurado: %d pisos, %d filas por piso, %d lugares por fila.\n",
		p->pisos, p->filasPorPiso, p->lugaresPorFila);
}




// Removi as funções de carregar tarifas e estacionamento. Vou converter em uma função que inicia a primeira leitura dos arquivos .atxt e depois dos arquivos em .bin [Samuel]


// Carregar ficheiros de dados

//esta funcao e basicamente "formatar" o sistema. Tudo volta ao "estado inicial"

//----------------------------------------------------
// Menus
//----------------------------------------------------
/*void mostrarMenuPrincipal() {

}

void mostrarOcupacaoPisos() {

}
*/

//----------------------------------------------------
// Entrada de veículos
//----------------------------------------------------

int registarEntradaVeiculo(SISTEMA* s) { //Bruno
    if (!s) return -1;


	//Somente um check-up da capacidade
    if (s->totalEstacionamentos >= MAX_ESTACIONAMENTOS) {
		fprintf(stderr, "Erro: Capacidade máxima de estacionamentos atingida.\n");
		return -1;
    }
        
    VAGAS novo;
    memset(&novo, 0, sizeof(VAGAS)); //zera todos os campos da struct

    printf("Matricula do veiculo: ");
    if (scanf("%15s", novo.matricula) != 1) {
		fprintf(stderr, "Erro ao ler matricula.\n");
		limparBuffer();
		return -1;
    }
	limparBuffer();

    int piso; //escolher piso
    printf("Piso pretendido (1 a %d): ", s->parque.pisos);
    if (scanf("%d", &piso) != 1) {
		fprintf(stderr, "Erro ao ler piso.\n");
		limparBuffer();
		return -1;
    }
	limparBuffer();
	piso--; //ajusta para indice 0-based

    if (piso < 0 || piso >= s->parque.pisos) { //validação piso
        fprintf(stderr, "Piso inválido!\n");
        return -1;
    }

    int filaIdx = -1, lugarIdx = -1;
    if (atribuirLugar(s, piso, &filaIdx, &lugarIdx) != 0) {
        fprintf(stderr, "Nao existem lugares disponiveis neste piso.\n");
        return -1;
    }

    novo.andar = piso; //guarda o piso
    novo.fila = (char)('A' + filaIdx); // índice 0..filasPorPiso-1 para letra
    novo.lugar = lugarIdx;             //guarda o lugar

    printf("Data de entrada (dd/mm/aaaa): ");
    if (scanf("%10s", novo.dataEntrada) != 1) {
		fprintf(stderr, "Erro ao ler data de entrada.\n");
		limparBuffer();
		return -1;
    }
	limparBuffer();

    printf("Hora de entrada (HH:MM): ");
    if (scanf("%5s", novo.horaEntrada) != 1) {
		fprintf(stderr, "Erro ao ler hora de entrada.\n");
		limparBuffer();
		return -1;
    }
	limparBuffer();

    novo.dataSaida[0] = '\0';
    novo.horaSaida[0] = '\0'; //inicializa data de saida e hora de saida como vazias
    novo.estado = LUGAR_OCUPADO;

    novo.id = ++s->ultimoNumEntrada;

    // Atualiza o mapa com índices corretos
    s->parque.mapa[piso][filaIdx][lugarIdx] = LUGAR_OCUPADO;

    //adiciona o registo
    s->estacionamentos[s->totalEstacionamentos++] = novo;

    printf("\n=========== TICKET ENTRADA ===========\n");
    printf("Ticket Nº: %d\n", novo.id);
    printf("Matricula: %s\n", novo.matricula);
    printf("Entrada:   %s %s\n", novo.dataEntrada, novo.horaEntrada);
    printf("Local:     Piso %d, Fila %c, Lugar %d\n",
        novo.andar + 1, novo.fila, novo.lugar + 1);
    printf("=======================================\n");

    return novo.id;
}

int atribuirLugar(SISTEMA* s, int piso, int* filaOut, int* lugarOut) //Bruno
{
    //valida entradas
    if (s == NULL || filaOut == NULL || lugarOut == NULL)
        return 1;

    //verifica validade piso
    if (piso < 0 || piso >= s->parque.pisos)
        return 1; //caso o piso seja invalido return 1

    const int filas = s->parque.filasPorPiso;
    const int lugares = s->parque.lugaresPorFila;

    if (filas <= 0 || lugares <= 0) {
        fprintf(stderr, "Nao tem capacidade definida: (filas = %d, lugares = %d", filas, lugares);


        return 2;
    }

    static int ultimoFilaPorPiso[MAX_PISO] = { 0 };
    static int ultimoLugarPorPiso[MAX_PISO] = { 0 };
    int startFila = ultimoFilaPorPiso[piso] % filas;
    int startLugar = ultimoLugarPorPiso[piso] % lugares;

    for (int offsetF = 0; offsetF < filas; ++offsetF) {
        int f = (startFila + offsetF) % filas;

        // Varre de startLugar ao fim
        for (int l = startLugar; l < lugares; ++l) {
            if (s->parque.mapa[piso][f][l] == LUGAR_LIVRE) {
                *filaOut = f;
                *lugarOut = l;
                ultimoFilaPorPiso[piso] = f;
                ultimoLugarPorPiso[piso] = (l + 1) % lugares;
                return 0;
            }
        }
        // Varre do inicio até startLugar-1
        for (int l = 0; l < startLugar; ++l) {
            if (s->parque.mapa[piso][f][l] == LUGAR_LIVRE) {
                *filaOut = f;
                *lugarOut = l;
                ultimoFilaPorPiso[piso] = f;
                ultimoLugarPorPiso[piso] = (l + 1) % lugares;
                return 0;
            }
        }
    }

    fprintf(stderr, "Nenhum lugar disponivel.\n");
    return 2;
}

void mostrarTicketEntrada(int numEntrada) {

}

//----------------------------------------------------
// Saída de veículos
//----------------------------------------------------
/*
int registarSaidaVeiculo() {

    return 0;
}

float calcularValorAPagar(int numEntrada) {

    return 0.0;
}

void mostrarTicketSaida(int numEntrada) {

}


void alterarSaida(int numEntrada) {                                          //Objetivo: Alterar a hora registada de saída de um veículo
    if (numEntrada < 0 || parqueRegisto[numEntrada].ativo == 1) {            //numEntrada → índice/registo do veículo no parque
        printf("Erro: Veiculo ainda no parque ou ID invalido!\n");
        return;
    }

    printf("Nova hora de saida (hh mm): ");
    scanf("%d %d", &parqueRegisto[numEntrada].saida.hora,
        &parqueRegisto[numEntrada].saida.minuto);

    printf("Saida alterada com sucesso!\n");

}  //ta a dar asneira, TUDO q ta em comment assim ta a dar asneira cm erros que nao sao simples



void anularSaida(int numEntrada) {
    if (numEntrada < 0) return;                                      //O veiculo ja deve ter saida registada (ativo == 0)
    //Se for inválido, a função não faz
    parqueRegisto[numEntrada].ativo = 1;
    printf("Saida anulada!\n");

}
*/
//----------------------------------------------------
// Consulta, alteração e eliminação de registos
//----------------------------------------------------

void consultarEstacionamento(SISTEMA* s, int numEntrada) {
    if (!s || !s->estacionamentos) { //valida ponteiro sistema e estacionamentos
        fprintf(stderr, "Erro: sistema ou estacionamentos nulo(s). \n");
        return;
    }

    if (numEntrada <= 0 || numEntrada > s->totalEstacionamentos) { //valida numero de entrada
        fprintf(stderr, "Erro: Numero de entrada invalido.\n");
        return;
    }

    //procura o estacionamento com o ID correspondente
    const VAGAS* v = NULL;
    for (int i = 0; i < s->totalEstacionamentos; i++) {
        if (s->estacionamentos[i].id == numEntrada) {
            v = &s->estacionamentos[i];
            break;
        }
    }

    if (!v) { //se n encontrar, emnsagem de nao encontrado
        fprintf(stderr, "Erro: Estacionamento com ID %d nao encontrado.\n", numEntrada);
        return;
    }

    const char* estadoStr =
        (v->estado == LUGAR_OCUPADO) ? "Ocupado" :
        (v->estado == LUGAR_LIVRE) ? "Livre" :
        (v->estado == LUGAR_INDISPONIVEL) ? "Indisponivel" :
        "Desconhecido";



    //mostra detalhes do estacionamento
    printf("------Consulta de Estacionamento------\n");
    printf("Ticket Nº: %d\n", v->id);               //numero ticket
    printf("Matricula: %s\n", v->matricula);         //matricula
    printf("Entrada: %s %s\n", v->dataEntrada, v->horaEntrada); //data e hora entrada
    printf("Saida: %s %s\n", v->dataSaida[0] ? v->dataSaida : "-", v->horaSaida[0] ? v->horaSaida : "-"); //data e hora saida
    printf("Local: Piso %d, Fila %c, Lugar %d\n", v->andar, v->fila, v->lugar);             //coordenadas
    printf("Estado: %s\n", estadoStr); 					 //estado do lugar


    //valida coordenadas e mostra estado no mapa
    if (!coordenadaValida(&s->parque, v->andar, v->fila, v->lugar)) {
        fprintf(stderr, "Aviso, coordenadas fora dos limites do parque\n"); //se n for valido, avisa
    }
    else {
        int filaIdx = colunaChar_Indice(v->fila); //converte fila char pra indice
        EstadoLugar mapaEstado = s->parque.mapa[v->andar][filaIdx][v->lugar]; //saca o estado do mapa (usa . em PARQUE)
        const char* mapaStr =
            (mapaEstado == LUGAR_OCUPADO) ? "Ocupado" :
            (mapaEstado == LUGAR_LIVRE) ? "Livre" :
            (mapaEstado == LUGAR_INDISPONIVEL) ? "Indisponivel" : "Desconhecido";
        printf("Mapa confirma: %s\n", mapaStr); //confirma estado no mapa
    }
    printf("==============================================\n");
}

void alterarEstacionamento(SISTEMA* s, int numEntrada) { //eu n faco puta se isto funciona, a meio do caminho o cerebro deciciu nao funcionar -Bruno-
    if (!s || !s->estacionamentos) { //valida ponteiro e estacionamentos
        fprintf(stderr, "Erro: sistema e/ou estacionamentos nulo(s).\n");
        return;
    }
    //localiza o registo pelo ID(ticket)
    int idx = -1;
    for (int i = 0; i < s->totalEstacionamentos; i++) { //ciclo for pra percorrer os registos
        if (s->estacionamentos[i].id == numEntrada) { //compara o id
            idx = i;
            break;
        }
    }
    if (idx < 0) { //caso nao encontre
        fprintf(stderr, "Erro:Estacionamento com ID %d nao encontrado.\n", numEntrada);
        return;
    }

    VAGAS* v = &s->estacionamentos[idx];

    //opcoes
    printf("Alterar estacionamento (Ticket Nº %d)\n", v->id);
    printf("1) Matricula\n");
    printf("2) Mover lugar (andar/fila/lugar)\n");
    printf("3) Data/Hora de entrada\n");
    printf("4) Data/Hora de saida\n");
    printf("5) Estado (0=Livre, 1=Ocupado, 2=Indisponivel)\n");
    printf("0) Cancelar\n");
    printf("Opcao: ");

    int opcao = -1;
    if (scanf("%d", &opcao) != 1) {
        fprintf(stderr, "Entrada invalida.\n");
        return;
    }

    if (opcao == 0) {
        printf("Operacao cancelada.\n");
        return;
    }

    switch (opcao) {
    case 1: {
        char novaMatricula[sizeof(v->matricula)] = { 0 }; //buffer com tamanho certo
        printf("Nova matricula: ");
        scanf("%15s", novaMatricula);
        strncpy(v->matricula, novaMatricula, sizeof(v->matricula) - 1); //copia a nova matricula
        v->matricula[sizeof(v->matricula) - 1] = '\0'; //garante que termina em null
        printf("Matricula alterada com sucesso.\n"); //ifnorma sucesso
        break;

    }

    case 2: {
        //mover veiculo pra novo lugar
        int novoAndar;
        char novaFilaChar;
        int novoLugar;

        printf("Novo andar (0..%d): ", s->parque.pisos - 1); //limite de andares
        if (scanf("%d", &novoAndar) != 1) { //valida andar
            fprintf(stderr, "Entrada invalida para andar.\n");
            return;
        }

        printf("Nova fila (A..%c): ", (char)('A' + s->parque.filasPorPiso - 1)); //limite de filas
        scanf(" %c", &novaFilaChar); //valida fila

        printf("Novo lugar (0..%d): ", s->parque.lugaresPorFila - 1); //limite de lugares por fila
        if (scanf("%d", &novoLugar) != 1) { //valida lugar
            fprintf(stderr, "Entrada invalida para lugar.\n");
            return;
        }

        if (!coordenadaValida(&s->parque, novoAndar, novaFilaChar, novoLugar)) { //Valida coordenadas
            fprintf(stderr, "Coordenadas invalidas.\n");
            return;
        }

        int novaFilaIdx = colunaChar_Indice(novaFilaChar); //converte fila char pra indice
        if (novaFilaIdx < 0) { //valida fila convertida
            fprintf(stderr, "Fila invalida.\n");
            return;
        }

        EstadoLugar estadoDestino = s->parque.mapa[novoAndar][novaFilaIdx][novoLugar]; //saca o estado do lugar destino
        if (estadoDestino != LUGAR_LIVRE) { //verifica se esta livre
            fprintf(stderr, "Lugar destino nao esta livre.\n");
            return;
        }

        //liberta lugar antigo se estava oucupado
        int filaAntigaIdx = colunaChar_Indice(v->fila);
        if (filaAntigaIdx >= 0 &&
            coordenadaValida(&s->parque, v->andar, v->fila, v->lugar) &&
            s->parque.mapa[v->andar][filaAntigaIdx][v->lugar] == LUGAR_OCUPADO) {
            s->parque.mapa[v->andar][filaAntigaIdx][v->lugar] = LUGAR_LIVRE;
        }

        //marca o novo lugar conforme estado atual
        if (v->estado == LUGAR_OCUPADO) {
            s->parque.mapa[novoAndar][novaFilaIdx][novoLugar] = LUGAR_OCUPADO;
        }

        v->andar = novoAndar; //atualiza andar
        v->fila = colunaIndice_Char(&s->parque, novaFilaIdx); //atualiza fila
        v->lugar = novoLugar; //atualiza lugar

        printf("Lugar atualizado: Piso %d, Fila %c, Lugar %d\n", v->andar, v->fila, v->lugar);
        break;
    }

    case 3: { //alterar data/hora de entrada
        char novaData[sizeof(v->dataEntrada)] = { 0 }; //buffe r data
        char novaHora[sizeof(v->horaEntrada)] = { 0 }; //buffer hora
        printf("Nova data de entrada (dd/mm/aaaa): ");
        scanf("%10s", novaData); //le data com limite de caracteres segundo o formato
        printf("Nova hora de entrada (HH:MM): ");
        scanf("%5s", novaHora); //le hora com limite de caracters segundo o formato

        strncpy(v->dataEntrada, novaData, sizeof(v->dataEntrada) - 1); //copia data
        v->dataEntrada[sizeof(v->dataEntrada) - 1] = '\0'; //garante que termina em null
        strncpy(v->horaEntrada, novaHora, sizeof(v->horaEntrada) - 1); //copia hora
        v->horaEntrada[sizeof(v->horaEntrada) - 1] = '\0'; //garante que termina em null

        printf("Data/Hora de entrada atualizadas.\n");
        break;
    }
    case 4: { //alterar data/hora de saida
        char novaData[sizeof(v->dataSaida)] = { 0 }; //buffer data
        char novaHora[sizeof(v->horaSaida)] = { 0 }; //buffer hora
        printf("Nova data de saida (dd/mm/aaaa, '-' para limpar): ");
        scanf("%10s", novaData);
        printf("Nova hora de saida (HH:MM, '-' para limpar): ");
        scanf("%5s", novaHora);

        if (strcmp(novaData, "-") == 0) { //se for "-", limpa
            v->dataSaida[0] = '\0'; //limpa
        }
        else {
            strncpy(v->dataSaida, novaData, sizeof(v->dataSaida) - 1); //copia data
            v->dataSaida[sizeof(v->dataSaida) - 1] = '\0'; //garante que termina em null
        }

        if (strcmp(novaHora, "-") == 0) { //se for "-", limpa
            v->horaSaida[0] = '\0'; //limpa
        }
        else {
            strncpy(v->horaSaida, novaHora, sizeof(v->horaSaida) - 1); //copia hora
            v->horaSaida[sizeof(v->horaSaida) - 1] = '\0'; //garante que termina em null
        }

        printf("Data/Hora de saida atualizadas.\n"); //mensagem de sucesso
        break;
    }
    case 5: { //alterar estado
        int novoEstado; //buffer novo estado
        printf("Novo estado (0=Livre, 1=Ocupado, 2=Indisponivel): ");
        if (scanf("%d", &novoEstado) != 1 || novoEstado < (int)LUGAR_LIVRE || novoEstado >(int)LUGAR_INDISPONIVEL) { //valida estado
            fprintf(stderr, "Estado invalido.\n");
            return;
        }

        //reflete no mapa se a coordenada for valida
        if (coordenadaValida(&s->parque, v->andar, v->fila, v->lugar)) {
            int filaIdx = colunaChar_Indice(v->fila);
            if (filaIdx >= 0) {
                s->parque.mapa[v->andar][filaIdx][v->lugar] = (EstadoLugar)novoEstado;
            }
        }

        v->estado = (EstadoLugar)novoEstado; //atualiza estado
        printf("Estado atualizado.\n");
        break;
    }
    default:
        fprintf(stderr, "Opcao invalida.\n");
        return;
    }

    //tenta persistir alteracoes
    if (!guardarBinario(s)) {
        fprintf(stderr, "Aviso: Falha ao guardar em binario.\n");
    }
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
/*void mostrarMapaPiso(int piso) {

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
*/