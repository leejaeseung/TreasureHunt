#pragma once
//기호상수, 보조 함수들
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <math.h>

#define SROW 0				//맵 행 시작
#define SCOL 30				//맵 열 시작
#define ROW (25 + SROW)		//맵 행 끝
#define COL (50 + SCOL)		//맵 열 끝
#define UE "◆"		//플레이어 모양
#define FE "◀"		//깃발 모양
#define HE "▶"		//힌트 깃발 모양
#define SX (ROW - 2)	//플레이어 시작 x
#define SY (1 + SCOL)	//플레이어 시작 y
#define M1X 20		//첫 번째 메뉴 x
#define M1Y 46		//첫 번째 메뉴 y
#define M2X 22		//첫 번째 메뉴 x
#define M2Y 46		//첫 번째 메뉴 y
#define M3X 24		//첫 번째 메뉴 x
#define M3Y 46		//첫 번째 메뉴 y
#define ME "☞"		//메뉴 선택 모양

#define EASY_FLAG 10
#define EASY_OBS 15
#define EASY_OBS_LENG 3
#define EASY_FLAG_HINT 3
#define EASY_MOVE 500
#define NORMAL_FLAG 15
#define NORMAL_OBS 20
#define NORMAL_OBS_LENG 5
#define NORMAL_FLAG_HINT 5
#define NORMAL_MOVE 400
#define HARD_FLAG 20
#define HARD_OBS 25
#define HARD_OBS_LENG 7
#define HARD_FLAG_HINT 7
#define HARD_MOVE 300
//난이도 값 - 깃발 개수, 장애물 개수, 장애물 길이, 깃발 힌트 개수

void gotoxy(int x, int y) {
	COORD Cur = { y, x };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void cursor(const int n) {
	//커서 안보이게 하는 함수
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

int** init_map() {
	int** map = (int**)malloc(sizeof(int*) * ROW);
	for (int i = 0; i < ROW; i++)
	{
		map[i] = (int*)malloc(sizeof(int) * COL);
	}
	return map;
}

int** init_flagInfo(const int flagCnt) {
	int** flagInfo = (int**)malloc(sizeof(int*) * flagCnt);
	for (int i = 0; i < flagCnt; i++)
	{
		flagInfo[i] = (int*)malloc(sizeof(int) * 4);
	}
	return flagInfo;
}

void erase_console(void)
{
	for (int i = 0; i <= 31; i++) {
		gotoxy(i, 0);
		printf("%120c", ' ');
	}
	gotoxy(0, 0);
}

void print_map(int** map, int** flagInfo, int* flagCnt) {
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			gotoxy(i, j);
			//장애물들을 표시
			if (map[i][j] == -1)
				printf("#");
			else
				printf(" ");
		}
		gotoxy(i, 0);
	}
	for (int i = 0; i < *flagCnt; i++)
	{
		//깃발을 맵에 표시
		int x = flagInfo[i][0];
		int y = flagInfo[i][1];
		gotoxy(x, y);
		printf("%s", FE);
	}
}

void make_side(int** map) {
	for (int i = SROW; i < ROW; i++)
	{
		if (i != SROW && i != ROW - 1) {
			map[i][SCOL] = -1;
			map[i][COL - 1] = -1;
			continue;
		}
		for (int j = SCOL; j < COL; j++)
		{
			map[i][j] = -1;
		}

	}
}

void erase_map(int** map) {
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			map[i][j] = -2;
		}
	}
}

void free_all(int** map, int** flagInfo, int flagCnt) {
	for (int i = 0; i < flagCnt; i++)
	{
		free(flagInfo[i]);
		flagInfo[i] = NULL;
	}
	free(flagInfo);
	flagInfo = NULL;

	for (int i = 0; i < ROW; i++)
	{
		free(map[i]);
		map[i] = NULL;
	}
	free(map);
	map = NULL;
}