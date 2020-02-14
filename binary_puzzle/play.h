/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/29                                                     **/
/** Description: 数独游戏相关函数头文件                        **/
/**********************************************************/

#pragma once
#include"puzzle.h"
#include"tool_function.h"

/**
@brief: 数独游玩模块主程序
@param p: 棋盘
*/
void puzzlePlay(struct Puzzle p)
{
	printf("/*待解答棋盘:\n");
	draw(p);

	info.num_literal = puzzle_size * puzzle_size;
	int flag = false;
	struct Puzzle p_ori = p;
	struct Formula* formula = transform(p);

	while (hasVacant(p)) {
		int* input = getPlayInput();
		if (input[2] != -1) {
			applyInput(&p, input);
			draw(p);
		}
		else {
			flag = true;
			break;
		}
	}
	if (!flag) {
		if (checkValid(p, formula)) printf("/*解答正确\n");
		else {
			printf("/*解答错误\n");
		}
	}
	else solvePuzzle(p_ori, false);
}