#define	_CRT_SECURE_NO_WARNINGS

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
//  Guardar Info
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
//   Leituras -- Prioriza ler .bin, caso contrario lê .txt
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
//   Texto: tarifas e estacionamentos
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
//  Inicialização e carregamento de dados
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


//====================================================
//  Entrada de veículos
//====================================================

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


//====================================================
//  Gestão de registos
//====================================================

    //  ===== Ferramentas de suporte =====  //

VAGAS* encontrarEstacionamento(SISTEMA* s, int numEntrada) {
    if (!s || !s->estacionamentos || numEntrada <= 0) {
		return NULL;
    }

	//Pesquisa linear nos registos por ID
    for (int i = 0; i < s->totalEstacionamentos; i++) {
        if (s->estacionamentos[i].id == numEntrada) {
			return &s->estacionamentos[i];
        }
    }
	return NULL; //Nao encontrado
}

int alterarMatricula(VAGAS* v, const char* novaMatricula) {
    if(!v || !novaMatricula) return 0;

    strncpy(v->matricula, novaMatricula, sizeof(v->matricula) - 1);
    v->matricula[sizeof(v->matricula) - 1] = '\0';

    return 1;
}

int moverLugar(SISTEMA* s, VAGAS* v, int novoPiso, char novaFila, int novoLugar) {
	if (!s || !v) return 0;

	// Update - Valida novas coordenadas
    if (!coordenadaValida(&s->parque, novoPiso, novaFila, novoLugar)) {
		return 0; //coordenadas invalidas
    }

	//Update - Conversão letra para índice
    int novaFilaIdx = colunaChar_Indice(novaFila);
	if (novaFilaIdx < 0) return 0;


	// Update - Verifica se o lugar destino está livre
	EstadoLugar estadoDestino = s->parque.mapa[novoPiso][novaFilaIdx][novoLugar];
    if (estadoDestino != LUGAR_LIVRE) {
		return 0; //lugar ocupado ou indisponivel
    }

	// Update - Libera lugar ocupado anteriormente
    int filaAntigaIdx = colunaChar_Indice(v->fila);
    if(filaAntigaIdx >= 0 && coordenadaValida(&s->parque, v->andar, v->fila, v->lugar) && s->parque.mapa[v->andar][filaAntigaIdx][v->lugar] == LUGAR_OCUPADO) {
        s->parque.mapa[v->andar][filaAntigaIdx][v->lugar] = LUGAR_LIVRE;
	}

	// Update - Ocupa novo lugar (Somente se o estado for ocupado)
    if (v->estado == LUGAR_OCUPADO) {
		s->parque.mapa[novoPiso][novaFilaIdx][novoLugar] = LUGAR_OCUPADO;
    }

	// Update - Atualiza registo de coordenadas
	v->andar = novoPiso;
    v->fila = (char)('A' + novaFilaIdx);
	v->lugar = novoLugar;

    return 1;
}

int alterarDataHoraEntrada(VAGAS* v, const char* novaData, const char* novaHora) {
    if (!v || !novaData || !novaHora) return 0;

    // Atualiza data
    strncpy(v->dataEntrada, novaData, sizeof(v->dataEntrada) - 1);
	v->dataEntrada[sizeof(v->dataEntrada) - 1] = '\0';

	// Atualiza hora
    strncpy(v->horaEntrada, novaHora, sizeof(v->horaEntrada) - 1);
    v->horaEntrada[sizeof(v->horaEntrada) - 1] = '\0';

	return 1;
}

int alterarDataHoraSaida(VAGAS* v, const char* novaData, const char* novaHora) {
	if (!v || !novaData || !novaHora) return 0;

	// Atualiza data -- "-" limpa/ Caso contrario atualiza 
    if (strcmp(novaData, "-") == 0) {
		v->dataSaida[0] = '\0';
    }
    else {
        strncpy(v->dataSaida, novaData, sizeof(v->dataSaida) - 1);
		v->dataSaida[sizeof(v->dataSaida) - 1] = '\0';
    }

    // Atualiza hora -- "-" limpa/ Caso contrario atualiza
    if (strcmp(novaHora, "-") == 0) {
        v->horaSaida[0] = '\0';
    }
    else {
        strncpy(v->horaSaida, novaHora, sizeof(v->horaSaida) - 1);
        v->horaSaida[sizeof(v->horaSaida) - 1] = '\0';
    }
	return 1;
}

