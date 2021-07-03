#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Cada estado � representado pelo tipo estado (struct estados_t)
typedef struct estados_t{
	int count;//vari�vel inteira que itera os vetores abaixo, tamb�m � respons�vel a rela��o entre eles.
	char leitura[10];//qual caractere da fita � lido na transi��o.
	char reescrita[10];//qual caractere � reescrito sobre o que � lido na transi��o.
	char direcao[10];//para qual dire��o se move a "cabe�a do leitor", no caso desse programa � incremento ou decremento no index da cadeia.
	int destino[10];//em qual estado a m�quina se encontrar� ap�s a transi��o.
}estado;

//As caracter�sticas gerais do automato s�o gravados no tipo dadosTuring(struct dados)
typedef struct dados{
	int	qtdEstados, qtdTerminais, qtdEstendidos, aceitacao;//respectivamente quantidade total de estados, quantidade de s�mbolos terminais, quantidades de s�mbolos do alfabeto estendido e estado de aceita��o.

	char *terminais, *estendidos;//String que cont�m todos os s�mbolos terminais e String que cont�m todos os s�mbolos do alfabeto estendido.
}dadosTuring;

int** preencheMatrizEstatal(dadosTuring dados);//Fun��o que gera a matriz que relaciona 2 estados.

void leTransicoes(dadosTuring dados,estado *total, int **MatrizTransicao);//Fun��o que l� um n�mero x de transi��es conforme o padr�o q x q0 y D, onde q, q0 ? Q, x, y ? S' e D ? {R, L, S}.

void lerCadeias(dadosTuring dados, int **MatrizTrancisao, estado *total);//Fun��o que l� um n�mero x de cadeias e chama a fun��o de teste, a partir do retorno da teste, escreve o resultado "aceita" ou "rejeita".

int testaCadeia(char *cadeia, dadosTuring dados, int **MatrizTrancisao, estado *total);//Fun��o respons�vel por avaliar a cadeia, retorna 1 caso a fun��o seja aceita pelo automato e 0 caso contr�rio.

int main(){
	int i, j, **MatrizTransicao;//i e j s�o iteradores, **MatrizFun��o � a matriz cujo par (x,y) identifica a exist�ncia de caminhos entre o estado Qx e Qy (posicao [i][j] == 1).
	estado *total;// *total � um vetor do tipo estado que guarda todos o n estados do automato (struct estados_t)
	dadosTuring dados;
	
	
	scanf("%d", &dados.qtdEstados);
	total = (estado*)malloc(dados.qtdEstados*sizeof(estado));//aloca��o din�mica do vetor de estados.
	
	MatrizTransicao = preencheMatrizEstatal(dados);//linha 82.
	
	for(i = 0; i < dados.qtdEstados; i++)//esse la�o inicializa o contador de controle dos estados como 0, note que cada estado tem um contador pr�prio
		total[i].count = 0;
		
	scanf("%d", &dados.qtdTerminais);
	dados.terminais = (char *)malloc(dados.qtdTerminais*(sizeof(char)));//44-48 aloca dinamicamente o vetor de s�mbolos terminais e os l� um por um
	for(i = 0; i < dados.qtdTerminais; i++){
		scanf(" %c", &dados.terminais[i]);

	}
	
	scanf("%d", &dados.qtdEstendidos);
	dados.estendidos = (char *)malloc(dados.qtdEstendidos * (sizeof(char)));//50-55 aloca dinamicamente o vetor de s�mbolos do alfabeto estendido e os l� um por um
	for(i = 0; i < dados.qtdEstendidos; i++){
		scanf(" %c", &dados.estendidos[i]);

	}

	scanf("%d", &dados.aceitacao);

	leTransicoes(dados, total, MatrizTransicao);//linha 93
	/*for(i = 0; i < dados.qtdEstados; i ++){      Optei por manter esse trecho comentado, pois foi usado no debug e � �til.
		printf("Estado Q%d\n", i);					Esse trecho imprime os dados coletados na fun��o acima facilitando a visualiza��o da correla��o
		for(j = 0; j < total[i].count; j++){		dos vetores dentro do estado[i] (qi).
			printf(" %c ",total[i].leitura[j]);		L� se: S�mbolo terminal lido		 |
		}												   S�mbolo a ser resscrito		|
		printf("\n");									   Estado que a m�quina entrar� |
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
	/*Essa fun��o aloca dinamicamente uma matriz de quantidade max de estados� elementos, calloc foi utilizado para inicializar uma matriz de 0s*/
	MatrizTransicao = (int**)malloc(dados.qtdEstados*sizeof(int*));
	for(i = 0; i < dados.qtdEstados; i++){
		MatrizTransicao[i] = (int*)calloc(dados.qtdEstados,sizeof(int));
	}
	
	return MatrizTransicao;//retorna a matriz gerada para a matriz declarada na main.
}

