#pragma once

#include<iostream>
#include<string>
using namespace std;

class pokemon
{
private:
	string name;
	int level;
	int hp;
	int ID;
	string skill[5];
	int pp[5];
public:
	int full_hp;

	pokemon();
	pokemon(string name, int level, int hp, int ID);
	~pokemon();

	void set_name(string s) { name = s; }
	string get_name() { return name; }

	void set_level(int x) { level = x; }
	int get_level() { return level; }

	void set_hp(int x) { hp = x; }
	int get_hp() { return hp; }

	void set_ID(int x) { ID = x; }
	int get_ID() { return ID; }

	string get_info();

	void set_pp(int p1,int p2,int p3,int p4)
    {
		pp[1] = p1;
		pp[2] = p2;
		pp[3] = p3;
		pp[4] = p4;
	}
	int get_pp1() { return pp[1]; }
	int get_pp2() { return pp[2]; }
	int get_pp3() { return pp[3]; }
	int get_pp4() { return pp[4]; }

};
extern string sx[10];//属性的名称
extern int pposx[16], pposy[16];//pokemon position in bmp
extern int belong[16];//某个ID是什么属性


