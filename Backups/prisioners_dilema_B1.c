//este programa consiste numa avaliação de diversas estratégias relacionadas com o dilema do prisioneiro, avaliando-as numa escala de pontos
//caso uma estratégia escolha "trair" e a outra "cooperar", a que traiu ganha 5 pontos e a que cooperou perde 2. Caso escolham as duas "cooperar", ganham ambas 3 pontos.
//caso escolham ambas "trair", cada uma perde 1 ponto

//nas jogadas das estratégias, "cooperar" é traduzido com um 0 e "trair" é traduzido com um 1

//há quatro estratégias no total. A estratégia do tolo (0) joga sempre "cooperar". A estratégia do traidor (1) joga sempre "trair". 
//A estratégia do espelho (2) joga "cooperar" inicialmente e depois copia a jogada anterior da estratégia
//adversária nas jogadas seguintes. A estratégia da memória (3) joga "cooperar" inicialmente e "trair" com estratégias que a tenham traído no passado

#include <stdio.h>
#include <stdlib.h>

#define STRAT_NUM 4

struct strategy
{	
	int id;
	int points;
	int nextmove;
	int last[2]; 					//array para o espelho: contém o último id contra o qual jogou e a respetiva jogada
	int traitors[STRAT_NUM - 1];	//para a estratégia da memória

}strategy;

struct strategy strat[STRAT_NUM];
int first_time = 0;


int main();
void init_strategies();
void reset_strategies(int, int);
void compare(int, int);
void generate_nextmove(int, int);
void simulate(int, int);
void outcome();


int main()
{
	printf("--- Program Started ---\n\n");
	init_strategies();

	for (int rep_num = 0; rep_num < 15; rep_num++)
	{
		for (int counter = 0; counter < STRAT_NUM; counter++)
		{
			for (int counter1 = counter + 1; counter1 < STRAT_NUM; counter1++)
			{
				simulate(counter, counter1);
			}
		}
	}
	outcome();

	printf("\n--- Program Terminated ---\n");
	return 0;
}

void init_strategies()
{
	for (int counter = 0; counter < STRAT_NUM; counter++)
	{
		strat[counter].id = counter;		//atribuir o id a cada estratégia para ser identificar nas funções seguintes
		strat[counter].id = 0;				//atribuir 0 pontos iniciais a cada estratégia
		strat[3].traitors[counter] = 6;		//limpa a memória da estratégia 3
	}

	strat[0].nextmove = 0;
	strat[1].nextmove = 1;
	strat[2].last[0] = 6;
	strat[2].last[1] = 0;
}

void reset_strategies(int id1, int id2)
{
	strat[2].nextmove = 0;
	strat[3].nextmove = 0;

	if (id2 == 3)
	{
		for (int counter = 0; counter < STRAT_NUM - 1; counter++) 
		{
			if (strat[id2].traitors[counter] == id1) //verifica se o outro jogador já o tinha traído
			{
				strat[3].nextmove = 1;
				break;
			}
		}
	}

}

void simulate(int id1, int id2)
{
	reset_strategies(id1, id2);
	for(int counter = 0; counter < 15; counter++)
	{
		compare(id1, id2);
		generate_nextmove(id1, id2);
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
}

void generate_nextmove(int id1, int id2)
{
	int traitor = 0;
	//strat 0 e 1 mantém-se iguais

	if (id2 == 3) //caso o segundo jogador seja a estratégia da memória (nunca pode ser o jogador 1)
	{
		strat[id2].nextmove = 0;

		for (int counter = 0; counter < STRAT_NUM - 1; counter++) 
		{
			if (strat[id2].traitors[counter] == id1) //verifica se o outro jogador já o tinha traído
			{
				traitor = 1;
				break;
			}
		}

		if ((traitor == 0) && (strat[id1].nextmove == 1)) //se a estratégia oponente não consta na lista de memória e a traiu, então procedemos ao registo
		{
			for (int counter = 0; counter < STRAT_NUM - 1; counter++)
			{
				if (strat[3].traitors[counter] == 6) //encontra um lugar vago na lista
				{
					strat[3].traitors[counter] = id1;	//coloca lá o id do oponente que traiu
					traitor = 1;
					break;
				}
			}
		}
	}

	if(id1 == 2) //caso o primeiro jogador seja o espelho
	{
		if (strat[2].last[0] == id2) //vemos se a última jogada foi contra o mesmo que agora
		{
			strat[2].nextmove = strat[2].last[1];	//se sim, então copia
			strat[2].last[1] = strat[id2].nextmove;
		}
		else
		{
			strat[2].last[0] = id2;
			strat[2].last[1] = strat[id2].nextmove;
			strat[2].nextmove = strat[2].last[1];
		}
	}
	if(id2 == 2)
	{
		if (strat[2].last[0] == id1)
		{
			strat[2].nextmove = strat[2].last[1];
			strat[2].last[1] = strat[id1].nextmove;
		}
		else
		{
			strat[2].last[0] = id1;
			strat[2].last[1] = strat[id1].nextmove;
			strat[2].nextmove = strat[2].last[1];
		}
	}

	if (traitor == 1) strat[3].nextmove = 1; //a memória joga "trair" se reconhecer um traidor

}

void outcome()
{
	for(int counter = 0; counter < STRAT_NUM; counter++)
	{
		printf("Strategy %d points: %d \n", counter, strat[counter].points);
	}	
}

