#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define MOSTRAR_ENTRADA

/* Cada estado é representado pelo tipo estado (struct estados_t)
	- count: Variável inteira que itera os vetores abaixo, 
		também é responsável a relação entre eles.
	- leitura[10]: Qual caractere da fita é lido na transição.
	- reescrita[10]: Qual caractere é reescrito sobre o que é lido na transição.
	- direção[10]: Para qual direção se move a "cabeça do leitor", 
		no caso desse programa é incremento ou decremento no index da cadeia.
	- destino[10]: Em qual estado a máquina se encontrará após a transição.
*/
typedef struct estados_t
{
	int count;
	char leitura[10];
	char reescrita[10];
	char direcao[10];
	int destino[10];
}estado;

/* As características gerais do automato são gravados no tipo 
   dadosTuring(struct dados)
	Inteiros:
		Respectivamente quantidade total de estados, 
		quantidade de símbolos terminais, quantidades de símbolos 
		do alfabeto estendido e estado de aceitação.
	Vetores de Char:
		Respectivamente string que contém todos os símbolos 
		terminais e String que contém todos os símbolos 
		do alfabeto estendido.
*/
typedef struct dados
{
	int	qtdEstados, qtdTerminais, qtdEstendidos, aceitacao;
	char *terminais, *estendidos;
}dadosTuring;


void leTransicoes(dadosTuring dados,estado *total);//Função que lê um número x de transições conforme o padrão q x q0 y D, onde q, q0 ? Q, x, y ? S' e D ? {R, L, S}.

void lerCadeias(dadosTuring dados, estado *total);//Função que lê um número x de cadeias e chama a função de teste, a partir do retorno da teste, escreve o resultado "aceita" ou "rejeita".

int testaCadeia(char *cadeia, dadosTuring dados, estado *total);//Função responsável por avaliar a cadeia, retorna 1 caso a função seja aceita pelo automato e 0 caso contrário.

int main(){
	/*----Variáveis----*/
	/* i é um iterador	*/
	int i;
	/* total é um vetor do tipo estado que guarda todos o n estados do 
	automato (struct estados_t)*/
	estado *total;
	// Representação da máquina de turing
	dadosTuring dados;
	
	
	//Representação da quantidade de dados
	scanf("%d", &dados.qtdEstados);
	//Alocação dinâmica do vetor de estados.
	total = (estado*)malloc(dados.qtdEstados*sizeof(estado));

	//Esse laço inicializa o contador de controle dos estados como 0, note que cada estado tem um contador próprio
	for(i = 0; i < dados.qtdEstados; i++)
	{
		total[i].count = 0;
	}	

	scanf("%d", &dados.qtdTerminais);

	//Aloca dinamicamente o vetor de símbolos terminais e os lê um por um
	dados.terminais = (char *)malloc(dados.qtdTerminais*(sizeof(char)));
	for(i = 0; i < dados.qtdTerminais; i++){
		scanf(" %c", &dados.terminais[i]);

	}

	//Aloca dinamicamente o vetor de símbolos do alfabeto estendido e os lê um por um
	scanf("%d", &dados.qtdEstendidos);
	dados.estendidos = (char *)malloc(dados.qtdEstendidos * (sizeof(char)));
	for(i = 0; i < dados.qtdEstendidos; i++){
		scanf(" %c", &dados.estendidos[i]);
	}

	scanf("%d", &dados.aceitacao);

	leTransicoes(dados, total);
	/*Esse trecho imprime os dados coletados na função acima facilitando a visualização da correlação dos vetores dentro do estado[i] (qi).
	Lê se: Símbolo terminal lido | Símbolo a ser resscrito | Estado que a máquina entrará 
	*/

	#ifdef MOSTRAR_ENTRADA
	for(i = 0; i < dados.qtdEstados; i ++){      
		printf("Estado Q%d\n", i);					
		for(j = 0; j < total[i].count; j++){		
			printf(" %c ",total[i].leitura[j]);		
		}												   
		printf("\n");									   
		for(j = 0; j < total[i].count; j++){
			printf(" %c ",total[i].reescrita[j]);
		}
		printf("\n");
		for(j = 0; j < total[i].count; j++){
			printf(" %d ",total[i].destino[j]);
		}
		printf("\n");
	}
	#endif


	//Leitura das Cadeias;
	lerCadeias(dados, total);
	//Liberação de espaços
	free(dados.terminais);
	free(dados.estendidos);
	return 0;
}

void leTransicoes(dadosTuring dados,estado *total)
{
	//Transicoes é a quatidade de transições a serem lidas, i é um iterador
	int transicoes, i, source, dest;
	char read, write, dir;
	
	scanf("%d", &transicoes);
	
	for(i = 0; i < transicoes; i++)
	{	
		//estado atual da máquina
		scanf("%d", &source);
		//caractere atual lido da fita
		scanf(" %c", &read);
		//estado que esse caractere leva
		scanf("%d", &dest);
		//caractere a ser reescrito antes mover na fita
		scanf(" %c", &write);
		//direção a mover na fita
		scanf(" %c", &dir);
	
		/* Aqui começa a correlação, lendo "read", devo reescrever "write" 
			e mover para "dir" na fita e entrar no estado "dest". 
			Com o uso do mesmo iterador nos 3 vetores diferentes têm-se a 
			relação de dados diferentes no mesmo índice.
			-> Count só incrementado após as inserções e é atrelado a cada estado.
		*/
		total[source].leitura[total[source].count] = read;
		total[source].reescrita[total[source].count] = write;	
		total[source].destino[total[source].count] = dest;				
		total[source].direcao[total[source].count] = dir;				
		total[source].count++;

	}
}

