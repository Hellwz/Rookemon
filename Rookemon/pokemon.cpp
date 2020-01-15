#include "stdafx.h"
#include "pokemon.h"
#include "map.h"

pokemon::pokemon()
{
	ID = 1;
	this->name = "admin";
	this->level = 1;
	this->hp = 50;
	this->full_hp = 50;
	for (int i = 1; i <= 4; i++)
	{
		this->pp[i] = 0;
		this->skill[i] = "null";
	}
}
pokemon::pokemon(string name,int level,int hp,int ID)
{
	this->name = name;
	this->level = level;
	this->hp = hp;
	this->full_hp = hp;
	for (int i = 1; i <= 4; i++)
	{
		this->pp[i] = 0;
		this->skill[i] = "null";
	}
	this->ID = ID;
}


pokemon::~pokemon()
{
}

string pokemon::get_info()
{
	string ret;
	ret += this->name;
	ret += "  ";
	ret += "Lv";
	ret += to_string(this->level);
	ret += "  ";
	ret += "HP:";
	ret += to_string(this->hp);
	ret += "  ";
	ret += sx[belong[this->ID]];
	return ret;
}

string sx[10] = { "Normal","Grass","Fire","Water","Elec" };// Ù–‘√˚≥∆
int pposx[16] = { 0,0,0,0,1,1,1,2,2,2,3,3,3,4,4,4 };
int pposy[16] = { 0,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2 };
int belong[16] = { 0,1,1,1,2,2,2,3,3,3,1,0,4,0,0,2};