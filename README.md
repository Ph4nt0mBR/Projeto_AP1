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

1. Na fase de “instalação” da aplicação, ativável através de um qualquer mecanismo que considerarem mais adequado:
    a) atendendo à topologia do parque de estacionamento, deverá ser especificado qual o número de andares, filas e lugares por fila do parque de estacionamento;
    b) ainda durante a fase de inicialização da aplicação, deverão ser lidos os ficheiros fornecidos:
		a) Tarifas.txt e guardar os seus dados em memória, devendo ser inserido no dado estruturado apropriado (atendendo à informação disponibilizada acima, sobre as tarifas);
		
		b) Estacionamentos.txt, onde cada linha contém os dados correspondentes a um registo de estacionamento, que deve ser inserido no dado estruturado conveniente, já com o 	
		valor pago calculado (devendo, para isso, ir ler o valor já disponível no dado estruturado em memória), se os veículos já tiverem saído do parque de estacionamento;

	  Obs. Atenção que algumas das linhas do ficheiro (correspondentes a registos de estacionamento), ainda não têm a hora de saída, pelo que se considerará, para esses casos, que 
	   os veículos ainda estão no parque e, assim, devem ser considerados em termos da ocupação do parque de estacionamento;
			
	c) como haverá outros dados importantes para o funcionamento do sistema (p. ex., numeração dos registos de estacionamentos e outros), deve ser também levado em consideração 
	   que estes deverão ser guardados em memória.

2. Aquando da utilização “normal” da aplicação, ao arrancar, devem ser lidos os ficheiros de dados gravados na utilização anterior da aplicação, carregando-os para dados simples ou 
   estruturados, utilizando estruturas de dados convenientes (desejavelmente, aquelas que foram usadas na utilização anterior da aplicação).

3. Criar uma estrutura de menus adequada, devendo estar sempre visível, em qualquer menu que seja apresentado ao utilizador, a informação quanto ao número de lugares disponíveis em 
   cada piso.

4.	a) Permitir o registo da entrada de cada veículo, atribuindo o lugar e mostrando-o ao condutor, não devendo ser esquecido que, a cada entrada, deve ser atualizado o estado do 
	parque de estacionamento e mostrar, no ecrã, algo semelhante a um ticket de estacionamento, com toda a informação disponível no momento, relativa ao estacionamento;
	
	b) Deve ser possível também a consulta, eliminação ou alteração de um qualquer registo de estacionamento, especificando-se o respetivo número de entrada (se alterado o 
	estacionamento, deve ser igualmente mostrado o novo “ticket”).

5. Quanto à atribuição do lugar, ele deve ser efetuado, atendendo àquele que estiver mais perto do A01 do piso em que o automobilista tiver entrado ou, utilizando uma qualquer 
outra política, que deve ser explicada e justificada.

6. Registar a saída do veículo, efetuando os cálculos do valor a pagar e atualizações consideradas necessárias, devendo poder efetuar-se a sua alteração ou anulação.

7.	a) Poder especificar lugares não disponíveis, indicando-se e registando-se o motivo (um caracter), que poderá ser: condições inadequadas (i); objeto de obras (o); 
    estar reservado (r); outros motivos (m);
	b) Poder reverter-se esta situação.

8. Mostrar um mapa de ocupação de um piso do parque de estacionamento, em qualquer momento, da forma mais explícita possível: um lugar ocupado deverá ser assinalado por um X, 
um lugar livre por um “-“ e um lugar indisponível, pela letra específica do motivo.

9. Assegurar a persistência dos dados existentes em memória, utilizando para o efeito ficheiros binários, sempre que o utilizador assim o entender e, obrigatoriamente, 
ao sair da aplicação, não devendo esquecer-se a atualização de outros ficheiros que tenham sido igualmente criados e utilizados para guardar outra informação importante e que terá 
sido carregada em memória, aquando do arranque da aplicação (neste caso, poderão ser guardados em ficheiros tipo txt), pois os dados que contêm poderão ser conhecidos com facilidade e, 
eventualmente, alterados, se for necessário.

10. Para as diversas listagens a desenvolver, dada a potencial elevada quantidade de informação a apresentar, deve implementar-se um mecanismo de avanço página a página. Adicionalmente, 
poderá ser implementado, neste caso particular e.g.: a) permitir o retrocesso para a página anterior; b) mostrar uma dada página, a especificar pelo utilizador, c) ou mostrar a última. 
No caso de alguma destas funcionalidade adicionais serem implementadas, haverá lugar a uma sobrevalorização.

Outra funcionalidade que deve ser oferecida é a possibilidade de gravar a listagem em ficheiro de texto .txt com o cabeçalho conveniente, assim podendo, depois, ser lida, facilmente, 
com um simples editor de texto. O nome do ficheiro .txt deve poder ser especificado pelo utilizador.

11. Devem ainda ser criadas mais 3 funcionalidades, à escolha, que devem, incluir pelo menos uma listagem e obviamente, ser descritas de forma clara.