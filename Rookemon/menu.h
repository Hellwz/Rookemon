#pragma once
class menu
{
public:
	int node;// �������ĸ���(�ĸ�����) ���ͽṹ
	int father[10];
	int child[10][10];
	struct pos_
	{
        int hs, ls; //����������
	}pos[10];
	menu();
	~menu();
};

