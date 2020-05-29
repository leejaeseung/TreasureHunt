#pragma once
//���� ����� �ʿ��� �Լ���
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
			//����� ���� ��ġ�ų�, ������ �� ���̻� �����ǰų�, ������ �������� �ʾ��� ���, �̹� ����� �������
			i--;
			continue;
		}
		if (fNum == 4) {
			createTreasure = 1;
			treasureIdx = i;
		}
		//��� ������ ����.
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
	//���� ���� �� �ʱ� ���� �� ���� ����

	erase_console();
	erase_map(map);

	*treasureIdx = setMap(map, flagInfo, flagCnt, obsCnt, obsLeng);
	//���, ��ֹ� ����, treasureIdx == flagInfo���� ������ index
	make_side(map);
	//�� �׵θ� ���
	print_map(map, flagInfo, flagCnt);
	//���, ��ֹ� ���
}

void print_info(const int* score, const int leftMove, const int level, int x, int y) {
	gotoxy(x++, y);
	printf("��-----------------��-------------��");
	gotoxy(x++, y);
	printf("�� SCORE : %6d �� MOVES : %3d��", *score, leftMove);
	gotoxy(x++, y);
	printf("��-----------------��-------------��");
	gotoxy(x++, y);
	switch (level) {
	case 0:
		printf("�� LEVEL :   EASY ��");
		break;
	case 1:
		printf("�� LEVEL : NORMAL ��");
		break;
	case 2:
		printf("�� LEVEL :   HARD ��");
		break;
	}

	gotoxy(x++, y);
	printf("��-----------------��");
	gotoxy(x++, y);
	printf("Press 'r' or 'R' to Reset and 'q' or 'Q' to Menu");
}


void flag1(int** flagInfo, const int* flagCnt, const int* treasureIdx, int* score, int cnt) {
	//1�� ����� ������ ���� ����� ������ 3,5,7���� ����� ������ �ٲ��.(�� ���� ��� ����), �ٽ� ��� �� 50%Ȯ���� ������ 2�� or 1/2��
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
	//2�� ����� ������ ó�� ���� ��ġ�� ���ư���.
	*x = SX;
	*y = SY;
}

void flag3(const int** map, const int** flagInfo, int* flagCnt, const int* treasureIdx, int* x, int* y, const int leftFlag, const int obsCnt, const int obsLeng) {
	//3�� ����� �����ִ� ���, ��ֹ��� ��ġ�� ���� �ٲ��, �÷��̾�� ���� �������� ���ư���.(��, �̵�Ƚ���� �״��)

	*x = SX;
	*y = SY;

	*flagCnt = leftFlag;

	create_map(map, flagInfo, flagCnt, treasureIdx, obsCnt, obsLeng);
}

void flag4(int* score) {
	//4�� ���(����)�� ������ ������ 500���� �߰��ǰ� ���� �ܰ�� �Ѿ��.

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
	//����� �Դ� �Լ�
	int ret = 0;
	if ((map[x][y] != -2 && map[x][y + 1] != -2) && map[x][y] != map[x][y + 1]) {
		//���� �ٸ� ����� ���ʿ� �����ִ� ���
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
		//����� ���ʿ� �����ִ� ���
		ret = flagInfo[map[x][y]][2];
		flagInfo[map[x][y]][3] = 0;
		map[x][y] = -2;
		map[x][y - 1] = -2;
		*leftFlag -= 1;

		gotoxy(x, y - 1);
		printf("  ");
	}
	else if (map[x][y + 1] != -2 && map[x][y] == -2) {
		//����� �����ʿ� �����ִ� ���
		ret = flagInfo[map[x][y + 1]][2];
		flagInfo[map[x][y + 1]][3] = 0;
		map[x][y + 1] = -2;
		map[x][y + 2] = -2;
		*leftFlag -= 1;

		gotoxy(x, y + 1);
		printf("  ");
	}
	else {
		//��߰� ��ġ�ϴ� ���
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
	//������� ����Ű �Է¿� ���� ���� ����
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
			//������ ���θ��� ���� ��
			sx = tsx;
			sy = tsy;
			continue;
		}
		else if (map[sx][sy] != -2 || map[sx][sy + 1] != -2) {
			//����� ������ ��
			int flagNum = getFlag(map, flagInfo, &leftFlag, sx, sy);
			//����� ���� �Ǻ�
			gotoxy(sx, sy);
			if (checkFlag(map, flagInfo, flagCnt, treasureIdx, score, level, &sx, &sy, flagNum, leftFlag, lv) == 1)
				return 1;
			//��߿� ���� ���� return ���� 1 -> ������ ã��.
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
			//�޴����� ����
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