#include "stdafx.h"
#include "menu.h"


menu::menu()
{
	node = 1;
	for (int i = 1; i <= 6; i++)
	{
       pos[i].hs = 1;
	   pos[i].ls = 1;
	}
	father[1] = 0;
	child[1][1] = 2;
	child[1][2] = 3;
	child[1][3] = 4;
	child[1][4] = 5;
	child[1][5] = 6;
	child[1][6] = 0;
	father[2] = 1; //2:图鉴
	father[3] = 1; //3:精灵
	father[4] = 1; //4:背包
	father[5] = 1; //5:玩家
	father[6] = 1; //6:保存
}


menu::~menu()
{
}
