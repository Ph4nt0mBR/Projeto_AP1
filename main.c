#include <windows.h>
#include<stdio.h>
#include <locale.h>  
int main()  
{ 
setlocale(LC_ALL, "Portuguese"); 
	
	int mes;
	int dia;
	int diasres;

	printf("Escolha um mês: 1-Janeiro, 2-Fevereiro, 3-Março, 4-Abril, 5-Maio, 6-Junho, 7-Julho, 8-Agosto, 9-Setembro, 10-Outubro, 11-Novembro, 12-Dezembro");
	scanf("%d", &mes);
	
	if (mes == 4) {
		printf("Qual o dia atual do mês?");
		scanf("%d", &dia);
		if (dia <30 ) {
			diasres = (30 - dia);
			printf("Faltam %.2lf\n dias para o mês acabar", diasres);
		}
	}

	
	
return 0;  
} 
