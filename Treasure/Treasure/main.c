#include "func.h"
#include "subFunc.h"

int main(void) {
	system("mode con cols=120 lines=32");
	srand(time(NULL));
	cursor(0);

	const int menu_pos[3][2] = { {M1X, M1Y}, {M2X, M2Y}, {M3X, M3Y} };	//�޴����� ��ǥ
	const int level[3][5] = { {EASY_FLAG, EASY_OBS, EASY_OBS_LENG, EASY_FLAG_HINT, EASY_MOVE},
								{NORMAL_FLAG, NORMAL_OBS, NORMAL_OBS_LENG, NORMAL_FLAG_HINT, NORMAL_MOVE},
								{HARD_FLAG, HARD_OBS, HARD_OBS_LENG, HARD_FLAG_HINT, HARD_MOVE} };			//���̵� ����
	int score = 0, highScore = 0, lv, obsCnt, obsLeng, treasureIdx, flagCnt, playResult;
	int** map;
	int** flagInfo;


	while (1) {

		print_menu();
		gotoxy(31, 50);
		printf("High Score : %d", max(highScore, score));
		lv = choose_menu(menu_pos);

		score = 0;

		while (lv < 3) {

			flagCnt = level[lv][0];
			obsCnt = level[lv][1];
			obsLeng = level[lv][2];

			map = init_map();	// -2 = ����, -1 = ��(��ֹ�), 1 = ���1, 2 = ���2, 3 = ���3, 4 = ����
			flagInfo = init_flagInfo(flagCnt);		//��ߵ��� x,y ��ǥ, ���, �������� ���� ����

			create_map(map, flagInfo, &flagCnt, &treasureIdx, obsCnt, obsLeng);

			playResult = play(map, flagInfo, &flagCnt, &treasureIdx, &score, level[lv], lv);
			if (playResult == 0)	continue;

			free_all(map, flagInfo, level[lv][0]);
			erase_console();
			if (playResult == 1) {
				//���� ���� -> clear �� ���� �ܰ��
				lv++;
			}
			else if (playResult == 2) {
				break;
			}
		}
	}

	return 0;
}