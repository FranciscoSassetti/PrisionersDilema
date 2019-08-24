//este programa consiste numa avaliação de diversas estratégias relacionadas com o dilema do prisioneiro, avaliando-as numa escala de pontos
//caso uma estratégia escolha "trair" e a outra "cooperar", a que traiu ganha 5 pontos e a que cooperou perde 2. Caso escolham as duas "cooperar", ganham ambas 3 pontos.
//caso escolham ambas "trair", cada uma perde 1 ponto

//nas jogadas das estratégias, "cooperar" é traduzido com um 0 e "trair" é traduzido com um 1

//há quatro estratégias no total. A estratégia do tolo (0) joga sempre "cooperar". A estratégia do traidor (1) joga sempre "trair". 
//A estratégia do espelho (2) joga "cooperar" inicialmente e depois copia a jogada anterior da estratégia
//adversária nas jogadas seguintes. A estratégia da memória (3) joga "cooperar" inicialmente e "trair" com estratégias que a tenham traído no passado

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PLAYER_NUM 100

struct strategy
{	
	int st;
	int id;
	int points;
	int nextmove;
	int last[2]; 							//array para o espelho: contém o último id contra o qual jogou e a respetiva jogada
	int traitors[PLAYER_NUM];				//para a estratégia da memória: têm a lista das estratégias que jogaram "trair" contra ela
	int death;

}strategy;

struct strategy strat[PLAYER_NUM];

int main();
void init_strategies();
void compare(int, int);
void generate_nextmove(int, int);
void strat2_check(int, int);
void strat3_check(int, int);
void simulate(int, int);
void statistics();


int main()
{
	int id1, id2;
	time_t t;
	srand((unsigned) time(&t));

	printf("--- Program Started ---\n\n");
	init_strategies();

	for (int rep_num = 0; rep_num < 1000000; rep_num++) //número de vezes que os indivíduos selecionados se encontram
	{
		id1 = id2 = 0;
		while(id1 == id2)
		{
			id1 = rand()%PLAYER_NUM; //escolhem-se dois indivíduos ao acaso
			id2 = rand()%PLAYER_NUM;
		}
		simulate(id1, id2); //confronto entre os indivíduos selecionados
	}
	statistics();

	printf("\n--- Program Terminated ---\n");
	return 0;
}

void init_strategies()
{
	time_t t;
	srand((unsigned) time(&t));

	for (int counter = 0; counter < PLAYER_NUM; counter++)
	{
		strat[counter].id = counter; 	//atribui um id a cada indivíduo
		strat[counter].st = rand()%4;	//atribui uma estratégia aleatoriamente a cada indivíduo
		strat[counter].death = 0;		//garante que todos os indivíduos estão vivos
	}

	for (int counter = 0; counter < PLAYER_NUM; counter++)
	{
		if (strat[counter].st == 1) strat[counter].nextmove = 1; //atribui a ação "trair" a estratégias traidoras
		else strat[counter].nextmove = 0;	//atribui a ação "cooperar" às restantes

		if (strat[counter].st == 2)
		{
			strat[counter].last[0] = PLAYER_NUM; 	//limpa o último jogador das estratégias espelho
			strat[counter].last[1] = 0; 		//atribui "cooperar" à primeira jogada das estratégias espelho
		}

		if(strat[counter].st == 3)
		{
			for(int counter1 = 0; counter1 < PLAYER_NUM - 1; counter1++)
			{
				strat[counter].traitors[counter1] = PLAYER_NUM;	//limpa a lista de traidores das estratégias de memória
			}
		}
	}
}

void simulate(int id1, int id2)
{
	for (int counter = 0; counter < 5; counter++)
	{
		if ((strat[id1].death == 0) && (strat[id2].death == 0))
		{
			generate_nextmove(id1, id2);
			compare(id1, id2);
		}
		else break;
	}
}

