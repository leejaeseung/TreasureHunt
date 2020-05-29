#pragma once
//게임 실행시 필요한 함수들
#include "subFunc.h"
#include "art.h"

int setMap(int** map, int** flagInfo, int* flagCnt, int obsCnt, int obsLeng) {
	int x, y, fNum;
	int createTreasure = 0;
	int treasureIdx = 0;
	int mv1[4] = { 0, 0, 1, -1 };
	int mv2[4] = { 1, -1, 0, 0 };
	int dir;

	for (int i = 0; i < obsCnt; i++)
	{
		x = (rand() % (ROW - 2 - SROW)) + 1 + SROW;
		y = (rand() % (COL - 2 - SCOL)) + 1 + SCOL;
		if ((x == SX && y == SY + 1) || (x == SX && y == SY)) {
			i--;
			continue;
		}

		map[x][y] = -1;
		dir = rand() % 4;

		for (int j = 1; j < obsLeng; j++)
		{
			x = x + mv1[dir];
			y = y + mv2[dir];

			if ((x == SX && y == SY + 1) || (x == SX && y == SY) || x >= ROW || y >= COL || x <= SROW || y <= SCOL) {
				break;
			}
			map[x][y] = -1;
		}
	}

	for (int i = 0; i < *flagCnt; i++)
	{
		x = (rand() % (ROW - 3 - SROW)) + 1 + SROW;
		y = (rand() % (COL - 4 - SCOL)) + 1 + SCOL;
		fNum = (rand() % 4) + 1;

		if ((x == SX && y == SY + 1) || (x == SX && y == SY) || (createTreasure == 1 && fNum == 4)
			|| (fNum != 4 && i == (*flagCnt) - 1 && createTreasure == 0) || map[x][y] != -2 || map[x][y + 1] != -2) {
			//깃발이 시작 위치거나, 보물이 두 개이상 생성되거나, 보물이 생성되지 않았을 경우, 이미 깃발이 있을경우
			i--;
			continue;
		}
		if (fNum == 4) {
			createTreasure = 1;
			treasureIdx = i;
		}
		//깃발 정보를 저장.
		map[x][y] = i;
		map[x][y + 1] = i;
		flagInfo[i][0] = x;
		flagInfo[i][1] = y;
		flagInfo[i][2] = fNum;
		flagInfo[i][3] = 1;
	}

	return treasureIdx;
}

void create_map(int** map, int** flagInfo, int* flagCnt, int* treasureIdx, int obsCnt, int obsLeng) {
	//게임 시작 시 초기 설정 및 게임 시작

	erase_console();
	erase_map(map);

	*treasureIdx = setMap(map, flagInfo, flagCnt, obsCnt, obsLeng);
	//깃발, 장애물 생성, treasureIdx == flagInfo에서 보물의 index
	make_side(map);
	//맵 테두리 출력
	print_map(map, flagInfo, flagCnt);
	//깃발, 장애물 출력
}

void print_info(const int* score, const int leftMove, const int level, int x, int y) {
	gotoxy(x++, y);
	printf("┌-----------------┬-------------┐");
	gotoxy(x++, y);
	printf("ㅣ SCORE : %6d ㅣ MOVES : %3dㅣ", *score, leftMove);
	gotoxy(x++, y);
	printf("├-----------------┼-------------┘");
	gotoxy(x++, y);
	switch (level) {
	case 0:
		printf("ㅣ LEVEL :   EASY ㅣ");
		break;
	case 1:
		printf("ㅣ LEVEL : NORMAL ㅣ");
		break;
	case 2:
		printf("ㅣ LEVEL :   HARD ㅣ");
		break;
	}

	gotoxy(x++, y);
	printf("└-----------------┘");
	gotoxy(x++, y);
	printf("Press 'r' or 'R' to Reset and 'q' or 'Q' to Menu");
}


void flag1(int** flagInfo, const int* flagCnt, const int* treasureIdx, int* score, int cnt) {
	//1번 깃발을 먹으면 보물 깃발을 포함한 3,5,7개의 깃발의 방향이 바뀐다.(한 번만 사용 가능), 다시 사용 시 50%확률로 점수가 2배 or 1/2배
	if (flagInfo[*treasureIdx][3] == 2) {
		if (rand() % 2 == 0) {
			*score *= 2;
		}
		else {
			*score /= 2;
		}
		return;
	}

	gotoxy(flagInfo[*treasureIdx][0], flagInfo[*treasureIdx][1]);
	flagInfo[*treasureIdx][3] = 2;
	printf("%s", HE);

	for (int i = 0; i < *flagCnt; i++)
	{
		if (flagInfo[i][3] == 1) {
			gotoxy(flagInfo[i][0], flagInfo[i][1]);
			flagInfo[i][3] = 2;
			printf("%s", HE);
			if (--cnt)	continue;
			break;
		}
	}
}

void flag2(int* x, int* y) {
	//2번 깃발을 먹으면 처음 시작 위치로 돌아간다.
	*x = SX;
	*y = SY;
}

void flag3(const int** map, const int** flagInfo, int* flagCnt, const int* treasureIdx, int* x, int* y, const int leftFlag, const int obsCnt, const int obsLeng) {
	//3번 깃발을 남아있는 깃발, 장애물의 위치가 전부 바뀌고, 플레이어는 시작 지점으로 돌아간다.(단, 이동횟수는 그대로)

	*x = SX;
	*y = SY;

	*flagCnt = leftFlag;

	create_map(map, flagInfo, flagCnt, treasureIdx, obsCnt, obsLeng);
}

