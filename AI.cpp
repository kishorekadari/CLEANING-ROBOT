//Final
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include<stdio.h>
#include <string>
#include <algorithm>
#include <iomanip>
#include<iterator>
#define TOO_LOW     case 0: \
                    case 1: \
                    case 2: \
                    case 3:
#define TOO_HIGH     case 11: \
                    case 12: \
                    case 13: \
                    case 14:
#define M 16  //// Maze size 
using namespace std;
time_t t;
int i = 0, n = 1;
int x=0, y=0;// x,y coordinates
int z, w;
int visited[400][400];//
//int sol[M][M];
int clean = 0;
int maze[M][M];
int reward = 0;
//int printsolution();
int prob;
int state[16][16];
double a=0.7;// learning parameter
double g = 0.9;// discount factor, gamma
int k; //epoch
int D;
//double f = rand() / (RAND_MAX + 1.0);
struct st
{
	int m_agentX, m_agentY, reward;
	/*void getdata()
	{
		cout << "z, w";
	}*/
}r, s;
int m_agentX=0, m_agentY=0;
int const sol[M][M] = 
{ { 3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,3 },
  { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,0,3,3,3,0,0,3,3,3,0,0,0,0,0,3 },
  { 3,0,3,3,3,0,0,3,3,3,0,0,0,0,0,3 },
  { 3,0,3,3,3,0,0,3,3,3,0,0,0,0,0,3 },
  { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,3 },
  { 3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,3 },
  { 3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,3 },
  { 3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,3,0,0,0,3,3,3,0,0,0,0,0,0,0,3 },
  { 3,3,0,0,0,3,3,3,0,0,0,0,0,0,0,3 },
  { 3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 }
};
char solup[M][M] =
{ 
  { 3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,3 },
  { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,0,3,3,3,0,0,3,3,3,0,0,0,0,0,3 },
  { 3,0,3,3,3,0,0,3,3,3,0,0,0,0,0,3 },
  { 3,0,3,3,3,0,0,3,3,3,0,0,0,0,0,3 },
  { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,3 },
  { 3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,3 },
  { 3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,3 },
  { 3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,3,0,0,0,3,3,3,0,0,0,0,0,0,0,3 },
  { 3,3,0,0,0,3,3,3,0,0,0,0,0,0,0,3 },
  { 3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
  { 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 }
};
void reset()
{
	m_agentX = 0;
	m_agentY = 13;
	solup[m_agentX][m_agentY] = sol[M][M];
}
double Qtable[16][16][4];
double maxQ[16][16][4];
enum class Actions
{
	North,
	South,
	East,
	West
};
class Roboclean
{

	
	public:
	int step1(Actions action)
	{
		if (action == Actions::North)
		{
			return moveAgent(0, -1);
				printf("North");
		}
		else if (action == Actions::South)
		{
				return moveAgent(0, +1);
				printf("South");	
		}
		else if (action == Actions::East)
		{
				return moveAgent(+1, 0);
				printf("East");
		}
		else if (action == Actions::West)
		{
			return moveAgent(-1, 0);
				printf("West");
		}
		return 0;
	}
public:
	int moveAgent(int x, int y)
	{
		int reward = 0;
		
		if (solup[m_agentX+x][m_agentY+y] != 3)
		{
			m_agentX += x;
			m_agentY += y;
		}
		if (solup[m_agentX][m_agentY]!= 3)
		{
			solup[m_agentX][m_agentY] = 1;
		}
		if (m_agentX < 0)
		{
			m_agentX = 0;
			reward = -10;
		}
		if (solup[m_agentX][m_agentY] == 1)
		{
			reward = -1;
			
		}
		if (solup[m_agentX][m_agentY] == 0)
		{
			reward = 1;
		}
		
		return reward;

	}
public:
	int getAgentX()
	{
		return m_agentX;
	}
	int getAgentY()
	{
		return m_agentY;
	}
};
	int totalEpochs;
	int totalsteps;
	class print
	{
	public:
int printsolution()
	{
		for (m_agentX = 0; m_agentX < 16; m_agentX++)
		{
				printf("\n\n");
				for (m_agentY = 0; m_agentY < 16; m_agentY++)
				{
					printf("%d", solup[m_agentX][m_agentY]);
				}
		}
		
	return solup[M][M];
}
int printsolution1()
{
	
	for (m_agentX = 0; m_agentX < 16; m_agentX++)
	{
		printf("\n");
		for (m_agentY = 0; m_agentY < 16; m_agentY++)
		{
			printf(" %d          ", Qtable[m_agentX][m_agentY][D]);
		}
	}
	return Qtable[M][M][4];
}
	
};

	class Qlearn
	{
		Roboclean game;
	public:
		void QLearner(Roboclean& game, int totalEpochs, int totalsteps)
		{
			const Actions actionList[4] = {
		 Actions::North,
		 Actions::South,
		 Actions::West,
		 Actions::East
			};
			srand((unsigned)time(&t));
			for (int epoch = 1; epoch <= totalEpochs; ++epoch)
			{
				double f = rand() / (RAND_MAX + 1.0);
				reset();
				printf("epoch-%d\t", epoch);
				printf("f-%f\t", f);

				for (int step = 1; step <= totalsteps; ++step)
				{
					
					int oldX = game.getAgentX();
					int oldY = game.getAgentY();
					int agentX = game.getAgentX();
					int agentY = game.getAgentY();
					
			int D = rand() % 4;
			//int D = distance(Qtable[agentX][agentY], max_element(Qtable[agentX][agentY], Qtable[agentX][agentY] + 4));
					printf("D-%d\t", D);
					//printf("oldX, oldY %d %d ", oldX, oldY);
					int result = game.step1(actionList[D]);
					//printf("Result%d ", result);
					agentX = game.getAgentX();
					agentY = game.getAgentY();
					printf("agentX, agentY %d %d \n", agentX, agentY);
					
					double maxq = *max_element(Qtable[agentX][agentY], Qtable[agentX][agentY] + 4);

					Qtable[oldX][oldY][D] = Qtable[oldX][oldY][D] + (a*(result + (g * maxq) - Qtable[oldX][oldY][D]));
					


				}
			}
		}
	};
		int main()
		{
			reset();
		m_agentX = 0;
		m_agentY = 13;
			print print1;
			Roboclean game;
			Qlearn agent;
			agent.QLearner(game, 55, 200);
			printf("%d ", game.moveAgent(m_agentX,m_agentY));
			print1.printsolution();
			print1.printsolution1();
			cin.clear();
			cin.ignore();
			cin.get();
			return 0;
			system("CLS");
		}
