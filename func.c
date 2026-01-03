#include "funcoes.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* ================== UTILITÁRIOS ================== */

*/

// Incluido a estruturas.h, e os outros includes estão lá





#include "funcoes.h" // -- Não precisa incluir stdio.h aqui, já está em estruturas.h [Samuel]
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>


//=======================================================
// Utilitários
//=======================================================

void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int clamp(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static int lerInt(const char *msg, int lo, int hi) {
    int x;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &x) != 1) {
            printf("Valor inválido.\n");
            limparBuffer();
            continue;
        }
        if (x < lo || x > hi) {
            printf("Fora do intervalo [%d..%d].\n", lo, hi);
            continue;
        }
        limparBuffer();
        return x;
    }
}

static void lerString(const char *msg, char *out, int maxlen) {
    printf("%s", msg);
    if (fgets(out, maxlen, stdin) == NULL) {
        out[0] = '\0';
        return;
    }
    out[strcspn(out, "\r\n")] = '\0';
}

static int parseHora(const char *hhmm, int *h, int *m) {
    if (sscanf(hhmm, "%d:%d", h, m) != 2) return 0;
    if (*h < 0 || *h > 23 || *m < 0 || *m > 59) return 0;
    return 1;
}

static double parseValor(const char *s) {
    // aceita "0.60", "0,60", "0.60€"
    char buf[64];
    int j = 0;
    for (int i = 0; s[i] && j < (int)sizeof(buf)-1; i++) {
        if (isdigit((unsigned char)s[i]) || s[i] == '.' || s[i] == ',')
            buf[j++] = (s[i] == ',') ? '.' : s[i];
    }
    buf[j] = '\0';
    return atof(buf);
}

static time_t make_time(int ano, int mes, int dia, int hora, int min) {
    struct tm t;
    memset(&t, 0, sizeof(t));
    t.tm_year = ano - 1900;
    t.tm_mon  = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min  = min;
    t.tm_isdst = -1;
    return mktime(&t);
}

static time_t ceil_to_quarter(time_t t) {
    // arredonda para cima ao próximo múltiplo de 15 min
    long sec = (long)t;
    long q = 15L * 60L;
    long r = sec % q;
    if (r == 0) return t;
    return (time_t)(sec + (q - r));
}

static int day_diff(time_t a, time_t b) {
    // diferença em dias (inteiros) entre datas (considera meia-noite)
    struct tm ta = *localtime(&a);
    struct tm tb = *localtime(&b);
    ta.tm_hour = ta.tm_min = ta.tm_sec = 0;
    tb.tm_hour = tb.tm_min = tb.tm_sec = 0;
    time_t da = mktime(&ta);
    time_t db = mktime(&tb);
    double diff = difftime(db, da);
    return (int)(diff / (24.0 * 3600.0));
}

static int midnights_crossed(time_t a, time_t b) {
    // nº de passagens de dia (nº de meias-noites atravessadas)
    int dd = day_diff(a, b);
    return (dd <= 0) ? 0 : dd;
}

/* ================== ESTRUTURAS / SISTEMA ================== */

void initSistema(Sistema *s) {
    memset(s, 0, sizeof(*s));
    s->parque.pisos = 0;
    s->parque.filas = 0;
    s->parque.lugaresPorFila = 0;
    s->parque.lugares = NULL;

    s->nTarifas = 0;
    s->ests = NULL;
    s->nEsts = 0;
    s->capEsts = 0;
    s->ultimoNumEntrada = 0;
}

void freeSistema(Sistema *s) {
    free(s->parque.lugares);
    s->parque.lugares = NULL;

    free(s->ests);
    s->ests = NULL;
    s->nEsts = s->capEsts = 0;
}

