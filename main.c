   //Criei o código pra funcionar como o start do programa, qualquer dúvida entrar em contato (main.c é a minha parte do projeto i think) __Luca

/*
  main.c
  Ponto de entrada principal — orquestra inicialização, menu e persistência.

  Nota: Este ficheiro assume que os protótipos em 'funcoes.h' e 'menu.h' 
  estão ajustados para receberem 'SISTEMA* s' quando necessário.
*/

#include <stdio.h>
#include <locale.h>             

#include "menu.h"
#include "funcoes.h"


/* Função principal — inicializa, executa o menu e garante persistência ao sair.  */
int main() {
    setlocale(LC_ALL, "Portuguese");
    // system("clear");  // Para MacOS
    system("cls");  // Para Windows

    SISTEMA sistema;

   
	ResultadoLeitura r = leituraConstante(&sistema);    //Inicia o sistema, verificando se tem o .bin, se n tiver tenta ler os .txt
    if (r != LER_OK) {
        printf(stderr, "Erro ao carregar dados: %d. Realizando instalação inicial...\n", r);
		r = primeiraLeitura(&sistema);
        if (r != LER_OK) {
            fprintf(stderr, "Erro crítico na instalação inicial: %d\n", r);
            return 1;
		}
    }

	//Menu principal
	mostrarMenuPrincipal(&sistema);

	// Ao sair do menu, grava os dados no binário
    if (!guardarBinario(&sistema)) {
        fprintf(stderr, "AVISO: Não foi possivel gravar dados no %s.\n", BIN_PATH);
        return 1;
	}

	printf("Programa encerrado com sucesso.\n");
	return 0;
}

// Se cada um já tiver atualizado seus arquivos com as instrucoes q mandei no grupo, 
// este arquivo main.c ja deve estar a funcionar perfeitamente para o estacionamento qualquer duvida so me chamar __Lu

