#include <easyx.h>
#include <time.h>
#include <conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<graphics.h>
using namespace std;

//函数声明
void GameInit();
void Paint();
void gamePaint();
void InitGame();
void Run();
void GameMDown(int mouseX, int mouseY);
void GameMUp(int mouseX, int mouseY);
void rebort();
int IsInside(int a, int b);
void GameReturnNoraml();
void Gem_Clear();
void Transmit_Gem();
int Can_Clear();
void Clear();
int dfs(int line, int column, int num);
void Append();
void append1(int line, int column);
//主函数入口
int main() {
	initgraph(640, 480, SHOWCONSOLE);
	InitGame();
	Paint();
	flushmessage();
	Run();
	closegraph();
	return 0;
}

//变量声明
TCHAR path[100];
IMAGE img,imgs[7][15];
typedef enum {
	GAME_NORMAL,
	SELECT_ONE,
	SELECT_TWO,
	CLEAR,
	APPEND
} GAME_STATE;
typedef enum {
	GEM_NORMAL,
	SELECTED_ONE,
	SELECTED_TWO,
	TO_CLEAR,
} GEM_STATE;
typedef struct {
	GEM_STATE State;
	int Type;
	int ImageNum;
} GEM;
GAME_STATE gamestate;
GEM gems[8][8];
GEM gemsTemp[8][8];
int level = 5;
int line1, line2, column1, column2;
int dir[4][2] = { {0,1},{1,0},{0,-1},{-1,0} };

