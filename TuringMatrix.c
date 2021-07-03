#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Cada estado é representado pelo tipo estado (struct estados_t)
typedef struct estados_t{
	int count;//variável inteira que itera os vetores abaixo, também é responsável a relação entre eles.
	char leitura[10];//qual caractere da fita é lido na transição.
	char reescrita[10];//qual caractere é reescrito sobre o que é lido na transição.
	char direcao[10];//para qual direção se move a "cabeça do leitor", no caso desse programa é incremento ou decremento no index da cadeia.
	int destino[10];//em qual estado a máquina se encontrará após a transição.
}estado;

//As características gerais do automato são gravados no tipo dadosTuring(struct dados)
typedef struct dados{
	int	qtdEstados, qtdTerminais, qtdEstendidos, aceitacao;//respectivamente quantidade total de estados, quantidade de símbolos terminais, quantidades de símbolos do alfabeto estendido e estado de aceitação.

	char *terminais, *estendidos;//String que contém todos os símbolos terminais e String que contém todos os símbolos do alfabeto estendido.
}dadosTuring;

int** preencheMatrizEstatal(dadosTuring dados);//Função que gera a matriz que relaciona 2 estados.

void leTransicoes(dadosTuring dados,estado *total, int **MatrizTransicao);//Função que lê um número x de transições conforme o padrão q x q0 y D, onde q, q0 ? Q, x, y ? S' e D ? {R, L, S}.

void lerCadeias(dadosTuring dados, int **MatrizTrancisao, estado *total);//Função que lê um número x de cadeias e chama a função de teste, a partir do retorno da teste, escreve o resultado "aceita" ou "rejeita".

int testaCadeia(char *cadeia, dadosTuring dados, int **MatrizTrancisao, estado *total);//Função responsável por avaliar a cadeia, retorna 1 caso a função seja aceita pelo automato e 0 caso contrário.

int main(){
	int i, j, **MatrizTransicao;//i e j são iteradores, **MatrizFunção é a matriz cujo par (x,y) identifica a existência de caminhos entre o estado Qx e Qy (posicao [i][j] == 1).
	estado *total;// *total é um vetor do tipo estado que guarda todos o n estados do automato (struct estados_t)
	dadosTuring dados;
	
	
	scanf("%d", &dados.qtdEstados);
	total = (estado*)malloc(dados.qtdEstados*sizeof(estado));//alocação dinâmica do vetor de estados.
	
	MatrizTransicao = preencheMatrizEstatal(dados);//linha 82.
	
	for(i = 0; i < dados.qtdEstados; i++)//esse laço inicializa o contador de controle dos estados como 0, note que cada estado tem um contador próprio
		total[i].count = 0;
		
	scanf("%d", &dados.qtdTerminais);
	dados.terminais = (char *)malloc(dados.qtdTerminais*(sizeof(char)));//44-48 aloca dinamicamente o vetor de símbolos terminais e os lê um por um
	for(i = 0; i < dados.qtdTerminais; i++){
		scanf(" %c", &dados.terminais[i]);

	}
	
	scanf("%d", &dados.qtdEstendidos);
	dados.estendidos = (char *)malloc(dados.qtdEstendidos * (sizeof(char)));//50-55 aloca dinamicamente o vetor de símbolos do alfabeto estendido e os lê um por um
	for(i = 0; i < dados.qtdEstendidos; i++){
		scanf(" %c", &dados.estendidos[i]);

	}

	scanf("%d", &dados.aceitacao);

	leTransicoes(dados, total, MatrizTransicao);//linha 93
	/*for(i = 0; i < dados.qtdEstados; i ++){      Optei por manter esse trecho comentado, pois foi usado no debug e é útil.
		printf("Estado Q%d\n", i);					Esse trecho imprime os dados coletados na função acima facilitando a visualização da correlação
		for(j = 0; j < total[i].count; j++){		dos vetores dentro do estado[i] (qi).
			printf(" %c ",total[i].leitura[j]);		Lê se: Símbolo terminal lido		 |
		}												   Símbolo a ser resscrito		|
		printf("\n");									   Estado que a máquina entrará |
		for(j = 0; j < total[i].count; j++){
			printf(" %c ",total[i].reescrita[j]);
		}
		printf("\n");
		for(j = 0; j < total[i].count; j++){
			printf(" %d ",total[i].destino[j]);
		}
		printf("\n");
	}*/
	lerCadeias(dados, MatrizTransicao, total);//linha 117
	return 0;
	
	free(MatrizTransicao);
	free(total);
}

int** preencheMatrizEstatal(dadosTuring dados){
	int i,j, **MatrizTransicao;
	/*Essa função aloca dinamicamente uma matriz de quantidade max de estados² elementos, calloc foi utilizado para inicializar uma matriz de 0s*/
	MatrizTransicao = (int**)malloc(dados.qtdEstados*sizeof(int*));
	for(i = 0; i < dados.qtdEstados; i++){
		MatrizTransicao[i] = (int*)calloc(dados.qtdEstados,sizeof(int));
	}
	
	return MatrizTransicao;//retorna a matriz gerada para a matriz declarada na main.
}