void flag4(int* score) {
	//4번 깃발(보물)을 먹으면 점수에 500점이 추가되고 다음 단계로 넘어간다.

	*score += 500;
	success(SROW, SCOL);
}

int checkFlag(const int** map, const int** flagInfo, const int* flagCnt, const int* treasureIdx, const int* score, const int* level,
	const int* x, const int* y, const int flagNum, const int leftFlag, const int lv) {
	int ret = 0;
	switch (flagNum) {
	case 1:
		flag1(flagInfo, flagCnt, treasureIdx, score, level[3] - 1);
		break;
	case 2:
		flag2(x, y);
		;		break;
	case 3:
		flag3(map, flagInfo, flagCnt, treasureIdx, x, y, leftFlag, level[1], level[2]);
		break;
	case 4:
		flag4(score);
		return 1;
	}
	return 0;
}


int getFlag(int** map, int** flagInfo, int* leftFlag, const int x, const int y) {
	//깃발을 먹는 함수
	int ret = 0;
	if ((map[x][y] != -2 && map[x][y + 1] != -2) && map[x][y] != map[x][y + 1]) {
		//서로 다른 깃발이 양쪽에 걸쳐있는 경우
		ret = max(flagInfo[map[x][y]][2], flagInfo[map[x][y + 1]][2]);
		flagInfo[map[x][y]][3] = 0;
		flagInfo[map[x][y + 1]][3] = 0;
		map[x][y] = -2;
		map[x][y - 1] = -2;
		map[x][y + 1] = -2;
		map[x][y + 2] = -2;
		*leftFlag -= 2;

		gotoxy(x, y - 1);
		printf("    ");
	}
	else if (map[x][y + 1] == -2 && map[x][y] != -2) {
		//깃발이 왼쪽에 걸쳐있는 경우
		ret = flagInfo[map[x][y]][2];
		flagInfo[map[x][y]][3] = 0;
		map[x][y] = -2;
		map[x][y - 1] = -2;
		*leftFlag -= 1;

		gotoxy(x, y - 1);
		printf("  ");
	}
	else if (map[x][y + 1] != -2 && map[x][y] == -2) {
		//깃발이 오른쪽에 걸쳐있는 경우
		ret = flagInfo[map[x][y + 1]][2];
		flagInfo[map[x][y + 1]][3] = 0;
		map[x][y + 1] = -2;
		map[x][y + 2] = -2;
		*leftFlag -= 1;

		gotoxy(x, y + 1);
		printf("  ");
	}
	else {
		//깃발과 일치하는 경우
		ret = flagInfo[map[x][y]][2];
		flagInfo[map[x][y]][3] = 0;
		map[x][y + 1] = -2;
		map[x][y] = -2;
		*leftFlag -= 1;

		gotoxy(x, y);
		printf("  ");
	}
	return ret;
}

int play(const int** map, const int** flagInfo, int* flagCnt, const int* treasureIdx, const int* score, const int* level, const int lv) {
	//사용자의 방향키 입력에 따라 게임 진행
	int sx = SX, sy = SY;
	int ch;
	int leftFlag = *flagCnt;
	int leftMove = level[4];

	while (1) {
		print_info(score, leftMove, lv, ROW, SCOL);

		int tsx = sx;
		int tsy = sy;

		gotoxy(sx, sy);
		printf("%s", UE);

		ch = _getch();
		if (ch == 224 && leftMove > 0)
			ch = _getch();
		else if (ch == 'r' || ch == 'R' || leftMove == 0) {
			*flagCnt = level[0];
			create_map(map, flagInfo, flagCnt, treasureIdx, level[1], level[2]);
			leftMove = level[4];
			sx = SX;
			sy = SY;
			continue;
		}
		else if (ch == 'q' || ch == 'Q') {
			return 2;
		}
		else
			continue;
		switch (ch) {
		case 72:
			sx--;
			break;
		case 80:
			sx++;
			break;
		case 75:
			sy--;
			break;
		case 77:
			sy++;
			break;
		}
		if (map[sx][sy] == -1 || map[sx][sy + 1] == -1) {
			//벽으로 가로막혀 있을 때
			sx = tsx;
			sy = tsy;
			continue;
		}
		else if (map[sx][sy] != -2 || map[sx][sy + 1] != -2) {
			//깃발이 존재할 때
			int flagNum = getFlag(map, flagInfo, &leftFlag, sx, sy);
			//깃발의 종류 판별
			gotoxy(sx, sy);
			if (checkFlag(map, flagInfo, flagCnt, treasureIdx, score, level, &sx, &sy, flagNum, leftFlag, lv) == 1)
				return 1;
			//깃발에 대한 수행 return 값이 1 -> 보물을 찾음.
		}
		leftMove--;

		gotoxy(tsx, tsy);
		printf("  ");
	}
	return 0;
}

int choose_menu(const int menu_pos[3][2]) {
	int choice = 0;
	int x = menu_pos[choice][0];
	int y = menu_pos[choice][1];
	int ch;

	while (1) {
		int tx = x;
		int ty = y;

		gotoxy(x, y);
		printf("%s", ME);

		ch = _getch();
		if (ch == 224)
			ch = _getch();
		else if (ch == 'q' || ch == 'Q') {
			//메뉴에서 종료
			exit(0);
		}
		else if (ch == 13) {
			return choice;
		}
		else
			continue;
		switch (ch) {
		case 72:
			choice = (choice + 2) % 3;
			break;
		case 80:
			choice = (choice + 1) % 3;
			break;
		}
		x = menu_pos[choice][0];
		y = menu_pos[choice][1];

		gotoxy(tx, ty);
		printf("  ");
	}
}