void lerCadeias(dadosTuring dados, estado *total)
{
	/*qtdEntradas é a quantidade de entradas, i iterador, 
	*result vetor de resultados da avaliação da cadeia.
	*/
	int qtdEntradas, i, result[50];

	//String que representa a cadeia de até 20 elementos.
	char cadeia[20];


	scanf("%d", &qtdEntradas);
	
	for(i = 0; i < qtdEntradas; i++)
	{
		scanf("%s", cadeia);
		result[i] =  testaCadeia(cadeia, dados, total);
	}//o retorno da testa cadeia é carregada no vetor de resultados
	
	for(i = 0; i < qtdEntradas; i++)
	{
		if(result[i] == 1) //para os casos positivos
		{
			printf("%d. aceita\n",i+1);
		}
		else //para os negativos
		{
			printf("%d. rejeita\n",i+1);
		}
	}

	
}

int testaCadeia(char *cadeia, dadosTuring dados, estado *total)
{
	// Caractere atual na fita
	char currChar;

	/* currState é o estado atual da máquina (iniciado em q0 por padrão), 
		i e j iteradores, 
		currCharIndex é o indíce na fita para atualizar currChar;
	*/
	int currState = 0, j, currCharIndex = 0;

	//o mesmo que fazer currCharr = cadeia[0], inicia currChar para o caractere mais a esquerda da fita
	currChar = *cadeia;

	/* O loop é executado até atingir o estado final (a partir do qual não há transições) 
	ou até que uma transição n seja possível(explicado posteriormente)
	*/
	while(currState != dados.aceitacao)
	{	
		for(j = 0; j < total[currState].count; j++)
		{
			 /* Dentro do vetor de estados na posição do estado atual,
			   busca-se valor do contador para evitar acessos desnecessários de memória.
			*/
			if(total[currState].leitura[j] == currChar)
			{
				/* Dentro do vetor de estados, busca-se os caracteres lidos pelo estado e os compara com currChar, 
				caso esse loop chege ao final sem que esse if seja satisfeito j não passará a verifivação.
				*/
				/* A posição atual na cadeia é reescrita com o elemento do vetor de símbolos estendidos de 
				   mesmo índice encontrado no laço e aceito no if.
				*/
				cadeia[currCharIndex] = total[currState].reescrita[j];//a posição atual na cadeia é reescrita com o elemento do vetor de símbolos estendidos de mesmo índice encontrado no laço e aceito no if.
				
				if(total[currState].direcao[j] == 'R')
				{	
					//Caso a direção seja direita o currCharIndex é incrementado uma unidade				
					currCharIndex++;

					/*Para garantir que a fita é "infinita", quando o index extrapola os limites do vetor cadeia, 
					currChar é atualizado para 'B', que é o símbolo que representa o Branco 
					*/
					if(currCharIndex >= strlen(cadeia))
					{		
						currChar = 'B';
					}
					else // Caso o índice esteja dentro do vetor
					{
						//currChar recebe o caractere na posição currCharIndex da fita.
						currChar = cadeia[currCharIndex];
					}
				}						
				else
				{
					/*Caso não vá para direita 'R', pode ir para esquerda 'L' decrementando uma unidade e as 
					intruções seguintes são espelhadas da direita com mudanças no limite do vetor
					*/
					if(total[currState].direcao[j] == 'L')
					{
						currCharIndex--;

						/*Para garantir que a fita é "infinita", quando o index extrapola os limites do vetor cadeia (<0), 
						currChar é atualizado para 'B', que é o símbolo que representa o Branco 
						*/
						if(currCharIndex < 0)
						{	
							currChar = 'B';
						}
						else
						{	
							currChar = cadeia[currCharIndex];
						}
								
					}
					
					//Caso não vá nem para direita nem para esquerda quer dizer que não há movimento 'S', logo o currCharIndex não é alterado.
				}
				
				//Feitas as verificações e edições o estado atual é modificado para o destino de mesmo índice j dentro do vetor de estados.
				currState = total[currState].destino[j];
				/*J é reescrito para 200 
				(valor inalcançável por j dentro do programa) 
				caso passe o if para futura verificação.
				*/
				j = 200;

				//Feitas todas as movimentações o loop interno é interrompido
				break;
			}
		}
		
		//Caso o if não seja executado, j não atinge 200 e o loop externo é interrompido.
		if(j != 200)
		{
			break;
		}
		
	}
	#ifdef MOSTRAR_ENTRADA
	//linha de debug mostra o estado de parada da máquina e o resultado da cadeia após as modificações
	printf("fita: %s -> q%d\n", cadeia, currState);
	#endif

	//Mos casos em que a máquina termina fora do estado de aceitação (caso do automato de cadeias a^2n)
	if(currState != dados.aceitacao)
	{
		for(j = 0; j < total[currState].count; j++)
		{	
			//É feita uma verificação onde busca-se a possibilidade de ler lambda ('-') para  chegar a outro estado
			if(total[currState].leitura[j] == '-')
			{
				//Caso a verificação seja bem sucedida o estado atual é atualizado
				currState = total[currState].destino[j];
				break;
			}
		}
	}
	
	/* Por fim, se o estado final do automato for o de aceitação o retorno é 1, 
	senão é 0
	*/
	if(currState != dados.aceitacao)
	{	
		return 0;
	}
	else
	{
		return 1;
	}	
}
