#ifndef AILOGIC_H
#define AILOGIC_H
//#include "constants.h"
#include "Vertex.h"

class AIMind
{
public:
	AIMind(int fieldWidth, int fieldHight, int startX, int startY, int endX, int endY);
	~AIMind();
	//Places a block at the position and updates the grid, returns false if it cant be done
	void addBlock(int x, int y);
	//Checks to see if the block can be placed
	bool canAddBlock(int x, int y);
	//Removes a block from the grid if there
	void removeBlock(int x, int y);
	//Resets all the blocks to zero. Then replaces start/end and updates the field
	void resetField();
	//Resets all the distance values and returns true if there is a path from start to end
	bool updateField();
	//Sets all the distances to 0
	void clearDistances();
	//Returns if the location is on the field
	bool validLocation(int x, int y)
	{
		return (0 <= x && 0 <= y) && (x < width && y < hight);
	}
	//Returns value of block at location -3 is block -2 is end -1 is start 0 is empty
	int valueAt(int x, int y)
	{
		return field[y][x];
	}
	//Returns if the field is valid 1, 0 not valid, and -1 if the field has changed
	int isValid()
	{
		return valid;
	}
	//Get best next location and returns the position, random if multiple
	D3DXVECTOR2 bestPath(int x, int y);
	//Returns start
	D3DXVECTOR2 getStart()
	{
		return start;
	}
	//Returns end
	D3DXVECTOR2 getEnd()
	{
		return end;
	}

private:
	int** field;
	int width;
	int hight;
	D3DXVECTOR2 start;
	D3DXVECTOR2 end;

	int valid;
};




#endif