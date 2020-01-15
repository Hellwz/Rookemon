// Rookemon.cpp : 定义应用程序的入口点。
// 不要在重绘函数中修改任何数据信息

#include "stdafx.h"
#include "Rookemon.h"
#include <iostream>
#include <string>
#include <wingdi.h>
#include "character.h"
#include "menu.h"
#include "text.h"
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
using namespace std;

#define MAX_LOADSTRING 100
#pragma warning(disable:4996)

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HDC mdc;

//转换函数
LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void				MyPaint(HDC hdc);
void				MyPaint2(HDC hdc);
pokemon*            create(int num);
pokemon*            create(string nn, int ll, int hh, int ii);
int                 spj(int xx);
void                change(int xx);
void                save();
void                load();

//My code
    
	character R; //Red
	menu M;
	int model=0; // 1 行走 2 菜单 3 对战 0 开始
	int b_now = 0; //战斗选项
	bool talking=0;
	int yyy = 0;//golden finger
	string tmp_s;
	pokemon *tmp_p;
	pokemon *my, *enemy;
	bool flag1 = 0;//是否需要进行对战精灵血量改变
	int mouse_x, mouse_y;
	int tmp_x, tmp_y, tmp_dir;//暂时保存坐标
	int tmp_level, tmp_hp, tmp_ID;
	char* tmp_c;
	bool lock1=0; //需要先进行对话
	double per; //血量百分比

	int lastx=9, lasty=6;//人物移动记录，用于人物移动动画
	int addx = 0, addy = 0;
	bool show1=0;//是否播放过攻击动画
	int lastmodel = 0;//上一个模式是什么，用于播放bgm

	//text T;
	wchar_t s1[] = L"--- 请问您需要治疗您的精灵吗?";
	wchar_t s2[] = L"--- 是（X），否（Z）";
	wchar_t s3[] = L"--- 好的，您的精灵已治疗完毕，欢迎下次再来玩哦";
	wchar_t s4[] = L"--- 好的（X）";
	wchar_t s5[] = L"--- 欢迎光临！";
	wchar_t s6[] = L"确认（X）";
	wchar_t s7[] = L"请选择一只精灵：";
	wchar_t s8[] = L"选择 小火龙（1） 杰尼龟（2） 妙蛙种子（3）";
	wchar_t s9[] = L"取消（Z）";
	wchar_t s10[] = L"您还没有精灵！请到兰博士的实验室领取！";
	wchar_t s11[] = L"已覆盖上次存档，保存成功！";
	wchar_t s12[] = L"嗯？我是谁？我在哪？...头好痛...";
	wchar_t s13[] = L"--- 你怎么才起来？！你忘了今天是什么日子了吗？";
	wchar_t s14[] = L"--- 咦？什么日子？您...是我妈妈？（X）";
	wchar_t s15[] = L"--- 你睡傻了吗？今天是你去领精灵开始冒险的日子啊！快去！";
	wchar_t s16[] = L"--- 哦哦哦，我马上去...（X）";
	wchar_t s17[] = L"请先和妈妈对话！";
	wchar_t s18[] = L"--- 快去！出门左转就是兰博士的实验室!";
	wchar_t s19[] = L"桌上有一张纸条：";
	wchar_t s20[] = L"--- 小智啊，我有事出去了，精灵就在桌上，你选一只带走吧！";
	wchar_t s21[] = L"--- 我...叫小智？ 那我就不客气了...（X）";
	wchar_t s22[] = L"河水蓝蓝的...可我不会游泳啊...还是去问问博士怎么办吧!";


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
	//R.add_pokemon(create(4));

	/*now = 4;
	R.set_x(9);
	R.set_y(6);
	R.set_dir(2);*/

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ROOKEMON, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ROOKEMON));

    MSG msg;

	PlaySound(L"1.wav", NULL, SND_FILENAME | SND_ASYNC);
    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ROOKEMON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ROOKEMON);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
      CW_USEDEFAULT, 0, 586, 508, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
	    {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...
			HBITMAP hbmp;// 位图绘制对象句柄
			mdc = CreateCompatibleDC(hdc); // 创建兼容的缓存DC对象
			switch (model)
			{
			case 1:
			{
				switch (now)
				{
				case 0:
					hbmp = (HBITMAP)LoadImage(NULL, L"map0.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
					break;
				case 2:
					hbmp = (HBITMAP)LoadImage(NULL, L"map2.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
					break;
				case 3:
					hbmp = (HBITMAP)LoadImage(NULL, L"map101.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
					break;
				case 4:
					hbmp = (HBITMAP)LoadImage(NULL, L"room.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
					break;
				case 5:
					hbmp = (HBITMAP)LoadImage(NULL, L"lab.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
					break;
				default:
					hbmp = (HBITMAP)LoadImage(NULL, L"map0.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
				}
			}break;
			case 2:
				switch (M.node)
				{
				case 1:
					hbmp = (HBITMAP)LoadImage(NULL, L"menu.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
					break;
				case 2:
					hbmp = (HBITMAP)LoadImage(NULL, L"menu2.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
					break;
				case 3:
					hbmp = (HBITMAP)LoadImage(NULL, L"menu3.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
					break;
				case 4:
					hbmp = (HBITMAP)LoadImage(NULL, L"menu4.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
					break;
				case 5:
					hbmp = (HBITMAP)LoadImage(NULL, L"menu5.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
					break;
				case 6:
					hbmp = (HBITMAP)LoadImage(NULL, L"menu.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
					break;
				default:
					hbmp = (HBITMAP)LoadImage(NULL, L"menu.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
				}
				break;
			case 3:
				hbmp = (HBITMAP)LoadImage(NULL, L"battle_background.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
				break;
			default:hbmp = (HBITMAP)LoadImage(NULL, L"white.bmp", IMAGE_BITMAP, 570, 450, LR_LOADFROMFILE);
			}

			// 缓存DC（mdc）选择位图绘制对象（可以理解为将图片保存到mdc中）
			SelectObject(mdc, hbmp);
			BitBlt(hdc, 0, 0, 570, 450, mdc, 0, 0, SRCCOPY);

			/* tmp
			hbmp = (HBITMAP)LoadImage(NULL, L"grass.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
			SelectObject(mdc, hbmp);
			for (int i = 0; i < 19; i++)
				for (int j = 0;j < 15; j++)
					BitBlt(hdc, i*30, j*30, 30, 30, mdc, 0, 0, SRCCOPY);*/

			if (model == 1)
			{
                if (R.get_x() != lastx || R.get_y() != lasty)
				{
					lastx = R.get_x();
					lasty = R.get_y();
					MyPaint2(hdc);//走半格
					Sleep(100);
					InvalidateRect(hWnd, NULL, true); //相当于重绘两次
					return 0;
				}
			}

			MyPaint(hdc);

			if (talking == 1)
			{
				//MessageBox(NULL, TEXT("Hello World"), TEXT("This is a C program"), MB_OK);
				hbmp = (HBITMAP)LoadImage(NULL, L"dialog.bmp", IMAGE_BITMAP, 570, 240, LR_LOADFROMFILE);
				SelectObject(mdc, hbmp);
				BitBlt(hdc, 0, 322, 570, 120, mdc, 0, 120, SRCAND);
				BitBlt(hdc, 0, 322, 570, 120, mdc, 0, 0, SRCPAINT);

				//攻击动画
				if (model == 3 && show1 == 0)
				{
					show1 = 1;
					if (b_now == 7)
					{
						hbmp = (HBITMAP)LoadImage(NULL, L"attack1.bmp", IMAGE_BITMAP, 120, 60, LR_LOADFROMFILE);
						SelectObject(mdc, hbmp);
						//BitBlt(hdc, 270, 160, 60, 55, mdc, 0, 0, SRCCOPY);
						for (int i = 0; i < 5; i++)
						{
							BitBlt(hdc, 210+i*20, 160-i*14, 60, 60, mdc, 60, 0, SRCAND);
							BitBlt(hdc, 210+i*20, 160-i*14, 60, 60, mdc, 0, 0, SRCPAINT);
							Sleep(200);
						}
						InvalidateRect(hWnd, NULL, true);
					}
					if (b_now == 13)
					{
						hbmp = (HBITMAP)LoadImage(NULL, L"attack2.bmp", IMAGE_BITMAP, 120, 60, LR_LOADFROMFILE);
						SelectObject(mdc, hbmp);
						for (int i = 0; i < 5; i++)
						{
							BitBlt(hdc, 290 - i * 20, 110 + i * 14, 60, 60, mdc, 60, 0, SRCAND);
							BitBlt(hdc, 290 - i * 20, 110 + i * 14, 60, 60, mdc, 0, 0, SRCPAINT);
							Sleep(200);
						}
						InvalidateRect(hWnd, NULL, true);
					}
				}

				SetBkMode(hdc, TRANSPARENT);
				//wchar_t ss[]= L"这是一个小彩蛋";
				if (model == 3)
				{
					//b_now = spj(b_now);//special judge
					switch (b_now)
					{
					case 1:
						tmp_s = battle_t[b_now] + " " + tmp_p->get_name() + "!";
						TextOut(hdc, 60, 340, stringToLPCWSTR(tmp_s), wcslen(stringToLPCWSTR(tmp_s)));
						TextOut(hdc, 60, 365, stringToLPCWSTR(battle_t[b_now + 1]), wcslen(stringToLPCWSTR(battle_t[b_now + 1])));
						break;
					case 3:
						tmp_s = battle_t[b_now] + " " + R.p[1]->get_name() + "!";
						TextOut(hdc, 60, 340, stringToLPCWSTR(tmp_s), wcslen(stringToLPCWSTR(tmp_s)));
						TextOut(hdc, 60, 365, stringToLPCWSTR(battle_t[b_now + 1]), wcslen(stringToLPCWSTR(battle_t[b_now + 1])));
						break;
					case 7:
						tmp_s = battle_t[b_now] + to_string(tmp_p->get_hp()) + "!";
						TextOut(hdc, 60, 340, stringToLPCWSTR(tmp_s), wcslen(stringToLPCWSTR(tmp_s)));
						TextOut(hdc, 60, 365, stringToLPCWSTR(battle_t[b_now + 1]), wcslen(stringToLPCWSTR(battle_t[b_now + 1])));
						break;
				    case 9:	
					case 13:
						tmp_s = battle_t[b_now] + to_string(R.p[1]->get_hp()) + "!";
						TextOut(hdc, 60, 340, stringToLPCWSTR(tmp_s), wcslen(stringToLPCWSTR(tmp_s)));
						TextOut(hdc, 60, 365, stringToLPCWSTR(battle_t[b_now + 1]), wcslen(stringToLPCWSTR(battle_t[b_now + 1])));
						break;
					default:
                        TextOut(hdc, 60, 340, stringToLPCWSTR(battle_t[b_now]), wcslen(stringToLPCWSTR(battle_t[b_now])));
					    TextOut(hdc, 60, 365, stringToLPCWSTR(battle_t[b_now + 1]), wcslen(stringToLPCWSTR(battle_t[b_now + 1])));
					}
				}
				else
				{
					switch (wtext)
					{
					case 1:
						TextOut(hdc, 60, 340, s1, wcslen(s1));
						TextOut(hdc, 60, 370, s2, wcslen(s2));
						break;
					case 2:
						TextOut(hdc, 60, 340, s3, wcslen(s3));
						TextOut(hdc, 60, 370, s4, wcslen(s4));
						break;
					case 3:
						TextOut(hdc, 60, 340, s5, wcslen(s5));
						TextOut(hdc, 60, 370, s6, wcslen(s6));
						break;
					case 4:
						TextOut(hdc, 60, 340, s7, wcslen(s7));
						TextOut(hdc, 60, 370, s8, wcslen(s8));
						TextOut(hdc, 60, 400, s9, wcslen(s9));
						break;
					case 5: //没有精灵
						TextOut(hdc, 60, 340, s10, wcslen(s10));
						TextOut(hdc, 60, 370, s6, wcslen(s6));
						break;
					case 6:
						TextOut(hdc, 60, 340, s11, wcslen(s11));
						TextOut(hdc, 60, 370, s6, wcslen(s6));
						break;
					case 7: //开场白
						TextOut(hdc, 60, 340, s12, wcslen(s12));
						TextOut(hdc, 60, 370, s6, wcslen(s6));
						break;
					case 8: //和妈妈对话
						TextOut(hdc, 60, 340, s13, wcslen(s13));
						TextOut(hdc, 60, 370, s14, wcslen(s14));
						break;
					case 9: //和妈妈对话
						TextOut(hdc, 60, 340, s15, wcslen(s15));
						TextOut(hdc, 60, 370, s16, wcslen(s16));
						break;
					case 10: 
						TextOut(hdc, 60, 340, s17, wcslen(s17));
						TextOut(hdc, 60, 370, s6, wcslen(s6));
						break;
					case 11:
						TextOut(hdc, 60, 340, s18, wcslen(s18));
						TextOut(hdc, 60, 370, s4, wcslen(s4));
						break;
					case 12: //领精灵
						TextOut(hdc, 60, 340, s19, wcslen(s19));
						TextOut(hdc, 60, 370, s20, wcslen(s20));
						TextOut(hdc, 60, 400, s21, wcslen(s21));
						break;
					case 13:
						TextOut(hdc, 60, 340, s22, wcslen(s22));
						TextOut(hdc, 60, 370, s6, wcslen(s6));
						break;
					}
				}
			}
			DeleteObject(hbmp);
			DeleteDC(mdc);

			EndPaint(hWnd, &ps);
	    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

	//My code
	case WM_KEYDOWN:
	{
		if (model == 0) return 0;
		if (talking == 1&& model != 3)
		{
			switch (wParam)
			{
			case 88://X
				switch (wtext)
				{
				case 1:
					wtext = 2;
					for (int i = 1; i <= 6; i++)
					{
						if (R.p[i] == NULL) continue;
						R.p[i]->set_hp(R.p[i]->full_hp);
					}
					break;
				case 8:
					wtext = 9;
					break;
				case 12:
					wtext = 4;
					break;
				case 2:
				case 3:
				case 5:
				case 6:
				case 7:
				case 9:
				case 10:
				case 11:
				case 13:
					talking = 0;
					break;
				}
				InvalidateRect(hWnd, NULL, true);
				break;
			case 90://Z
				switch (wtext)
				{
				case 1:
					wtext = 3;
					break;
				case 4:
					talking = 0;
					break;
				}
				InvalidateRect(hWnd, NULL, true);
				break;
			case 49://1
				if (wtext == 4)
				{
					talking = 0;
					R.add_pokemon(create(4));
				}
				InvalidateRect(hWnd, NULL, true);
				break;
			case 50://2
				if (wtext == 4)
				{
					talking = 0;
					R.add_pokemon(create(7));
				}
				InvalidateRect(hWnd, NULL, true);
				break;
			case 51://3
				if (wtext == 4)
				{
					talking = 0;
					R.add_pokemon(create(1));
				}
				InvalidateRect(hWnd, NULL, true);
				break;
			}
			return 0;
		}
		switch (wParam) //非对话状态
		{
		case 88: //X
			if (model == 2)
			{
				if (M.node == 1 && M.pos[1].hs == 5) save();
				if (M.node==1 && M.pos[1].hs == 6)
					model = 1;
				if (M.child[M.node][M.pos[M.node].hs] != 0)
					M.node = M.child[M.node][M.pos[M.node].hs];
				if (M.node == 6)
				{
					M.node = 1;
					talking = 1;
					wtext = 6;
				}
			}
			else if (model == 1)
				 {
					if (R.get_dir() == 1 && R.get_y() - 1 >= 0 && mp[now][R.get_y() - 1][R.get_x()] == 7) //护士
					{   
						talking = 1;
						wtext = 1;
					}
					if (R.get_dir() == 1 && R.get_y() - 1 >= 0 && mp[now][R.get_y() - 1][R.get_x()] == 8) //妈妈
					{
						talking = 1;
						if (lock1 == 1) wtext = 11;
						else
						{
							wtext = 8;
							lock1 = 1;
						}
					}
					if (R.get_dir() == 1 && R.get_y() - 1 >= 0 && mp[now][R.get_y() - 1][R.get_x()] == 10) //领精灵
					{
						talking = 1;
						wtext = 12;
					}
					if (R.get_dir() == 1 && R.get_y() - 1 >= 0 && mp[now][R.get_y() - 1][R.get_x()] == 12) //河
					{
						talking = 1;
						wtext = 13;
					}
				 }
				else if (model == 3)
					 {
						if (b_now != -1 && b_next[b_now][0] != 0)
						{
							b_now = b_next[b_now][0];
							if (b_now != -1) b_now = spj(b_now); //若即将退出则不能再调用判断函数

							if (b_now == 13) change(13); //对方攻击
							if (b_now == 13) show1 = 0;
						}
						if (b_now == -1)
						{
							model = 1;
							PlaySound(L"2.wav", NULL, SND_FILENAME | SND_ASYNC);
							talking = 0;
							b_now = 0;
							delete tmp_p;
						}
					 }
			InvalidateRect(hWnd, NULL, true);
			break;
		case 80: //P
			if (model == 1) model = 2;
			InvalidateRect(hWnd, NULL, true);
			break;
		case 66: //B
			if (model == 1 && mp[now][R.get_y()][R.get_x()] == 9)
			{
				model = 3;
				PlaySound(L"3.wav", NULL, SND_FILENAME | SND_ASYNC);
				talking = 1;
				b_now = 1;
				tmp_p=create(11);
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		case 90: //Z
			if (M.node == 1 && model == 2) model = 1;
			if (M.father[M.node] != 0)
			{
				M.node = M.father[M.node];
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		case 49: //1
			if (model == 3)
			{
				if (b_now != -1 && b_next[b_now][1] != 0)
				{
					b_now = b_next[b_now][1];
					change(b_now);
				}
				if (b_now == 7) show1 = 0;
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		case 50: //2
			if (model == 3)
			{
				if (b_now != -1 && b_next[b_now][2] != 0)
				{
					b_now = b_next[b_now][2];
					change(b_now);
				}
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		case 51: //3
			if (model == 3)
			{
				if (b_now != -1 && b_next[b_now][3] != 0)
				{
					b_now = b_next[b_now][3];
				}
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		case 89:
			yyy++;
			if (yyy == 3)
			{
                yyy = 0;
				talking = 1;
				wtext = 4;
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		case VK_UP:
			if (model==1)
			{
				if (R.get_y() > 0 && R.get_dir() == 1 && mp[now][R.get_y() - 1][R.get_x()] == 4) //进家门
				{
					now = 4;
					R.set_dir(1);
					R.set_x(9);
					R.set_y(11);
				}
				else
				if (R.get_y() > 0 && R.get_dir() == 1 && mp[now][R.get_y() - 1][R.get_x()] == 5) //进实验室
				{
					now = 5;
					R.set_dir(1);
					R.set_x(9);
					R.set_y(14);
				}
				else
				if (R.get_y() > 0 && R.get_dir() == 1 && mp[now][R.get_y() - 1][R.get_x()] == 2) //进医院
				{
					now = 2;
					R.set_dir(1);
					R.set_x(9);
					R.set_y(9);
				}
				else if (R.get_dir() != 1) R.set_dir(1);
					 else R.set_y(R.get_y() - 1);
				InvalidateRect(hWnd, NULL, true);
			}
			if (model == 2)
			{
				M.pos[M.node].hs--;
				if (M.pos[M.node].hs == 0) M.pos[M.node].hs = 6;
				InvalidateRect(hWnd, NULL, true);
			}
			break;
		case VK_DOWN:
			if (model == 1)
			{
				if (now==5 && R.get_dir() == 2 && mp[now][R.get_y()][R.get_x()] == 102) //出实验室
				{
					now = 0;
					R.set_dir(2);
					R.set_x(14);
					R.set_y(13);
				}
				else
				if (now==4 && R.get_y() < 18 && R.get_dir() == 2 && mp[now][R.get_y() + 1][R.get_x()] == 2) //出家门
				{
					if (lock1 == 1)
					{
						now = 0;
						R.set_dir(2);
						R.set_x(2);
						R.set_y(13);
					}
					else
					{
						talking = 1;
						wtext = 10;
					}
				}
			    else
				if (R.get_y() < 18 && R.get_dir() == 2 && mp[now][R.get_y() + 1][R.get_x()] == 6) //出医院
				{
					now = 0;
					R.set_dir(2);
					R.set_x(9);
					R.set_y(4);
				}
				else if (R.get_dir() != 2) R.set_dir(2);
				else R.set_y(R.get_y() + 1);
				InvalidateRect(hWnd, NULL, true);
			}
			if (model == 2)
			{
				M.pos[M.node].hs++;
				if (M.pos[M.node].hs == 7) M.pos[M.node].hs = 1;
				InvalidateRect(hWnd, NULL, true);
			}
			break;
		case VK_LEFT:
			if (model == 1)
			{
				if (R.get_dir() == 3 && mp[now][R.get_y()][R.get_x()] == 100) //出101道路
				{
					tmp_x = R.get_x();
					tmp_y = R.get_y();
					now = 0;
					R.set_dir(3);
					R.set_x(18);
					R.set_y(tmp_y);
				}
				else if (R.get_dir() != 3) R.set_dir(3);
				     else R.set_x(R.get_x() - 1);
				InvalidateRect(hWnd, NULL, true);
			}
			break;
		case VK_RIGHT: 
			if (model == 1)
			{
				if (R.get_dir() == 4 && mp[now][R.get_y()][R.get_x()] == 101) //进101道路
				{
					if (R.p[1] == NULL)
					{
						talking = 1;
						wtext = 5;
					}
					else
					{
						tmp_x = R.get_x();
						tmp_y = R.get_y();
						now = 3;
						R.set_dir(4);
						R.set_x(0);
						R.set_y(tmp_y);
					}
				}
				else if (R.get_dir() != 4) R.set_dir(4);
				     else R.set_x(R.get_x() + 1);
				InvalidateRect(hWnd, NULL, true);
			}
			break;
		default:
			break;
		}
	}
	break;
	case WM_LBUTTONDOWN: //鼠标左键
		if (model != 0) return 0;
		mouse_x = LOWORD(lParam);//鼠标的横坐标
		mouse_y = HIWORD(lParam);
		if (mouse_x >= 195 && mouse_x <= 375 && mouse_y >= 230 && mouse_y <= 290)
		{
			talking = 1;
			wtext = 7;
			model = 1;
			PlaySound(L"2.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
		if (mouse_x >= 195 && mouse_x <= 375 && mouse_y >= 310 && mouse_y <= 370)
		{
			load();
			model = 1;
			PlaySound(L"2.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
		if (mouse_x >= 30 && mouse_x <= 150 && mouse_y >= 360 && mouse_y <= 420)
		{
			MessageBox(NULL, TEXT("通过方向键控制人物移动\n按X键可与NPC对话或触发剧情\n按P键可打开菜单栏，菜单栏中X键确认，Z键返回\n在草丛中按B键可触发战斗\n对话与战斗过程中按键方式均有文字提示\n"), TEXT("规则"), MB_OK);
		}
		InvalidateRect(hWnd, NULL, true);
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void MyPaint(HDC hdc)
{
	HBITMAP hR;
	if (model == 3)
	{
		// 血条
		per = (double)R.p[1]->get_hp() / (double)R.p[1]->full_hp;
		//tmp_s = "hp/hp5_right.bmp";
		tmp_s = "hp/hp";
		if (per <= 1 && per > 0.8) tmp_s += "5";
		if (per <= 0.8 && per > 0.6) tmp_s += "4";
		if (per <= 0.6 && per > 0.4) tmp_s += "3";
		if (per <= 0.4 && per > 0.2) tmp_s += "2";
		if (per <= 0.2 && per > 0) tmp_s += "1";
		if (per == 0) tmp_s += "0";
		tmp_s += "_right.bmp";
		hR = (HBITMAP)LoadImage(NULL, stringToLPCWSTR(tmp_s), IMAGE_BITMAP, 200, 60, LR_LOADFROMFILE);
		SelectObject(mdc, hR);
		BitBlt(hdc, 360, 200, 200, 60, mdc, 0, 0, SRCCOPY);

		per = (double)tmp_p->get_hp() / (double)tmp_p->full_hp;
		tmp_s = "hp/hp";
		if (per <= 1 && per > 0.8) tmp_s += "5";
		if (per <= 0.8 && per > 0.6) tmp_s += "4";
		if (per <= 0.6 && per > 0.4) tmp_s += "3";
		if (per <= 0.4 && per > 0.2) tmp_s += "2";
		if (per <= 0.2 && per > 0) tmp_s += "1";
		if (per == 0) tmp_s += "0";
		tmp_s += "_left.bmp";
		hR = (HBITMAP)LoadImage(NULL, stringToLPCWSTR(tmp_s), IMAGE_BITMAP, 200, 60, LR_LOADFROMFILE);
		SelectObject(mdc, hR);
		BitBlt(hdc, 10, 55, 200, 60, mdc, 0, 0, SRCCOPY);

		tmp_s = "No." + to_string(R.p[1]->get_ID()) + "_right.bmp";
		//tmp_s = "No.4_right.bmp";
		hR = (HBITMAP)LoadImage(NULL, stringToLPCWSTR(tmp_s), IMAGE_BITMAP, 360, 180, LR_LOADFROMFILE);
		SelectObject(mdc, hR);
		BitBlt(hdc, 0, 130, 360, 180, mdc, 0, 0, SRCCOPY);

		tmp_s = "No." + to_string(tmp_p->get_ID()) + "_left.bmp";
		hR = (HBITMAP)LoadImage(NULL, stringToLPCWSTR(tmp_s), IMAGE_BITMAP, 360, 180, LR_LOADFROMFILE);
		SelectObject(mdc, hR);
		BitBlt(hdc, 210, -10, 360, 180, mdc, 0, 0, SRCCOPY);

		DeleteObject(hR);
		return;
	}
	if (model == 2)
	{
		hR = (HBITMAP)LoadImage(NULL, L"ball.bmp", IMAGE_BITMAP, 37, 36, LR_LOADFROMFILE);
		SelectObject(mdc, hR);
		BitBlt(hdc, 60 ,60 * M.pos[M.node].hs , 37, 36, mdc, 0, 0, SRCCOPY);
		if (M.node == 3)
		{
			for (int i = 1; i <= 6; i++)
			{
				if (R.p[i] == NULL) continue;
				hR = (HBITMAP)LoadImage(NULL, L"my_pokemon1.bmp", IMAGE_BITMAP, 180, 250, LR_LOADFROMFILE);
				SelectObject(mdc, hR);
				BitBlt(hdc, 120+4, 60*i+4, 55, 37, mdc, pposy[R.p[i]->get_ID()]*60+5, 50*pposx[R.p[i]->get_ID()]+5, SRCCOPY);//60 50
				tmp_s = R.p[i]->get_info();
				TextOut(hdc, 180, 60*i+15, stringToLPCWSTR(tmp_s), wcslen(stringToLPCWSTR(tmp_s)));
			}
		}
		DeleteObject(hR);
		return;
	}
	if (model != 1) return;
	switch (R.get_dir())
	{
        case 1:
            hR = (HBITMAP)LoadImage(NULL, L"back1.bmp", IMAGE_BITMAP, 60, 38, LR_LOADFROMFILE);
			break;
		case 2:
			hR = (HBITMAP)LoadImage(NULL, L"front1.bmp", IMAGE_BITMAP, 60, 38, LR_LOADFROMFILE);
			break;
		case 3:
			hR = (HBITMAP)LoadImage(NULL, L"left1.bmp", IMAGE_BITMAP, 60, 38, LR_LOADFROMFILE);
			break;
		case 4:
			hR = (HBITMAP)LoadImage(NULL, L"right1.bmp", IMAGE_BITMAP, 60, 38, LR_LOADFROMFILE);
			break;
		default: 
			hR = (HBITMAP)LoadImage(NULL, L"front1.bmp", IMAGE_BITMAP, 60, 38, LR_LOADFROMFILE);
	}
	SelectObject(mdc, hR);

	BitBlt(hdc, 30 * R.get_x(), 30 * R.get_y() - 8, 30, 38, mdc, 30, 0, SRCAND);
	BitBlt(hdc, 30 * R.get_x(), 30 * R.get_y() - 8, 30, 38, mdc, 0, 0, SRCPAINT);

	DeleteObject(hR);
}

void MyPaint2(HDC hdc)
{
	HBITMAP hR;
	switch (R.get_dir())
	{
	case 1:
		hR = (HBITMAP)LoadImage(NULL, L"back1.bmp", IMAGE_BITMAP, 60, 38, LR_LOADFROMFILE);
		addx = 0; addy = 15;
		break;
	case 2:
		hR = (HBITMAP)LoadImage(NULL, L"front1.bmp", IMAGE_BITMAP, 60, 38, LR_LOADFROMFILE);
		addx = 0; addy = -15;
		break;
	case 3:
		hR = (HBITMAP)LoadImage(NULL, L"left1.bmp", IMAGE_BITMAP, 60, 38, LR_LOADFROMFILE);
		addx = 15; addy = 0;
		break;
	case 4:
		hR = (HBITMAP)LoadImage(NULL, L"right1.bmp", IMAGE_BITMAP, 60, 38, LR_LOADFROMFILE);
		addx = -15; addy = 0;
		break;
	default:
		hR = (HBITMAP)LoadImage(NULL, L"front1.bmp", IMAGE_BITMAP, 60, 38, LR_LOADFROMFILE);
		addx = 0; addy = 0;
	}
	SelectObject(mdc, hR);

	BitBlt(hdc, 30 * R.get_x() + addx, 30 * R.get_y() - 8 + addy, 30, 38, mdc, 30, 0, SRCAND);
	BitBlt(hdc, 30 * R.get_x() + addx, 30 * R.get_y() - 8 + addy, 30, 38, mdc, 0, 0, SRCPAINT);

	DeleteObject(hR);
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

pokemon* create(int ID)
{
	pokemon *ret;
	switch (ID)
	{
		case 4:
			ret = new pokemon("Charmander", 5, 100, ID);
			break;
		case 1:
			ret = new pokemon("Bulbasaur", 5, 100, ID);
			break;
		case 7:
			ret = new pokemon("Squirtle", 5, 100, ID);
			break;
		case 11:
			ret = new pokemon("Rattata", 5, 100, ID);
			break;
		default:
			ret = new pokemon("Rookie", 5, 100, ID);
	}
	//名字 等级 血量 ID
	return ret;
}
pokemon* create(string nn, int ll, int hh, int ii)
{
	pokemon* ret;
	ret = new pokemon(nn, ll, hh, ii);
	return ret;
}
int spj(int xx) //战斗情况判定
{
	my = R.p[1];
	enemy = tmp_p;
	if (enemy->get_hp() == 0) return 15;
	if (my->get_hp() == 0) return 17;
	return xx;
}

void change(int xx) //改变血量信息
{
	int tmp = 0;
	my = R.p[1];
	enemy = tmp_p;
	switch (xx)
	{
	case 7: //我方攻击
		tmp = enemy->get_hp() - my->get_level() * 4;
		if (tmp < 0) tmp = 0;
		enemy->set_hp(tmp);
		break;
	case 9: //我方回复
		tmp = my->get_hp() + my->get_level() * 3;
		if (tmp > my->full_hp) tmp = my->full_hp;
		my->set_hp(tmp);
		break;
	case 13: //敌方攻击
		tmp = my->get_hp() - enemy ->get_level() * 4;
		if (tmp < 0) tmp = 0;
		my->set_hp(tmp);
		break;
	}
}

void save() //保存
{
	FILE *fp;
	fp=fopen("save.txt", "w");
	fprintf(fp, "%d %d %d %d\n", now, R.get_x(), R.get_y(), R.get_dir());
	for (int i = 1; i <= 6; i++)
	{
		if (R.p[i] == NULL) continue;
		fprintf(fp, "%s ", R.p[i]->get_name().c_str());
		fprintf(fp, "%d %d %d\n", R.p[i]->get_level(), R.p[i]->get_hp(), R.p[i]->get_ID());
	}
	fclose(fp);
}

void load() //载入
{
	freopen("save.txt", "r", stdin);
	cin >> now >> tmp_x >> tmp_y >> tmp_dir;
	R.set_x(tmp_x);
	R.set_y(tmp_y);
	R.set_dir(tmp_dir);
	lastx = tmp_x;
	lasty = tmp_y;
	while (cin>>tmp_s)
	{
		cin >> tmp_level >> tmp_hp >> tmp_ID;
		R.add_pokemon(create(tmp_s,tmp_level,tmp_hp,tmp_ID));
	}
	fclose(stdin);
}