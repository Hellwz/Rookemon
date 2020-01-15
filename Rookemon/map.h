#pragma once
#include<string>
#include<iostream>
using namespace std;

class map
{
public:
	map();
	~map();

};
extern int now; //在哪个地图里
extern int wtext; //触发哪段对话
extern int mp[6][15][19];

extern int b_next[100][100]; //跳到哪段对话 model=3
extern string battle_t[100];