int configurarParque(Sistema *s) {
    int pisos  = lerInt("Nº pisos (1..5): ", 1, MAX_PISOS);
    int filas  = lerInt("Nº filas por piso (1..26): ", 1, MAX_FILAS);
    int lugPF  = lerInt("Nº lugares por fila (1..50): ", 1, MAX_LUGARES);

    free(s->parque.lugares);

    s->parque.pisos = pisos;
    s->parque.filas = filas;
    s->parque.lugaresPorFila = lugPF;

    int total = pisos * filas * lugPF;
    s->parque.lugares = (Lugar*)malloc(sizeof(Lugar) * total);
    if (!s->parque.lugares) {
        printf("Erro: memória insuficiente.\n");
        return 0;
    }

    for (int i = 0; i < total; i++) {
        s->parque.lugares[i].estado = LUGAR_LIVRE;
        s->parque.lugares[i].motivo = '-';
        s->parque.lugares[i].numEntrada = -1;
    }

    return 1;
}

int idxLugar(const Sistema *s, int piso, int fila, int lugar) {
    // piso: 1..pisos, fila: 0..filas-1, lugar: 1..lugPF
    int p = piso - 1;
    int l = lugar - 1;
    int f = fila;
    int lugPF = s->parque.lugaresPorFila;
    int filas = s->parque.filas;
    return (p * filas * lugPF) + (f * lugPF) + l;
}

int contarLivresNoPiso(const Sistema *s, int piso) {
    int livres = 0;
    for (int f = 0; f < s->parque.filas; f++) {
        for (int l = 1; l <= s->parque.lugaresPorFila; l++) {
            int id = idxLugar(s, piso, f, l);
            if (s->parque.lugares[id].estado == LUGAR_LIVRE) livres++;
        }
    }
    return livres;
}

void mostrarDisponiveisPorPiso(const Sistema *s) {
    if (s->parque.pisos <= 0) return;
    printf("\n[Lugares livres por piso] ");
    for (int p = 1; p <= s->parque.pisos; p++) {
        printf("P%d=%d ", p, contarLivresNoPiso(s, p));
    }
    printf("\n");
}

void imprimirMapaPiso(const Sistema *s, int piso) {
    if (piso < 1 || piso > s->parque.pisos) {
        printf("Piso inválido.\n");
        return;
    }
    printf("\nMapa Piso %d\n", piso);
    printf("   ");
    for (int l = 1; l <= s->parque.lugaresPorFila; l++) printf("%02d ", l);
    printf("\n");

    for (int f = 0; f < s->parque.filas; f++) {
        char letra = (char)('A' + f);
        printf("%c: ", letra);
        for (int l = 1; l <= s->parque.lugaresPorFila; l++) {
            int id = idxLugar(s, piso, f, l);
            Lugar lg = s->parque.lugares[id];
            char c = '-';
            if (lg.estado == LUGAR_OCUPADO) c = 'X';
            else if (lg.estado == LUGAR_INDISPONIVEL) c = lg.motivo;
            printf(" %c ", c);
        }
        printf("\n");
    }
}

/* ================== TARIFAS ================== */

int carregarTarifasTxt(Sistema *s, const char *fname) {
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        fprintf(stderr, "Erro ao abrir %s\n", fname);
        return 0;
    }