int alterarEstado(SISTEMA* s, VAGAS* v, EstadoLugar novoEstado) {
    if (!s || !v) return 0;

	// Validação do novo estado
    if (novoEstado < LUGAR_LIVRE || novoEstado > LUGAR_INDISPONIVEL) {
		return 0; //estado invalido
    }

	// Atualiza o estado no mapa
    if (coordenadaValida(&s->parque, v->andar, v->fila, v->lugar)) {
        int filaIdx = colunaChar_Indice(v->fila);
        if (filaIdx >= 0) {
            s->parque.mapa[v->andar][filaIdx][v->lugar] = novoEstado;
        }
    }
	// Atualiza o estado no registo
	v->estado = novoEstado;

	return 1;
}

    //  ===== Funções principais =====  //

void consultarEstacionamento(SISTEMA* s, int numEntrada) {
    if (!s || !s->estacionamentos) { //valida ponteiro sistema e estacionamentos
        fprintf(stderr, "Erro: sistema ou estacionamentos nulo(s). \n");
        return;
    }

    if (numEntrada <= 0) { //valida numero de entrada
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
    printf("Ticket Nº:  %d\n", v->id);               //numero ticket
    printf("Matricula:  %s\n", v->matricula);         //matricula
    printf("Entrada:    %s %s\n", v->dataEntrada, v->horaEntrada); //data e hora entrada
    printf("Saida:      %s %s\n", v->dataSaida[0] ? v->dataSaida : "-", v->horaSaida[0] ? v->horaSaida : "-");  //data e hora saida
    printf("Local:      Piso %d, Fila %c, Lugar %d\n", v->andar + 1, v->fila, v->lugar + 1);             //coordenadas
    printf("Estado:     %s\n", estadoStr); 					 //estado do lugar


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
    
	//Checkup do numero de entrada
    if (numEntrada <= 0) {
		fprintf(stderr, "Erro: Numero de entrada invalido.\n");
        return;
    }

	VAGAS* v = encontrarEstacionamento(s, numEntrada); //procura o estacionamento
    if (!v) {
        fprintf(stderr, "Erro: Estacionamento com ID %d nao encontrado.\n", numEntrada);
		return;
    }

    //Menu
    printf("\n===== ALTERAR ESTACIONAMENTO (Ticket N. %d) =====\n", v->id);
    printf("1) Matricula\n");
    printf("2) Mover lugar (piso/fila/lugar)\n");
    printf("3) Data/Hora de entrada\n");
    printf("4) Data/Hora de saida\n");
    printf("5) Estado (0=Livre, 1=Ocupado, 2=Indisponivel)\n");
    printf("0) Cancelar\n");
    printf("Opcao: ");

    int opcao = -1;
    if (scanf("%d", &opcao) != 1) {
        fprintf(stderr, "Entrada invalida.\n");
		limparBuffer();
        return;
    }
	limparBuffer();

    if (opcao == 0) {
        printf("Operacao cancelada.\n");
        return;
    }

    switch (opcao) {
    case 1: {
		//alterar matricula
		char novaMatricula[16] = { 0 }; //buffer matricula
        printf("Nova matricula: ");
        if (scanf("%15s", novaMatricula) != 1) {
            fprintf(stderr, "Erro ao ler matricula.\n");
			limparBuffer();
			return;
        }
		limparBuffer();

        if (alterarMatricula(v, novaMatricula)) {
            printf("Matricula alterada com sucesso.\n");
        }
        else {
			fprintf(stderr, "Falha ao alterar matricula.\n");
        }
        break;
    }

    case 2: {
        //mover lugar
        int novoPiso;
        char novaFilaChar;
        int novoLugar;

        printf("Novo piso (1 a %d): ", s->parque.pisos); //limite de andares
        if (scanf("%d", &novoPiso) != 1) { //valida andar
            fprintf(stderr, "Entrada invalida para andar.\n");
			limparBuffer();
            return;
        }
		limparBuffer();
		novoPiso--; //ajusta para indice 0-based

        printf("Nova fila (A a %c): ", (char)('A' + s->parque.filasPorPiso - 1)); //limite de filas
        if(scanf(" %c", &novaFilaChar) != 1){ //valida fila
            fprintf(stderr, "Entrada invalida para fila.\n");
			limparBuffer();
            return;
		}
		limparBuffer();

        

        printf("Novo lugar (1 a %d): ", s->parque.lugaresPorFila); //limite de lugares por fila
        if (scanf("%d", &novoLugar) != 1) { //valida lugar
            fprintf(stderr, "Entrada invalida para lugar.\n");
			limparBuffer();
            return;
        }
		limparBuffer();
		novoLugar--; //ajusta para indice 0-based

        if (moverLugar(s, v, novoPiso, novaFilaChar, novoLugar)) {
            printf("Lugar atualizado: Piso %d, Fila %c, Lugar %d\n",
                v->andar + 1, v->fila, v->lugar + 1);
        } else{
            fprintf(stderr, "Falha ao mover lugar. Verifique se o lugar esta livre e as coordenadas sao validas.\n");
		}
        break;
	}

    case 3: { //alterar data/hora de entrada
		char novaData[12] = { 0 }; //buffer data
		char novaHora[8] = { 0 }; //buffer hora

        printf("Nova dara de entrada (dd/mm/aaaa):\n");
        if (scanf("%10s", novaData) != 1) {
			fprintf(stderr, "Erro ao ler data.\n");
			limparBuffer();
			return;
        }
		limparBuffer();

        printf("Nova hora de entrada (HH:MM):\n");
        if (scanf("%5s", novaHora) != 1) {
            fprintf(stderr, "Erro ao ler hora.\n");
            limparBuffer();
            return;
        }
		limparBuffer();

        if (alterarDataHoraEntrada(v, novaData, novaHora)) {
			printf("Data/Hora de entrada atualizadas.\n");
        }
        else {
			fprintf(stderr, "Falha ao alterar data/hora de entrada.\n");
        }
		break;
    }

    case 4: { //alterar data/hora de saida
        char novaData[12] = { 0 };
        char novaHora[8] = { 0 };

        printf("Nova data de saida (dd/mm/aaaa, '-' para limpar): ");
        if (scanf("%10s", novaData) != 1) {
            fprintf(stderr, "Erro ao ler data.\n");
            limparBuffer();
            return;
        }
        limparBuffer();

        printf("Nova hora de saida (HH:MM, '-' para limpar): ");
        if (scanf("%5s", novaHora) != 1) {
            fprintf(stderr, "Erro ao ler hora.\n");
            limparBuffer();
            return;
        }
        limparBuffer();

        if (alterarDataHoraSaida(v, novaData, novaHora)) {
            printf("Data/Hora de saida atualizadas.\n");
        }
        else {
            fprintf(stderr, "Falha ao atualizar data/hora de saida.\n");
        }
        break;
    }

    case 5: { //alterar estado
        int novoEstado;
        printf("Novo estado (0=Livre, 1=Ocupado, 2=Indisponivel): ");
        if (scanf("%d", &novoEstado) != 1) {
            fprintf(stderr, "Entrada invalida.\n");
            limparBuffer();
            return;
        }
        limparBuffer();

        if (alterarEstado(s, v, (EstadoLugar)novoEstado)) {
            printf("Estado atualizado.\n");
        }
        else {
            fprintf(stderr, "Falha ao atualizar estado (valor invalido).\n");
        }
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

void eliminarEstacionamento(SISTEMA* s, int numEntrada) {
    if (!s || !s->estacionamentos) {
		fprintf(stderr, "Erro: sistema e/ou estacionamentos nulo(s).\n");
		return;
    }

    if (numEntrada <= 0) {
        fprintf(stderr, "Erro: Numero de entrada invalido.\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < s->totalEstacionamentos; i++) {
        if (s->estacionamentos[i].id == numEntrada) {
            idx = i;
			break;
        }
    }

    if (idx < 0) {
		fprintf(stderr, "Erro: Estacionamento com ID %d não encontrado.\n", numEntrada);
		return;
    }

	VAGAS* v = &s->estacionamentos[idx];

    if (coordenadaValida(&s->parque, v->andar, v->fila, v->lugar)) {
        int filaIdx = colunaChar_Indice(v->fila);
        if (filaIdx >= 0 && s->parque.mapa[v->andar][filaIdx][v->lugar] == LUGAR_OCUPADO) {
			s->parque.mapa[v->andar][filaIdx][v->lugar] = LUGAR_LIVRE;
        }
    }

    for (int i = idx; i < s->totalEstacionamentos - 1; i++) {
		s->estacionamentos[i] = s->estacionamentos[i + 1];
    }

	memset(&s->estacionamentos[s->totalEstacionamentos - 1], 0, sizeof(VAGAS));

    s->totalEstacionamentos--;
    
	printf("Estacionamento(Ticket N. %d) eliminado com sucesso.\n", numEntrada);

    if(!guardarBinario(s)) {
        fprintf(stderr, "Aviso: Falha ao guardar em binario.\n");
	}
}


//====================================================
//  Saída de veículos
//====================================================

    //  ===== Ferramentas de suporte =====  //

int parseData(const char* dataStr, int* dia, int* mes, int* ano) {
    if (!dataStr || !dia || !mes || !ano) return 0;

    if (sscanf(dataStr, "%d/%d/%d", dia, mes, ano) != 3) return 0;

    if (*dia < 1 || *dia>31 || *mes < 1 || *mes>12 || *ano < 2000 || *ano > 2100) return 0;

    return 1;
}

int parseHora(const char* horaStr, int* hora, int* minuto) {
    if (!horaStr || !hora || !minuto) return 0;

    if (sscanf(horaStr, "%d:%d", hora, minuto) != 2) return 0;

    if (*hora < 0 || *hora > 23 || *minuto < 0 || *minuto > 59) return 0;

    return 1;
}

void arredondar15Min(int* hora, int* minuto, int arredondarParaCima) {
    if (!hora || !minuto) return;
    if (arredondarParaCima) {
        if (*minuto % 15 != 0) {    // Arredonda para o prox mult de 15
            *minuto = ((*minuto / 15) + 1) * 15;
            if (*minuto >= 60) {
                *minuto = 0;
                (*hora)++;
                if (*hora >= 24) {
                    *hora = 0;  // pula para o próximo dia (não lida com data aqui)
                }
            }
        }
    }
    else {
        if (*minuto % 15 != 0) {
            *minuto = ((*minuto / 15) + 1) * 15;
            if (*minuto >= 60) {
                *minuto = 0;
                (*hora)++;
            }
        }
    }
}

int calcularMinutosDiferenca(int diaE, int horaE, int minE, int diaS, int horaS, int minS) {    // *S - saida, *E - entrada
    int diasDif = diaS - diaE;
    int minutosTotais = diasDif * 24 * 60;  // Conversão: dia -> min
    minutosTotais += (horaS - horaE) * 60;  // Soma diff horas -> min
    minutosTotais += (minS - minE);         // Soma diff min

    return minutosTotais;
}

int contarMudancasDia(int diaE, int diaS) {
    int dias = diaS - diaE;
    return (dias >= 0) ? dias : 0;
}

void mostrarTicketSaida(const VAGAS* v, float valorPagar) {
    if (!v) return;
    printf("\n=========== TICKET SAIDA ===========\n");
    printf("Ticket Nº:     %d\n", v->id);
    printf("Matricula:     %s\n", v->matricula);
    printf("Entrada:       %s %s\n", v->dataEntrada, v->horaEntrada);
    printf("Saida:         %s %s\n", v->dataSaida, v->horaSaida);
    printf("Local:         Piso %d, Fila %c, Lugar %d\n",
        v->andar + 1, v->fila, v->lugar + 1);
    printf("------------------------------------\n");
    printf("VALOR A PAGAR: %.2f EUR\n", valorPagar);
    printf("====================================\n");
}

//  ===== Funções principais =====  //

float calcularValorPagar(SISTEMA* s, VAGAS* v) {
    if (!s || !v) return 0.0f;

    // parse entrada
    int diaE, mesE, anoE, horaE, minE;
    if (!parseData(v->dataEntrada, &diaE, &mesE, &anoE)) {
        fprintf(stderr, "Erro: Data de entrada inválida.\n");
        return 0.0f;
    }
    if (!parseHora(v->horaEntrada, &horaE, &minE)) {
        fprintf(stderr, "Erro: Hora de entrada inválida.\n");
        return 0.0f;
    }

    // parse saída
    int diaS, mesS, anoS, horaS, minS;
    if (!parseData(v->dataSaida, &diaS, &mesS, &anoS)) {
        fprintf(stderr, "Erro: Data de saída inválida.\n");
        return 0.0f;
    }
    if (!parseHora(v->horaSaida, &horaS, &minS)) {
        fprintf(stderr, "Erro: Hora de saída inválida.\n");
        return 0.0f;
    }

    arredondar15Min(&horaE, &minE, 0); // Entrada> prox multiplo
    arredondar15Min(&horaS, &minS, 1); // Saída> prox multiplo

    //Calcular diferença
    int totalMinutos = calcularMinutosDiferenca(diaE, horaE, minE, diaS, horaS, minS);
    int mudancasDia = contarMudancasDia(diaE, diaS);

    float T1 = 0, T2 = 0, T3 = 0, T4 = 0; // Tarifas zeradas para extrair do .txt
    int check = 0;

    for (int i = 0; i < s->totalTarifas; i++) {
        if (strcmp(s->tarifas[i].codigo, "CT1") == 0) {
            T1 = s->tarifas[i].valor; check++;
        }
        if (strcmp(s->tarifas[i].codigo, "CT2") == 0) {
            T2 = s->tarifas[i].valor; check++;
        }
        if (strcmp(s->tarifas[i].codigo, "CT3") == 0) {
            T3 = s->tarifas[i].valor; check++;
        }
        if (strcmp(s->tarifas[i].codigo, "CT4") == 0) {
            T4 = s->tarifas[i].valor; check++;
        }
    }
    if (check < 4) {
        fprintf(stderr, "Erro: Tarifas incompletas no ficheiro!\n");
        return 0.0f;
    }

    //  Calculo T4
    if (mudancasDia >= 2) {
        int totalDias = mudancasDia + 1; //Dias completos + imcompleto
        return totalDias * T4;
    }

    //  Calculo T3
    float totalHoras = totalMinutos / 60.0f;
    float valorHorario = totalHoras * T1; // Assume diurno default

    if (valorHorario > T3 && mudancasDia == 0) {
        return T3; // Limite T3
    }

    float valor = (totalMinutos / 60.0f) * T1; // Valor inicial
    return(valor > T3 && mudancasDia == 1) ? T3 : valor;
}

int registarSaidaVeiculo(SISTEMA* s, int numEntrada) {
    if (!s || !s->estacionamentos) {
        fprintf(stderr, "Erro: Sistema nulo.\n");
        return 0;
    }

    if (numEntrada <= 0) {
        fprintf(stderr, "Erro: Numero de entrada invalido");
        return -1;
    }

    // Pesquisa do registo
    VAGAS* v = encontrarEstacionamento(s, numEntrada);
    if (!v) {
        fprintf(stderr, "Erro: Veiculo com ticket %d nao encontrado.\n", numEntrada);
        return -1;
    }


    if (v->dataSaida[0] != '\0') {
        fprintf(stderr, "Erro: Veiculo com ticket %d ja possui saida registada.\n", numEntrada);
        return -1;
    }

    // Pede data e hora de saída
    printf("Insira:\nData de saida (dd/mm/aaaa): \n");
    if (scanf("%10s", v->dataSaida) != 1) {
        fprintf(stderr, "Erro ao ler data de saida.\n");
        limparBuffer();
        return -1;
    }
    limparBuffer();

    printf("Hora de saida (HH:MM): \n");
    if (scanf("%5s", v->horaSaida) != 1) {
        fprintf(stderr, "Erro ao ler hora de saida.\n");
        limparBuffer();
        return -1;
    }
    limparBuffer();

    //Calculo valor a pagar
    float valorPagar = calcularValorPagar(s, v);

    //Limpa o lugar no mapa
    if (coordenadaValida(&s->parque, v->andar, v->fila, v->lugar)) {
        int filaIdx = colunaChar_Indice(v->fila);
        if (filaIdx >= 0) {
            s->parque.mapa[v->andar][filaIdx][v->lugar] = LUGAR_LIVRE;
        }
    }

    //Atualiza estado
    v->estado = LUGAR_LIVRE;

    //Mostra o ticket
    mostrarTicketSaida(v, valorPagar);

    if (!guardarBinario(s)) {
        fprintf(stderr, "Aviso: Nao foi possivel guardar os dados!\n");
    }
    return 0;
}


//====================================================
// Mapa do piso
//====================================================
void mostrarMapaPiso(SISTEMA* s,int piso) {
    if (!s) {
        fprintf(stderr, "Erro: Sistema nulo.\n");
        return;
    }
    if(piso < 0 || piso >= s->parque.pisos) {
        fprintf(stderr, "Erro: Piso invalido! (1 a %d)\n", s->parque.pisos);
        return;
	}

    printf("\n========== MAPA DO PISO %d ==========\n", piso + 1);

	printf("    ");
    for (int l = 0; l < s->parque.lugaresPorFila; l++) {
		printf("%3d", l + 1);
    }
	printf("\n");

    for (int f = 0; f < s->parque.filasPorPiso; f++) {
        printf(" %c ", 'A' + f);
        for (int l = 0; l < s->parque.lugaresPorFila; l++) {
			EstadoLugar estado = s->parque.mapa[piso][f][l];
            char c;
            switch (estado) {
			case LUGAR_LIVRE:   c = '-'; break;
			case LUGAR_OCUPADO: c = 'X'; break;
			case LUGAR_INDISPONIVEL: c = '#'; break;
			default: c = '?'; break;
            }
			printf(" %c ", c);
        }
		printf("\n");
    }
    printf("\nLegenda: - = Livre | X = Ocupado | # = Indisponivel\n");
    printf("==========================================\n");
}


//====================================================
// Listagens e paginação
//====================================================
void listarEstacionamentos(SISTEMA *s) {
    if (!s || !s->estacionamentos) {
        fprintf(stderr, "Erro: Sistema nulo.\n");
		return;
    }

    if (s->totalEstacionamentos == 0) {
        printf("Nenhum estacionamento registado.\n");
        return;
    }
    printf("\n============ LISTA DE ESTACIONAMENTOS ============\n");
    printf("%-5s %-12s %-12s %-8s %-10s\n",
        "ID", "Matricula", "Entrada", "Local", "Estado");
    printf("=================================================== \n");

    for (int i = 0; i < s->totalEstacionamentos; i++) {
		VAGAS* v = &s->estacionamentos[i];

        const char* estado;
        switch (v->estado) {
        case LUGAR_LIVRE: estado = "Livre"; break;
        case LUGAR_OCUPADO: estado = "Ocupado"; break;
        case LUGAR_INDISPONIVEL: estado = "Indisp."; break;
        default: estado = "?"; break;
        }

        printf("%-5d %-12s %-12s %d%c%-2d     %-10s\n",
            v->id,
            v->matricula,
            v->dataEntrada,
            v->andar + 1,
            v->fila,
            v->lugar + 1,
            estado);
    }
	printf("===================================================\n");
	printf("Total: %d registos\n", s->totalEstacionamentos);
}

/*void avancarPagina() {

}

void recuarPagina() {

}

void mostrarPagina(int pagina) {

}

void gravarListagemTXT() {

}


//====================================================
// Persistência
//====================================================

void guardarDadosEmBinario() {

}

void gravarEstacionamentosTexto() {

}

void gravarErros() {

}


//====================================================
// Lugares indisponíveis
//====================================================
void marcarLugarIndisponivel() {

}

void reverterLugarIndisponivel() {

}


//====================================================
// Funcionalidades adicionais obrigatórias (3 à escolha)
//====================================================
void funcionalidadeExtra1() {

}

void funcionalidadeExtra2() {

}

void funcionalidadeExtra3() {

}


//====================================================
// Extras opcionais (E1, E2, E3)
//====================================================
void gerarGrafico() {

}

void gerarTabelaDinamica() {

}

void gerarCSV() {

}
*/