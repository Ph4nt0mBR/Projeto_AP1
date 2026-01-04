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

int main() {
	setlocale(LC_ALL, "Portuguese");
	//system("cls"); // Limpa o ecrã (Windows). Use "clear" para Unix/Linux.

	SISTEMA sistema;
	inicializarSistema(&sistema);

	if (!leituraConstante(&sistema)) {
		fprintf(stderr, "Aviso: ficheiro binario não encontrado ou inválido. Tentando instalação inicial...\n");
		if (!primeiraLeitura(&sistema)) {
			fprintf(stderr, "Erro: falha na instalação inicial. Encerrando programa.\n");
			return 1;
		}
	}
	/*Implementar o resto do codigo aqui*/

	if (!guardarBinario(&sistema)) {
		fprintf(stderr, "Erro: Não foi possivel guardar dados em %s!\n", BIN_PATH);
		return 1;
	}

	printf("Programa encerrado com sucesso.\n");
	return 0;
}
// Se cada um já tiver atualizado seus arquivos com as instrucoes q mandei no grupo,
// este arquivo main.c ja deve estar a funcionar perfeitamente para o estacionamento qualquer duvida so me chamar __Lu