void inicializarSistema(SISTEMA* s) {
    if (!s) return;

    s->estacionamentos = malloc(sizeof(VAGAS) * MAX_ESTACIONAMENTOS);
    if (!s->estacionamentos) {
        fprintf(stderr, "Erro ao alocar memoria para estacionamentos!\n");
        exit(1);
    }

    s->totalTarifas = 0;
    s->totalEstacionamentos = 0;
    s->ultimoNumEntrada = 0;

        // tenta ler 5 campos: Tp Cod HoraInf HoraSup Valor
        char tpStr[8], cod[16], hInf[16], hSup[16], valStr[64];
        int n = sscanf(linha, "%7s %15s %15s %15s %63s", tpStr, cod, hInf, hSup, valStr);
        if (n < 2) continue;

    for (int a = 0; a < MAX_PISO; a++)
        for (int f = 0; f < MAX_FILA; f++)
            for (int l = 0; l < MAX_LUGARES; l++)
                s->parque.mapa[a][f][l] = LUGAR_LIVRE;
}

        Tarifa t;
        memset(&t, 0, sizeof(t));

        t.tp = tpStr[0];
        strncpy(t.cod, cod, MAX_COD_TARIFA-1);
        t.cod[MAX_COD_TARIFA-1] = '\0';

        if (t.tp == 'H') {
            if (n < 5) {
                fprintf(stderr, "Linha tarifa inválida (H): %s", linha);
                continue;
            }
            if (!parseHora(hInf, &t.hInf, &t.mInf) || !parseHora(hSup, &t.hSup, &t.mSup)) {
                fprintf(stderr, "Horas inválidas: %s", linha);
                continue;
            }
            t.valor = parseValor(valStr);
        } else if (t.tp == 'D') {
            // Para D, horas podem vir "00:00 00:00" mas não interessam
            if (n >= 5) t.valor = parseValor(valStr);
            else {
                fprintf(stderr, "Linha tarifa inválida (D): %s", linha);
                continue;
            }
        } else {
            fprintf(stderr, "Tipo tarifa desconhecido: %s", linha);
            continue;
        }

        if (s->nTarifas < MAX_TARIFAS) {
            s->tarifas[s->nTarifas++] = t;
        }
    }

    fclose(fp);
    return (s->nTarifas > 0);
}

double obterValorTarifa(const Sistema *s, const char *cod) {
    for (int i = 0; i < s->nTarifas; i++) {
        if (strcmp(s->tarifas[i].cod, cod) == 0) return s->tarifas[i].valor;
    }
    return 0.0;
}

static double calcularValorAPagar(const Sistema *s, time_t entrada, time_t saida) {
    // arredondamentos a 15 min (entrada e saída para cima)
    time_t e = ceil_to_quarter(entrada);
    time_t x = ceil_to_quarter(saida);

    if (difftime(x, e) <= 0) return 0.0;

    int passagens = midnights_crossed(e, x);

    double t3 = obterValorTarifa(s, "CT3");
    double t4 = obterValorTarifa(s, "CT4");
    double t1 = obterValorTarifa(s, "CT1");
    double t2 = obterValorTarifa(s, "CT2");

    // Se passagens de dia >=2 -> tarifa vários dias
    if (passagens >= 2) {
        int days = day_diff(e, x) + 1; // dias tocados (completo+incompleto)
        return (double)days * t4;
    }

    // Caso contrário: calcula por quartos de hora usando CT1/CT2
    double total = 0.0;
    time_t cur = e;

    while (cur < x) {
        struct tm tt = *localtime(&cur);
        int hh = tt.tm_hour;

        // Regra simples: CT1 das 08:00..21:59 e CT2 das 22:00..07:59
        double rate = (hh >= 8 && hh <= 21) ? t1 : t2;
        total += rate / 4.0;
        cur += 15 * 60;
    }

    // Se não mudou de dia e ultrapassa T3 -> aplica dia completo
    if (passagens == 0 && t3 > 0.0 && total > t3) total = t3;

    return total;
}

//----------------------------------------------------
// Entrada de veículos
//----------------------------------------------------