void compare(int id1, int id2) //atribuição de pontos consoante as jogadas efetuadas
{
	if (strat[id1].nextmove > strat[id2].nextmove)
	{
		strat[id1].points += 5;
		strat[id2].points -= 2;
	}
	if (strat[id1].nextmove < strat[id2].nextmove)
	{
		strat[id1].points -= 2;
		strat[id2].points += 5;
	}
	if ((strat[id1].nextmove == strat[id2].nextmove) && (strat[id1].nextmove == 0)) //jogaram ambas "cooperar"
	{
		strat[id1].points += 3;
		strat[id2].points += 3;
	}
	if ((strat[id1].nextmove == strat[id2].nextmove) && (strat[id1].nextmove == 1)) //jogaram ambas "trair"
	{
		strat[id1].points -= 1;
		strat[id2].points -= 1;
	}

	if(strat[id1].points < -6) strat[id1].death = 1;
	if(strat[id2].points < -6) strat[id2].death = 1;

}

void generate_nextmove(int id1, int id2)
{
	//strat 0 e 1 mantém-se iguais

	strat3_check(id1, id2); //quando o primeiro jogador é estratégia de memória, verifica-se se o segundo jogador já "traiu" (e regista esse jogador caso seja a primeira vez a "trair")
	strat3_check(id2, id1);	//o mesmo para o segundo jogador
	
	strat2_check(id1, id2);
	strat2_check(id2, id1);
}

void strat2_check(int a, int b) //"a" é o indivíduo para o qual se verifica se é estratégia espelho e "b" é o indivíduo a ser verificado de acordo com os parâmetros referidos
{
	if(strat[a].st == 2) //caso o primeiro jogador seja o espelho
	{
		if (strat[a].last[0] == b) //vemos se a última jogada foi contra o mesmo que agora
		{
			strat[a].nextmove = strat[a].last[1];	//se sim, então copia
			strat[a].last[1] = strat[b].nextmove;
		}
		else 	//caso não tenha sido
		{
			strat[a].last[0] = b;	//registamos esse novo jogador
			strat[a].last[1] = strat[b].nextmove;	//copiamos o seu último movimento
			strat[a].nextmove = 0;	//atribuímos esse movimento à próxima jogada
		}
	}
}

void strat3_check(int a, int b) //"a" é o indivíduo para o qual se está a verificar se é estratégia de memória e "b" é o indivíduo que se verifica nas listas (ambos são IDs)
{

	if (strat[a].st == 3)
	{
		strat[a].nextmove = 0;

		for (int counter = 0; counter < PLAYER_NUM - 1; counter++) 
		{
			if (strat[a].traitors[counter] == b) //verifica se o outro indivíduo já a tinha traído
			{
				strat[a].nextmove = 1;
				break;			
			}
		}

		if (strat[b].nextmove == 1) //se a estratégia oponente não consta na lista de memória e a traiu, então procedemos ao registo
		{
			for (int counter = 0; counter < PLAYER_NUM - 1; counter++)
			{
				if (strat[a].traitors[counter] == PLAYER_NUM) //encontra um lugar vago na lista
				{
					strat[a].traitors[counter] = b;	//coloca aí o id do oponente que traiu
					strat[a].nextmove = 1;
					break;
				}
			}
		}
	}
}

void statistics()
{
	int sum[4], n[4], n_alive[4];

	for(int counter = 0; counter < 4; counter++) //inicialização das variáveis para fazer a média
	{
		sum[counter] = 0;
		n[counter] = 0;
		n_alive[counter] = 0;
	}

	for(int counter = 0; counter < PLAYER_NUM; counter++)
	{
		//printf("Individual %d (using strategy %d) points: %d", counter, strat[counter].st, strat[counter].points);
		/*if (strat[counter].death == 1) 
		{
			printf(" (Died)");
		}
		printf("\n"); */

		sum[strat[counter].st] += strat[counter].points; //soma de pontos por estratégia
		(n[strat[counter].st])++;	//número de indivíduos por estratégia

		if (strat[counter].death == 0) 
		{
			(n_alive[strat[counter].st])++;	//número de indivíduos vivos por estratégia
		}
	}

	//printf("\n");
	for (int counter = 0; counter < 4; counter++)
	{
		if(n[counter] == 0) 
		{
			n[counter]++; //para evitar divisão por 0
		}
		printf("Average of Strategy %d: %.2f\n", counter, (((double) sum[counter])/ ((double) n[counter])) );
		printf("	Survival rate: %.3f (%d out of %d)\n", ((double) n_alive[counter])/ ((double) n[counter]), n_alive[counter], n[counter]);
	}
}