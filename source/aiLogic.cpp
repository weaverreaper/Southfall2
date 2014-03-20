#include "aiLogic.h"
#include <queue>

AIMind::AIMind(int fieldWidth, int fieldHight, int startX, int startY, int endX, int endY): width(fieldWidth), hight(fieldHight), start(startX, startY), end(endX, endY)
{
	field = new int*[hight];
	for(int i(0); i < hight; i++)
	{
		field[i] = new int[width];
	}
	resetField();
	updateField();
}

AIMind::~AIMind()
{
	for(int i(0); i < hight; i++)
	{
		delete[] field[i];
	}
	delete[] field;
}

void AIMind::resetField()
{
	for(int i(0); i < hight; i++)
	{
		for(int j(0); j < width; j++)
		{
			field[i][j] = 0;
		}
	}
	field[static_cast<int>(start.y)][static_cast<int>(start.x)] = -1;
	field[static_cast<int>(end.y)][static_cast<int>(end.x)] = -2;
}

void AIMind::addBlock(int x, int y)
{
	if(field[y][x] != -3)
	{
		field[y][x] = -3;
		updateField();
	}

}

bool AIMind::canAddBlock(int x, int y)
{
	if(field[y][x] == -1 || field[y][x] == -2)
	{
		return false;
	}
	int temp = field[y][x];
	field[y][x] = -3;
	if(updateField())
	{
		field[y][x] = temp;
		updateField();
		return true;
	}else{
		field[y][x] = temp;
		updateField();
		return false;
	}
}

void AIMind::removeBlock(int x, int y)
{
	if(field[y][x] == -3)
	{
		field[y][x] = 0;
		updateField();
	}
}

bool AIMind::updateField()
{
	clearDistances();
	bool compleatable(false);
	std::queue<D3DXVECTOR2> blocksValue;
	blocksValue.push(end+D3DXVECTOR2(0, 1));
	blocksValue.push(end+D3DXVECTOR2(0, -1));
	blocksValue.push(end+D3DXVECTOR2(1, 0));
	blocksValue.push(end+D3DXVECTOR2(-1, 0));
	for(int i(0); i < 4; i++)
	{
		D3DXVECTOR2 pos = blocksValue.front();
		blocksValue.pop();
		if(validLocation(pos.x, pos.y))
		{
			if(field[static_cast<int>(pos.y)][static_cast<int>(pos.x)] == 0)
			{
				field[static_cast<int>(pos.y)][static_cast<int>(pos.x)] = 1;
				blocksValue.push(pos+D3DXVECTOR2(0, 1));
				blocksValue.push(pos+D3DXVECTOR2(0, -1));
				blocksValue.push(pos+D3DXVECTOR2(1, 0));
				blocksValue.push(pos+D3DXVECTOR2(-1, 0));
			}
		}
	}
	while(blocksValue.size() > 0)
	{
		D3DXVECTOR2 pos = blocksValue.front();
		blocksValue.pop();
		if(validLocation(pos.x, pos.y))
		{
			if(field[static_cast<int>(pos.y)][static_cast<int>(pos.x)] == 0)
			{
				int lowest(0);
				if(validLocation(pos.x, pos.y + 1))
				{
					blocksValue.push(pos+D3DXVECTOR2(0, 1));
					if(field[static_cast<int>(pos.y + 1)][static_cast<int>(pos.x)] > 0)
					{
						if(lowest == 0 || field[static_cast<int>(pos.y + 1)][static_cast<int>(pos.x)] < lowest)
						{
							lowest = field[static_cast<int>(pos.y + 1)][static_cast<int>(pos.x)];
						}
					}
				}
				if(validLocation(pos.x, pos.y - 1))
				{
					blocksValue.push(pos+D3DXVECTOR2(0, -1));
					if(field[static_cast<int>(pos.y - 1)][static_cast<int>(pos.x)] > 0)
					{
						if(lowest == 0 || field[static_cast<int>(pos.y - 1)][static_cast<int>(pos.x)] < lowest)
						{
							lowest = field[static_cast<int>(pos.y - 1)][static_cast<int>(pos.x)];
						}
					}
				}
				if(validLocation(pos.x + 1, pos.y))
				{
					blocksValue.push(pos+D3DXVECTOR2(1, 0));
					if(field[static_cast<int>(pos.y)][static_cast<int>(pos.x + 1)] > 0)
					{
						if(lowest == 0 || field[static_cast<int>(pos.y)][static_cast<int>(pos.x + 1)] < lowest)
						{
							lowest = field[static_cast<int>(pos.y)][static_cast<int>(pos.x + 1)];
						}
					}
				}
				if(validLocation(pos.x - 1, pos.y))
				{
					blocksValue.push(pos+D3DXVECTOR2(-1, 0));
					if(field[static_cast<int>(pos.y)][static_cast<int>(pos.x - 1)] > 0)
					{
						if(lowest == 0 || field[static_cast<int>(pos.y)][static_cast<int>(pos.x - 1)] < lowest)
						{
							lowest = field[static_cast<int>(pos.y)][static_cast<int>(pos.x - 1)];
						}
					}
				}
				if(lowest == 0)
				{
					lowest = 1000;
				}
				field[static_cast<int>(pos.y)][static_cast<int>(pos.x)] = lowest+1;
			}
		}
		if(pos == start)
		{
			compleatable = true;
		}
	}
	valid = compleatable;
	return compleatable;
}

void AIMind::clearDistances()
{
	for(int i(0); i < hight; i++)
	{
		for(int j(0); j < width; j++)
		{
			if(field[i][j] > 0)
			{
				field[i][j] = 0;
			}
		}
	}
}

D3DXVECTOR2 AIMind::bestPath(int x, int y)
{
	int placeValue = field[y][x];
	if(placeValue == 1 || placeValue == -2)
	{
		return end;
	}
	int arraySize(0);
	D3DXVECTOR2 nextSpot[4];
	if(placeValue == -1)
	{
		placeValue = 1000;
	}
	if(validLocation(x+1, y) && (field[y][x+1] < placeValue && field[y][x+1] > 0))
	{
		nextSpot[arraySize] = D3DXVECTOR2(x+1, y);
		arraySize++;
	}
	if(validLocation(x-1, y) && (field[y][x-1] < placeValue && field[y][x-1] > 0))
	{
		nextSpot[arraySize] = D3DXVECTOR2(x-1, y);
		arraySize++;
	}
	if(validLocation(x, y+1) && (field[y+1][x] < placeValue && field[y+1][x] > 0))
	{
		nextSpot[arraySize] = D3DXVECTOR2(x, y+1);
		arraySize++;
	}
	if(validLocation(x, y-1) && (field[y-1][x] < placeValue && field[y-1][x] > 0))
	{
		nextSpot[arraySize] = D3DXVECTOR2(x, y-1);
		arraySize++;
	}
	if(arraySize == 0)
	{
		return D3DXVECTOR2(x, y);
	}
	return nextSpot[rand()%arraySize];
}