int registarEntradaVeiculo(SISTEMA* s) { //Bruno
    if (!s) return -1;

static void garantirCap(Sistema *s) {
    if (s->nEsts < s->capEsts) return;
    int newCap = (s->capEsts == 0) ? 128 : s->capEsts * 2;
    Estacionamento *tmp = (Estacionamento*)realloc(s->ests, sizeof(Estacionamento) * newCap);
    if (!tmp) {
        printf("Erro: memória insuficiente.\n");
        exit(1);
    }
    s->ests = tmp;
    s->capEsts = newCap;
}

    // Usar 'id' em vez de 'numEntrada' (que não existe na struct)
    novo.id = ++s->ultimoNumEntrada;

    printf("Matricula do veiculo: ");
    scanf("%s", novo.matricula);

    // Removido: tipoVeiculo não existe na struct VAGAS
    // printf("Tipo de veiculo (ex: 'Carro', 'Moto', 'Camiao'): ");
    // scanf("%s", novo.tipoVeiculo);
int carregarEstacionamentosTxt(Sistema *s, const char *fname) {
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        fprintf(stderr, "Erro ao abrir %s\n", fname);
        return 0;
    }

    char linha[512];
    while (fgets(linha, sizeof(linha), fp)) {
        if (linha[0] == '\n' || linha[0] == '\r') continue;
        if (linha[0] == '<') continue; // cabeçalho

    if (piso < 0 || piso >= s->parque.pisos) { //validação piso
        printf("Piso inválido!\n");
        return -1;
    }

    // Procurar lugar livre no piso escolhido
    int fila, lugar;
    int encontrou = 0;

    for (fila = 0; fila < s->parque.filasPorPiso && !encontrou; fila++) {
        for (lugar = 0; lugar < s->parque.lugaresPorFila && !encontrou; lugar++) {
            if (s->parque.mapa[piso][fila][lugar] == LUGAR_LIVRE) {
                encontrou = 1;
            }
        }
    }

    if (!encontrou) {
        printf("Não existem lugares disponíveis neste piso.\n");
        return -1;
    }

    //guardar lugar atribuido
    novo.andar = piso;  // Usar 'andar' em vez de 'piso'

    // ATENÇÃO: a struct usa char para fila
    // Converter índice numérico para caractere (A, B, C, ...)
    novo.fila = 'A' + (fila - 1);  // Converte para letra

    novo.lugar = lugar - 1;

    //registar data e hora de entrada
    printf("Data de entrada (dd/mm/aaaa): ");
    scanf("%s", novo.dataEntrada);

    printf("Hora de entrada (HH:MM): ");
    scanf("%s", novo.horaEntrada);

    // Inicializar data e hora de saída como vazias
    novo.dataSaida[0] = '\0';
    novo.horaSaida[0] = '\0';

    //estado inicial do estacionamento
    novo.estado = LUGAR_OCUPADO;

    //atualizar mapa - usar indices numéricos para a matriz
    s->parque.mapa[piso][fila - 1][lugar - 1] = LUGAR_OCUPADO;

    //adicionar a lista de estacionamentos
    s->estacionamentos[s->totalEstacionamentos++] = novo;

    printf("Entrada registada com sucesso! Ticket Nº %d\n", novo.id);
    printf("Lugar: Piso %d, Fila %c, Lugar %d\n", novo.andar, novo.fila, novo.lugar);

    return novo.id;
}

int atribuirLugar(SISTEMA* s, int piso, int* filaOut, int* lugarOut) //Bruno
{
    //valida entradas
    if (s == NULL || filaOut == NULL || lugarOut == NULL)
        return 1;

    while (1) {
        int start = page * pageSize;
        int end = start + pageSize;
        if (end > s->nEsts) end = s->nEsts;

        printf("\n--- Estacionamentos (Página %d/%d) ---\n", page+1, totalPages);
        printf("numE  matricula        entrada              lugar  saida                valor\n");

        for (int i = start; i < end; i++) {
            const Estacionamento *e = &s->ests[i];
            if (e->temSaida) {
                printf("%-5d %-15s %04d-%02d-%02d %02d:%02d  %-5s %04d-%02d-%02d %02d:%02d  %6.2f\n",
                    e->numE, e->matricula,
                    e->anoE,e->mesE,e->diaE,e->horaE,e->minE, e->lugarCod,
                    e->anoS,e->mesS,e->diaS,e->horaS,e->minS, e->valorPago);
            } else {
                printf("%-5d %-15s %04d-%02d-%02d %02d:%02d  %-5s (no parque)         %6s\n",
                    e->numE, e->matricula,
                    e->anoE,e->mesE,e->diaE,e->horaE,e->minE, e->lugarCod,
                    "-");
            }
        }

        printf("\n[n] próxima  [p] anterior  [e] export txt  [q] sair : ");
        char op = 0;
        scanf(" %c", &op);
        limparBuffer();

        if (op == 'n' || op == 'N') {
            if (page < totalPages-1) page++;
        } else if (op == 'p' || op == 'P') {
            if (page > 0) page--;
        } else if (op == 'e' || op == 'E') {
            char fn[128];
            lerString("Nome do ficheiro .txt: ", fn, sizeof(fn));
            if (fn[0]) exportarListagemTxt(s, fn);
        } else if (op == 'q' || op == 'Q') {
            break;
        }
    }
}

/* ================== ATRIBUIÇÃO DE LUGAR ================== */

static int atribuirLugarMaisProximo(const Sistema *s, int pisoEscolhido, int *outP, int *outF, int *outL) {
    // Política simples: varrer do A01 para cima no piso escolhido
    int p = pisoEscolhido;
    for (int f = 0; f < s->parque.filas; f++) {
        for (int l = 1; l <= s->parque.lugaresPorFila; l++) {
            int id = idxLugar(s, p, f, l);
            if (s->parque.lugares[id].estado == LUGAR_LIVRE) {
                *outP = p; *outF = f; *outL = l;
                return 1;
            }
        }
    }
    return 0;
}

static void codLugar(int piso, int fila, int lugar, char *out) {
    sprintf(out, "%d%c%02d", piso, (char)('A'+fila), lugar);
}

/* ================== OPERAÇÕES ================== */

}

