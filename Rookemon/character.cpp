#include "stdafx.h"
#include "character.h"
#include "map.h"


character::character()
{
	x[4] = 9;
	y[4] = 6;
	dir[4] = 2;
	x[0] = 2;
	y[0] = 13;
	dir[0] = 2;
	x[2] = 9;
	y[2] = 9;
	dir[2] = 1;
}


character::~character()
{
}

int character::check(int x, int y)
{
	if (x < 0 || x > 18) return 0;
	if (y < 0 || y > 14) return 0;
	if (mp[now][y][x] == 9) return 2;
	if (mp[now][y][x] > 0 && mp[now][y][x] < 100) return 0;
	return 1;
}

bool character::add_pokemon(pokemon * x)
{
	for (int i = 1; i <= 6; i++)
	{
		if (p[i] == NULL) {
			p[i] = x; 
			return 1;
		}
	}
	return 0;
}

