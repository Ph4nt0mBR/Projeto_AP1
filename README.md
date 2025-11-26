# Projeto-AP
LOCK TF IN

RESUMO TRABALHO:

- O objetivo do programa é gerir um parque de estacionamento.
- Tamanho do parque modular / customizavel
- x = numero de filas - máximo de 26 filas - Definido pelo utilizador
- y = numero de lugares por fila - maximo de 50 lugares por fila - Definido pelo utilizador (1 a 50)
- z = numedo de andares - máximo de 5 - Definido pelo utilizador 

- Extra: Em caso de indisponibilidade em uma região especifica, o programa podce marcar os lugares da região como não disponiveis/ocupados, podendo ser revertida

- O estacionamento é pago. Preço tarifário abaixo:
- 8:00 - 21:59 - T1 Euro/hora (valor calculado em períodos de 15 minutos)
- 22:00 - 7:59 - T2 Euro/hora (valor calculado em períodos de 15 minutos)
- Dia completo (24h) - T3 Euro/dia (O periodo vale de 00:00 a 23:59, após passar para o próximo dia, o valor é cobrado novamente)
- Vários dias - T4 Euro/dia, sendo considerado o número de dias que esteve estacionado, independente do horário de entrada e saída

Observações do professor:

1. Para os tarifários das alíneas a) e b) o valor a pagar é calculado em períodos de 15 min, ou seja: 1) se a viatura entrar às 8:10, é considerado, 
para efeitos de cálculo, que entrou às 8:15. Quanto à hora de saída, é considerado o período de 15 min seguinte, ou seja, se a viatura sair às 9:55, 
considera-se para efeitos de cálculo do valor a pagar, que a saída ocorreu às 10H00;

2. Se o valor a pagar ultrapassar o valor do dia completo, mas o período do estacionamento não tiver mudado de dia, 
deve ser considerado o valor de um dia completo (aplicando-se a tarifa da alínea c);

3. Se a viatura permanecer por um período de vários dias, o número de dias a considerar deve ser a soma dos dias completos e incompletos, 
só sendo considerada esta tarifa se o número de passagens de dia forem >=2;

4. Considere que T1=0.6€; T2=0.3€; T3=8€ e T4=6€;

5. Os valores referidos no item anterior devem poder ser facilmente alterados, no futuro.

6. Os veículos estão em estacionamentos.txt
	- Obs. Atenção que algumas das linhas do ficheiro (correspondentes a registos de estacionamento), 	
	ainda não têm a hora de saída, pelo que se considerará, para esses casos, que os veículos ainda estão no parque e, 
	assim, devem ser considerados em termos da ocupação do parque de estacionamento;

7. O valor das tarifas estão em tarifas.txt

	----------------
	FUNCIONALIDADES:
	----------------

1.Primeira execução {

	[] O usuário vai definir as dimensões do estacionamento (X, Y, Z)

	[] Ler os ficheiros tarifas.txt e estacionamentos.txt

	[] Certificar de que outros dados importantes para a execução do programa estão guardados na memória (ex: numeração dos registros de estacionamento, etc)
}

2.Segunda execução e seguintes {

	[] Ler os ficheiros préviamente lidos mas em uma estrutura de dados conveniente (Possivelmente será utilizado uma .bin para guardar os dados em binário)

}

3.Estrutura de Menu {

	- Estruturas do menu{

		[] Entrada veículo

		[] Consultar ocupações - Pesquisa feita a partir do númedo do ticket de entrada

		[] Saída veículo + Cobrança - Pesquisa feita a partir do númedo do ticket de entrada

		[] 
		}

	[] A informação de lugares disponíveis por piso deve estar sempre visível

}

4.Registros {

	[] Registro de entrada {
		
		[] Saída no ecrã semelhante a um "Ticket de estacionamento"

		[] Apresentar toda a informação disponível no momento (Horário de entrada, Coordenadas x, y, z da vaga ocupada)

		[] Alterar estado da vaga de disponível para ocupado - Também altera a apresentação no Menu
	}

	[] Registro de saída {
	
		[] Mesmas condições da entrada 
	}
}

Por hora trabalhar com isso.