//----------------------------------------------------
// Saída de veículos
//----------------------------------------------------
/*
int registarSaidaVeiculo() {

    return 0;
}

void registarEntrada(Sistema *s) {
    if (s->parque.pisos <= 0) {
        printf("Parque não configurado.\n");
        return;
    }

    char mat[16];
    lerString("Matricula: ", mat, sizeof(mat));
    if (!mat[0]) { printf("Matrícula vazia.\n"); return; }

    int piso = lerInt("Piso de entrada: ", 1, s->parque.pisos);


void alterarSaida(int numEntrada) {                                          //Objetivo: Alterar a hora registada de saída de um veículo
    if (numEntrada < 0 || parqueRegisto[numEntrada].ativo == 1) {            //numEntrada → índice/registo do veículo no parque
        printf("Erro: Veiculo ainda no parque ou ID invalido!\n");
        return;
    }

    // Data/hora atual
    time_t now = time(NULL);
    struct tm tt = *localtime(&now);

    Estacionamento e;
    memset(&e, 0, sizeof(e));
    e.numE = ++s->ultimoNumEntrada;
    strncpy(e.matricula, mat, sizeof(e.matricula)-1);

    e.anoE = tt.tm_year + 1900;
    e.mesE = tt.tm_mon + 1;
    e.diaE = tt.tm_mday;
    e.horaE = tt.tm_hour;
    e.minE = tt.tm_min;

    codLugar(p, f, l, e.lugarCod);
    e.temSaida = 0;
    e.valorPago = 0.0;

    garantirCap(s);
    s->ests[s->nEsts++] = e;

    int id = idxLugar(s, p, f, l);
    s->parque.lugares[id].estado = LUGAR_OCUPADO;
    s->parque.lugares[id].motivo = '-';
    s->parque.lugares[id].numEntrada = e.numE;

//----------------------------------------------------
// Consulta, alteração e eliminação de registos
//----------------------------------------------------
/*
void consultarEstacionamento(int numEntrada) {

static Estacionamento* findEstByNum(Sistema *s, int numE) {
    for (int i = 0; i < s->nEsts; i++) {
        if (s->ests[i].numE == numE) return &s->ests[i];
    }
    return NULL;
}

void registarSaida(Sistema *s) {
    int numE = lerInt("Num entrada a fechar: ", 1, 2000000000);
    Estacionamento *e = findEstByNum(s, numE);
    if (!e) { printf("Não encontrado.\n"); return; }
    if (e->temSaida) { printf("Já tem saída registada.\n"); return; }

    // Data/hora atual
    time_t now = time(NULL);
    struct tm tt = *localtime(&now);

    e->anoS = tt.tm_year + 1900;
    e->mesS = tt.tm_mon + 1;
    e->diaS = tt.tm_mday;
    e->horaS = tt.tm_hour;
    e->minS = tt.tm_min;
    e->temSaida = 1;

    time_t te = make_time(e->anoE,e->mesE,e->diaE,e->horaE,e->minE);
    time_t ts = make_time(e->anoS,e->mesS,e->diaS,e->horaS,e->minS);

    e->valorPago = calcularValorAPagar(s, te, ts);

    // Libertar lugar
    int p, f, l;
    if (parseLugarCod(s, e->lugarCod, &p, &f, &l)) {
        int id = idxLugar(s, p, f, l);
        // só liberta se estiver ocupado por este registo
        if (s->parque.lugares[id].estado == LUGAR_OCUPADO &&
            s->parque.lugares[id].numEntrada == e->numE) {
            s->parque.lugares[id].estado = LUGAR_LIVRE;
            s->parque.lugares[id].motivo = '-';
            s->parque.lugares[id].numEntrada = -1;
        }
    }

    imprimirTicket(e);
}

void consultarAlterarEliminar(Sistema *s) {
    int numE = lerInt("Num entrada: ", 1, 2000000000);
    Estacionamento *e = findEstByNum(s, numE);
    if (!e) { printf("Não encontrado.\n"); return; }

    imprimirTicket(e);

//----------------------------------------------------
// Persistência
//----------------------------------------------------

void guardarDadosEmBinario() {

    if (op == 1) {
        char mat[16];
        lerString("Nova matrícula: ", mat, sizeof(mat));
        if (mat[0]) {
            strncpy(e->matricula, mat, sizeof(e->matricula)-1);
            imprimirTicket(e);
        }
    } else if (op == 2) {
        // Se estava no parque, liberta o lugar
        if (!e->temSaida) {
            int p,f,l;
            if (parseLugarCod(s, e->lugarCod, &p, &f, &l)) {
                int id = idxLugar(s, p, f, l);
                if (s->parque.lugares[id].estado == LUGAR_OCUPADO &&
                    s->parque.lugares[id].numEntrada == e->numE) {
                    s->parque.lugares[id].estado = LUGAR_LIVRE;
                    s->parque.lugares[id].motivo = '-';
                    s->parque.lugares[id].numEntrada = -1;
                }
            }
        }

void gravarEstacionamentosTexto() {

}

/* ================== INDISPONIBILIDADES ================== */