//函数实现
void InitGame() {
	srand((unsigned)time(NULL));
	setbkcolor(NULL);
	setbkmode(TRANSPARENT);
	GameInit();
}
void GameInit() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			gems[i][j].ImageNum = 0;
			gems[i][j].State = GEM_STATE::GEM_NORMAL;
			gems[i][j].Type = rand() % level;
			gemsTemp[i][j].Type = -1;
		}
	}
	line1 = line2 = column1 = column2 = -1;
	gamestate = GAME_NORMAL;
	loadimage(&img,_T(".\\images\\back.png"));
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 15; j++) {
			swprintf_s(path, L".\\images\\%d\\%d.png", i, j);
			loadimage(&imgs[i][j], path);
		}
	}
}
void Paint() {
	BeginBatchDraw();
	cleardevice();
	gamePaint();
	EndBatchDraw();
}
void gamePaint() {
	putimage(0, 0, &img);
	outtextxy(10, 10, L"hello");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			putimage(200 + j * 52, 10 + i * 52, &imgs[gems[i][j].Type][gems[i][j].ImageNum]);
		}
	}
}
void Run() {
	ExMessage m;
	while (1) {
		if (peekmessage(&m, EM_MOUSE, false)) {
			m = getmessage();
			if (m.message == WM_LBUTTONDOWN) {
				GameMDown(m.x,m.y);
				Paint();
				continue;
			}
			else if (m.message == WM_LBUTTONUP) {
				GameMUp(m.x,m.y);
				Paint();
				continue;
			}
		}
		rebort();
		Paint();
		Sleep(10);
	}
}
void GameMDown(int mouseX, int mouseY) {
	if (mouseX > 24 && mouseY > 143 && mouseX < 168 && mouseY < 175) {
		InitGame();
		return;
	}
	if (gamestate == GAME_NORMAL) {
		line1 = (mouseY - 10) / 52;
		column1 = (mouseX - 200) / 52;
		if (IsInside(line1, column1)) {
			gamestate = SELECT_ONE;
			gems[line1][column1].State = GEM_STATE::SELECTED_ONE;
		}
		else {
			GameReturnNoraml();
		}
	}
	else if (gamestate == SELECT_ONE) {
		line2 = (mouseY - 10) / 52;
		column2 = (mouseX - 200) / 52;
		if (IsInside(line2, column2)) {
			if (((line1==line2)&&(column1==column2 - 1))&&((line1 == line2) && (column1 == column2 + 1))&& ((line1 == line2 - 1) && (column1 == column2)) && ((line1 == line2 + 1) && (column1 == column2))) {
				GameReturnNoraml();
			}
			else {
				gamestate = SELECT_TWO;
				gems[line2][column2].State = SELECTED_TWO;
				Transmit_Gem();
				Gem_Clear();
			}
		}
		else {
			GameReturnNoraml();
		}
	}
		
		//gamestate = SELECT_ONE;
	
}
void GameMUp(int mouseX, int mouseY) {
	printf("mouse up from : %d %d\n", mouseX, mouseY);
}
void rebort() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((line1 != i && column1 != j) && (line2 != i && column2 != j)) {
				gems[i][j].ImageNum = 0;
			}
		}
	}
	if (line1 != -1 && column1 != -1) {
		gems[line1][column1].ImageNum++;
		if (gems[line1][column1].ImageNum > 14) {
			gems[line1][column1].ImageNum = 0;
		}
	}
	if (line2 != -1 && column2 != -1) {
		gems[line2][column2].ImageNum++;
		if (gems[line2][column2].ImageNum > 14) {
			gems[line2][column2].ImageNum = 0;
		}
	}
}
int IsInside(int a,int b) {
	return a >= 0 && a < 8 && b >= 0 && b < 8;
}
void GameReturnNoraml() {
	line1 = line2 = column1 = column2 = -1;
	gamestate = GAME_NORMAL;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			gems[i][j].ImageNum = 0;
			gems[i][j].State = GEM_STATE::GEM_NORMAL;
		}
	}
}
void Gem_Clear() {
	if (Can_Clear()) {
		gamestate = CLEAR;
		Clear();
		gamestate = APPEND;
		Append();
		GameReturnNoraml();
	}
	else {
		Transmit_Gem();
		GameReturnNoraml();
	}
}
void Transmit_Gem() {
	GEM g;
	g = gems[line1][column1];
	gems[line1][column1] = gems[line2][column2];
	gems[line2][column2] = g;
}
int Can_Clear() {
	int x1, y1, x2, y2;
	x1 = x2 = line1;
	y1 = column1 - 1;
	y2 = column1 + 1;
	bool a = (gems[line1][column1].Type == gems[x1][y1].Type) && (gems[line1][column1].Type == gems[x2][y2].Type);
	y1 = y2 = column1;
	x1 = line1 - 1;
	x2 = line1 + 1;
	bool b = (gems[line1][column1].Type == gems[x1][y1].Type) && (gems[line1][column1].Type == gems[x2][y2].Type);
	x1 = x2 = line2;
	y1 = column2 - 1;
	y2 = column2 + 1;
	bool c = (gems[line2][column2].Type == gems[x1][y1].Type) && (gems[line2][column2].Type == gems[x2][y2].Type);
	y1 = y2 = column2;
	x1 = line2 - 1;
	x2 = line2 + 1;
	bool d = (gems[line2][column2].Type == gems[x1][y1].Type) && (gems[line2][column2].Type == gems[x2][y2].Type);
	if (a || b || c || d) {
		return 1;
	}
	return 0;
}
void Clear() {
	gems[line1][column1].State = GEM_STATE::TO_CLEAR;
	int num1 = 1, num2 = 1;
	num1=dfs(line1, column1, num1);
	if (num1 >= 3) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (gems[i][j].State == GEM_STATE::TO_CLEAR) {
					gems[i][j].Type = -1;
				}
				else {
					continue;
				}
			}
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (gems[i][j].State == TO_CLEAR) {
					gems[i][j].State = GEM_NORMAL;
				}
			}
		}
	}
	gems[line2][column2].State = GEM_STATE::TO_CLEAR;
	num2=dfs(line2, column2, num2);
	printf("%d %d\n", num1, num2);
	if (num2 >= 3) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (gems[i][j].State == GEM_STATE::TO_CLEAR) {
					gems[i][j].Type = -1;
				}
				else {
					continue;
				}
			}
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (gems[i][j].State == TO_CLEAR) {
					gems[i][j].State = GEM_NORMAL;
				}
			}
		}
	}
}
int dfs(int line, int column, int num) {
	int n_line, n_column;
	for (int i = 0; i < 4; i++) {
		n_line = line + dir[i][0];
		n_column = column + dir[i][1];
		if (gems[n_line][n_column].Type == gems[line][column].Type && gems[n_line][n_column].State != GEM_STATE::TO_CLEAR) {
			num++;
			gems[n_line][n_column].State = GEM_STATE::TO_CLEAR;
			num = dfs(n_line, n_column, num);
		}
	}
	return num;
}
void Append() {
	for (int j = 0; j < 8; j++) {
		for (int i = 7; i >=0; i--) {
			if (gems[i][j].Type == -1) {
				append1(i,j);
			}
		}
	}
}
void append1(int line, int column) {
	for (int j = 0; j < 8; j++) {
		for (int i = line; i >= 1; i--) {
			gems[i][column] = gems[i - 1][column];
		}
	}
	gems[0][column].Type = -1;
}
