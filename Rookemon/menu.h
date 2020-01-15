#pragma once
class menu
{
public:
	int node;// 现在在哪个点(哪个界面) 树型结构
	int father[10];
	int child[10][10];
	struct pos_
	{
        int hs, ls; //行数、列数
	}pos[10];
	menu();
	~menu();
};