void marcarIndisponivel(Sistema *s) {
    int piso = lerInt("Piso: ", 1, s->parque.pisos);
    char filaCh;
    printf("Fila (A..): ");
    scanf(" %c", &filaCh); limparBuffer();
    filaCh = (char)toupper((unsigned char)filaCh);
    int fila = filaCh - 'A';
    int lug = lerInt("Lugar (1..): ", 1, s->parque.lugaresPorFila);

    if (fila < 0 || fila >= s->parque.filas) { printf("Fila inválida.\n"); return; }

    printf("Motivo [i] inadequado, [o] obras, [r] reservado, [m] outros: ");
    char mot;
    scanf(" %c", &mot); limparBuffer();
    mot = (char)tolower((unsigned char)mot);
    if (mot!='i' && mot!='o' && mot!='r' && mot!='m') { printf("Motivo inválido.\n"); return; }

    int id = idxLugar(s, piso, fila, lug);
    if (s->parque.lugares[id].estado == LUGAR_OCUPADO) {
        printf("Lugar está ocupado, não pode marcar indisponível.\n");
        return;
    }
    s->parque.lugares[id].estado = LUGAR_INDISPONIVEL;
    s->parque.lugares[id].motivo = mot;
    s->parque.lugares[id].numEntrada = -1;
    printf("Lugar marcado indisponível.\n");
}

