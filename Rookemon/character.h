#pragma once
#include "map.h"
#include "pokemon.h"
class character
{
private:
	int x[10], y[10]; // 0<=x<=18 0<=y<=14 //每个地图里的坐标
	// x y 用的时候要反过来
	int dir[10];// 1 2 3 4 up down left right
	
public:
	pokemon *p[7];
	
	character();
	~character();
	int check(int x, int y);
	int get_x() { return x[now]; }
	int get_y() { return y[now]; }
	int get_dir() { return dir[now]; }
	void set_x(int x) { if (check(x,this->y[now]) != 0) this->x[now] = x; }
	void set_y(int y) { if (check(this->x[now],y) != 0) this->y[now] = y; }
	void set_dir(int dir) { this->dir[now] = dir; }
	
	bool add_pokemon(pokemon *x);
};