void leTransicoes(dadosTuring dados,estado *total, int **MatrizTransicao){
	int transicoes, i, source, dest;//transicoes � a quatidade de transi��es a serem lidas, i � um iterador, linha 100, linha 103
	char read, write, dir;//linha 102, linha 104, linha 105
	
	scanf("%d", &transicoes);
	
	for(i = 0; i < transicoes; i++){
	
		scanf("%d", &source);//estado atual da m�quina
		scanf(" %c", &read);//caractere atual lido da fita
		scanf("%d", &dest);//estado que esse caractere leva
		scanf(" %c", &write);//caractere a ser reescrito antes mover na fita
		scanf(" %c", &dir);//dire��o a mover na fita
	
		MatrizTransicao[source][dest] = 1;//identifica a exist�ncia de um caminho no par [source][dest]
		total[source].leitura[total[source].count] = read;				//Aqui come�a a correla��a, lendo "read", devo reescrever "write" e mover
		total[source].reescrita[total[source].count] = write;			//para "dir" na fita e entrar no estado "dest". Com o uso do mesmo iterador
		total[source].destino[total[source].count] = dest;				//nos 3 vetores diferentes t�m-se a rela��o de dados diferentes no mesmo
		total[source].direcao[total[source].count] = dir;				//�ndice.
		total[source].count++;											//Count s� incrementado ap�s as inser��es e � atrelado a cada estado. 
		
	}
}

void lerCadeias(dadosTuring dados, int **MatrizTransicao, estado *total){
	int qtdEntradas, i, *result;//qtdEntradas � a quantidade de entradas, i iterador, *result vetor de resultados da avalia��o da cadeia.
	char cadeia[20];//String que representa a cadeia de at� 20 elementos.
	scanf("%d", &qtdEntradas);
	
	result = (int *)malloc(qtdEntradas*sizeof(int));
	for(i = 0; i < qtdEntradas; i++){
		scanf("%s", cadeia);
		result[i] = testaCadeia(cadeia, dados, MatrizTransicao, total);//linha 138
	}//o retorno da testa cadeia � carregada no vetor de resultados
	
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
	int currState = 0, i, j, currCharIndex = 0;//currState � o estado atual da m�quina (iniciado em q0 por padr�o), i e j iteradores, currCharIndex � o ind�ce na fita para atualizar currChar;
	
	currChar = *cadeia;//o mesmo que fazer currCharr = cadeia[0], inicia currChar para o caractere mais a esquerda da fita
	while(currState != dados.aceitacao){	//o loop � executado at� atingir o estado final (a partir do qual n�o h� transi��es) ou at� que uma transi��o n seja poss�vel(explicado posteriormente)
		for(j = 0; j < total[currState].count; j++){ //dentro do vetor de estados na posi��o do estado atual busca-se valor do contador para evitar acessos desnecess�rios de mem�ria.
			if(total[currState].leitura[j] == currChar){//dentro do vetor de estados, busca-se os caracteres lidos pelo estado e os compara com currChar, caso esse loop chege ao final sem que esse if seja satisfeito j n�o passar� a verifiva��o da linha 174.
				cadeia[currCharIndex] = total[currState].reescrita[j];//a posi��o atual na cadeia � reescrita com o elemento do vetor de s�mbolos estendidos de mesmo �ndice encontrado no la�o e aceito no if.
				
				if(total[currState].direcao[j] == 'R'){//caso a dire��o seja direita o currCharIndex � incrementado uma unidade
					currCharIndex++;
					
						if(currCharIndex >= strlen(cadeia))//para garantir que a fita � "infinita", quando o index extrapola os limites do vetor cadeia, currChar � atualizado para 'B', que � o s�mbolo que representa o Branco 
								currChar = 'B';
						else//caso o �ndice esteja dentro do vetor
							currChar = cadeia[currCharIndex];//currChar recebe o caractere na posi��o currCharIndex da fita.
						
				}else{//caso n�o v� para direita 'R', pode ir para esquerda 'L' decrementando uma unidade e as intru��es seguintes s�o espelhadas da direita com mudan�as no limite do vetor
					if(total[currState].direcao[j] == 'L'){
						currCharIndex--;
						
							if(currCharIndex < 0)
								currChar = 'B';
							else
								currChar = cadeia[currCharIndex];
								
					}//caso n�o v� nem para direita nem para esquerda quer dizer que n�o h� movimento 'S', logo o currCharIndex n�o � alterado.
				}
				
				currState = total[currState].destino[j];//feitas as verifica��es e edi��es o estado atual � modificado para o destino de mesmo �ndice j dentro do vetor de estados.
				j = 200;//j � reescrito para 200 (c�digo 200 retornado em uma conex�o bem sucedida na web e valor inalcan��vel por j) caso passe o if para futura verifica��o.
				break;//feitas todas as movimenta��es o loop interno � interrompido
			}
		}
		
		if(j != 200)//caso o if n�o seja executado, j n�o atinge 200 e o loop externo � interrompido.
			break;
		
	}
	//printf("fita: %s -> q%d\n", cadeia, currState); linha de debug mostra o estado de parada da m�quina e o resultado da cadeia ap�s as modifica��es

	if(currState != dados.aceitacao){//nos casos em que a m�quina termina fora do estado de aceita��o (caso do automato de cadeias a^2n)
		for(j = 0; j < total[currState].count; j++){	//� feita uma verifica��o onde busca-se a possibilidade de ler lambda ('-') para  chegar ao 
			if(total[currState].leitura[j] == '-'){		//estado de aceita��o.
				currState = total[currState].destino[j];//caso a verifica��o seja bem sucedida o estado atual � atualizado (tlvz eu deva fzr isso em loop, vai q o �ltimo n � o de aceita��o)
				break;
			}
		}
	}
	
	if(currState != dados.aceitacao)//por fim, se o estado final do automato for o de aceita��o o retorno � 1, sen�o � 0 -> linha 126
		return 0;
	else
		return 1;
			
}