void reverterIndisponivel(Sistema *s) {
    int piso = lerInt("Piso: ", 1, s->parque.pisos);
    char filaCh;
    printf("Fila (A..): ");
    scanf(" %c", &filaCh); limparBuffer();
    filaCh = (char)toupper((unsigned char)filaCh);
    int fila = filaCh - 'A';
    int lug = lerInt("Lugar (1..): ", 1, s->parque.lugaresPorFila);

    if (fila < 0 || fila >= s->parque.filas) { printf("Fila inválida.\n"); return; }

    int id = idxLugar(s, piso, fila, lug);
    if (s->parque.lugares[id].estado != LUGAR_INDISPONIVEL) {
        printf("Lugar não está indisponível.\n");
        return;
    }
    s->parque.lugares[id].estado = LUGAR_LIVRE;
    s->parque.lugares[id].motivo = '-';
    s->parque.lugares[id].numEntrada = -1;
    printf("Lugar revertido para livre.\n");
}

/* ================== BINÁRIO ================== */

int guardarBinario(const Sistema *s, const char *fname) {
    FILE *fp = fopen(fname, "wb");
    if (!fp) return 0;

    // escreve configuração parque
    fwrite(&s->parque.pisos, sizeof(int), 1, fp);
    fwrite(&s->parque.filas, sizeof(int), 1, fp);
    fwrite(&s->parque.lugaresPorFila, sizeof(int), 1, fp);

    int total = s->parque.pisos * s->parque.filas * s->parque.lugaresPorFila;
    fwrite(s->parque.lugares, sizeof(Lugar), total, fp);

    // tarifas
    fwrite(&s->nTarifas, sizeof(int), 1, fp);
    fwrite(s->tarifas, sizeof(Tarifa), s->nTarifas, fp);

    // estacionamentos
    fwrite(&s->ultimoNumEntrada, sizeof(int), 1, fp);
    fwrite(&s->nEsts, sizeof(int), 1, fp);
    fwrite(s->ests, sizeof(Estacionamento), s->nEsts, fp);

    fclose(fp);
    return 1;
}

int carregarBinario(Sistema *s, const char *fname) {
    FILE *fp = fopen(fname, "rb");
    if (!fp) return 0;

    freeSistema(s);

    int pisos, filas, lugPF;
    if (fread(&pisos, sizeof(int), 1, fp) != 1) { fclose(fp); return 0; }
    if (fread(&filas, sizeof(int), 1, fp) != 1) { fclose(fp); return 0; }
    if (fread(&lugPF, sizeof(int), 1, fp) != 1) { fclose(fp); return 0; }

    s->parque.pisos = clamp(pisos, 1, MAX_PISOS);
    s->parque.filas = clamp(filas, 1, MAX_FILAS);
    s->parque.lugaresPorFila = clamp(lugPF, 1, MAX_LUGARES);

    int total = s->parque.pisos * s->parque.filas * s->parque.lugaresPorFila;
    s->parque.lugares = (Lugar*)malloc(sizeof(Lugar) * total);
    if (!s->parque.lugares) { fclose(fp); return 0; }

    if ((int)fread(s->parque.lugares, sizeof(Lugar), total, fp) != total) {
        fclose(fp);
        return 0;
    }

    if (fread(&s->nTarifas, sizeof(int), 1, fp) != 1) { fclose(fp); return 0; }
    if (s->nTarifas < 0 || s->nTarifas > MAX_TARIFAS) { fclose(fp); return 0; }
    if ((int)fread(s->tarifas, sizeof(Tarifa), s->nTarifas, fp) != s->nTarifas) { fclose(fp); return 0; }

    if (fread(&s->ultimoNumEntrada, sizeof(int), 1, fp) != 1) { fclose(fp); return 0; }

    if (fread(&s->nEsts, sizeof(int), 1, fp) != 1) { fclose(fp); return 0; }
    if (s->nEsts < 0) { fclose(fp); return 0; }

    s->capEsts = s->nEsts;
    s->ests = (Estacionamento*)malloc(sizeof(Estacionamento) * s->capEsts);
    if (!s->ests && s->nEsts > 0) { fclose(fp); return 0; }

    if (s->nEsts > 0) {
        if ((int)fread(s->ests, sizeof(Estacionamento), s->nEsts, fp) != s->nEsts) {
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return 1;
}

