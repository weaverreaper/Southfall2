#ifndef WAVE_H
#define WAVE_H
#include "enemy.h"
#include "constants.h"
#define WIN32_LEAN_AND_MEAN

class Wave
{
public:
	Wave()
	{
		for(int i(0); i < MAXENEMIES*15; i++)
		{
			enemies[i] = 0;
		}
		enemyCount = 0;
	}
	~Wave()
	{
		for(int i(0); i < enemyCount; i++)
		{
			delete enemies[i];
		}
	}
	Enemy* operator[](int index)
	{
		return enemies[index];
	}
	void Add(float health, int red, int blue, int green, int number)
	{
		int loops = min(max(MAXENEMIES*15-enemyCount, 0), number);
		for(int i(0); i < loops; i++)
		{
			enemies[enemyCount] = new Enemy(health, red, blue, green);
			enemyCount++;
		}
	}
	int size()
	{
		return enemyCount;
	}



private:
	int enemyCount;
	Enemy* enemies[MAXENEMIES*15];



};






#endif