#include "Entity.h"

float findHeight(float z3, float z1, float z2, float a, float b)
{
	float t = z1*(1-a/(1-b))+z2*a/(1-b);
	return t*(1-b)+z3*b;
}

void Entity::checkTerrain()
{
	if(position.x > 0 && position.x < (terr->x-1)*terr->scale && position.z > 0 && position.z < (terr->z-1)*terr->scale)
	{		
		int tx = int(position.x/terr->scale);
		int tz = int(position.z/terr->scale);
		float rx = position.x/terr->scale - tx;
		float rz = position.z/terr->scale - tz;
		if(rx + rz <= 1 && rx > rz)
			position.y = max(findHeight(terr->grid[tx][tz+1],terr->grid[tx][tz],terr->grid[tx+1][tz],rx,rz), 
				findHeight(terr->grid[tx][tz],terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],rz,1-rx));
		else if(rx + rz > 1 && rx > rz)
			position.y = max(findHeight(terr->grid[tx][tz],terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],rz,1-rx), 
				findHeight(terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],1-rx,1-rz));
		else if(rx + rz > 1 && rx <= rz)
			position.y = max(findHeight(terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],1-rx,1-rz), 
				findHeight(terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],terr->grid[tx][tz],1-rz,rx));
		else if(rx + rz <= 1 && rx <= rz)
			position.y = max(findHeight(terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],terr->grid[tx][tz],1-rz,rx), 
				findHeight(terr->grid[tx][tz+1],terr->grid[tx][tz],terr->grid[tx+1][tz],rx,rz));

		position.y = position.y*terr->scale;
	}
}


void Entity::update(float dt)
{
	//insert AI, etc. here
	checkTerrain();
	GeoObject::update(dt);
}