void leTransicoes(dadosTuring dados,estado *total, int **MatrizTransicao){
	int transicoes, i, source, dest;//transicoes é a quatidade de transições a serem lidas, i é um iterador, linha 100, linha 103
	char read, write, dir;//linha 102, linha 104, linha 105
	
	scanf("%d", &transicoes);
	
	for(i = 0; i < transicoes; i++){
	
		scanf("%d", &source);//estado atual da máquina
		scanf(" %c", &read);//caractere atual lido da fita
		scanf("%d", &dest);//estado que esse caractere leva
		scanf(" %c", &write);//caractere a ser reescrito antes mover na fita
		scanf(" %c", &dir);//direção a mover na fita
	
		MatrizTransicao[source][dest] = 1;//identifica a existência de um caminho no par [source][dest]
		total[source].leitura[total[source].count] = read;				//Aqui começa a correlaçõa, lendo "read", devo reescrever "write" e mover
		total[source].reescrita[total[source].count] = write;			//para "dir" na fita e entrar no estado "dest". Com o uso do mesmo iterador
		total[source].destino[total[source].count] = dest;				//nos 3 vetores diferentes têm-se a relação de dados diferentes no mesmo
		total[source].direcao[total[source].count] = dir;				//índice.
		total[source].count++;											//Count só incrementado após as inserções e é atrelado a cada estado. 
		
	}
}

void lerCadeias(dadosTuring dados, int **MatrizTransicao, estado *total){
	int qtdEntradas, i, *result;//qtdEntradas é a quantidade de entradas, i iterador, *result vetor de resultados da avaliação da cadeia.
	char cadeia[20];//String que representa a cadeia de até 20 elementos.
	scanf("%d", &qtdEntradas);
	
	result = (int *)malloc(qtdEntradas*sizeof(int));
	for(i = 0; i < qtdEntradas; i++){
		scanf("%s", cadeia);
		result[i] = testaCadeia(cadeia, dados, MatrizTransicao, total);//linha 138
	}//o retorno da testa cadeia é carregada no vetor de resultados
	
	for(i = 0; i < qtdEntradas; i++){
		if(result[i] == 1)//para os casos positivos
			printf("aceita\n");
		else//para os negativos
			printf("rejeita\n");
	}
	
	free(result);
}

int testaCadeia(char *cadeia, dadosTuring dados, int **MatrizTransicao, estado *total){
	char currChar;//caractere atual na fita
	int currState = 0, i, j, currCharIndex = 0;//currState é o estado atual da máquina (iniciado em q0 por padrão), i e j iteradores, currCharIndex é o indíce na fita para atualizar currChar;
	
	currChar = *cadeia;//o mesmo que fazer currCharr = cadeia[0], inicia currChar para o caractere mais a esquerda da fita
	while(currState != dados.aceitacao){	//o loop é executado até atingir o estado final (a partir do qual não há transições) ou até que uma transição n seja possível(explicado posteriormente)
		for(j = 0; j < total[currState].count; j++){ //dentro do vetor de estados na posição do estado atual busca-se valor do contador para evitar acessos desnecessários de memória.
			if(total[currState].leitura[j] == currChar){//dentro do vetor de estados, busca-se os caracteres lidos pelo estado e os compara com currChar, caso esse loop chege ao final sem que esse if seja satisfeito j não passará a verifivação da linha 174.
				cadeia[currCharIndex] = total[currState].reescrita[j];//a posição atual na cadeia é reescrita com o elemento do vetor de símbolos estendidos de mesmo índice encontrado no laço e aceito no if.
				
				if(total[currState].direcao[j] == 'R'){//caso a direção seja direita o currCharIndex é incrementado uma unidade
					currCharIndex++;
					
						if(currCharIndex >= strlen(cadeia))//para garantir que a fita é "infinita", quando o index extrapola os limites do vetor cadeia, currChar é atualizado para 'B', que é o símbolo que representa o Branco 
								currChar = 'B';
						else//caso o índice esteja dentro do vetor
							currChar = cadeia[currCharIndex];//currChar recebe o caractere na posição currCharIndex da fita.
						
				}else{//caso não vá para direita 'R', pode ir para esquerda 'L' decrementando uma unidade e as intruções seguintes são espelhadas da direita com mudanças no limite do vetor
					if(total[currState].direcao[j] == 'L'){
						currCharIndex--;
						
							if(currCharIndex < 0)
								currChar = 'B';
							else
								currChar = cadeia[currCharIndex];
								
					}//caso não vá nem para direita nem para esquerda quer dizer que não há movimento 'S', logo o currCharIndex não é alterado.
				}
				
				currState = total[currState].destino[j];//feitas as verificações e edições o estado atual é modificado para o destino de mesmo índice j dentro do vetor de estados.
				j = 200;//j é reescrito para 200 (código 200 retornado em uma conexão bem sucedida na web e valor inalcançável por j) caso passe o if para futura verificação.
				break;//feitas todas as movimentações o loop interno é interrompido
			}
		}
		
		if(j != 200)//caso o if não seja executado, j não atinge 200 e o loop externo é interrompido.
			break;
		
	}
	//printf("fita: %s -> q%d\n", cadeia, currState); linha de debug mostra o estado de parada da máquina e o resultado da cadeia após as modificações

	if(currState != dados.aceitacao){//nos casos em que a máquina termina fora do estado de aceitação (caso do automato de cadeias a^2n)
		for(j = 0; j < total[currState].count; j++){	//é feita uma verificação onde busca-se a possibilidade de ler lambda ('-') para  chegar ao 
			if(total[currState].leitura[j] == '-'){		//estado de aceitação.
				currState = total[currState].destino[j];//caso a verificação seja bem sucedida o estado atual é atualizado (tlvz eu deva fzr isso em loop, vai q o último n é o de aceitação)
				break;
			}
		}
	}
	
	if(currState != dados.aceitacao)//por fim, se o estado final do automato for o de aceitação o retorno é 1, senão é 0 -> linha 126
		return 0;
	else
		return 1;
			
}





