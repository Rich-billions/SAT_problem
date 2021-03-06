/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: 主控模块相关头文件		                          
/**********************************************************/

#pragma once
#include"./debug.h"

#include"../binary_puzzle/play.h"
#include"../naive_dpll/solver/solver.h"
#include"../opti_deci_dpll/solver/solver.h"
#include"../opti_memo_dpll/solver/solver.h"

//模式状态
//MAIN: 主界面(选择模式界面), 
//CNF:CNF求解模式, 
//PUZZLE: 求解数独模式
//PUZZLE_PLAY: 游玩数独模式
//DEBUG: DEBUG模式(输出验证信息)
//ESC: 退出程序
enum g_mode
{
	MAIN, CNF, PUZZLE, PUZZLE_PLAY, DEBUG, ESC
} mode;


/**
@brief: 打印cnf解答器版本
*/
void printSolverVer()
{
	if (cnf_solver_version == 0) printf("/*正在使用最初版本解答器\n");
	else if (cnf_solver_version == 1) printf("/*正在使用决策优化版解答器\n");
	else printf("/*正在使用内存使用优化版解答器\n");
}


/**
@brief: 打印说明界面
*/
void printGuide()
{
	printf("/**********************************************************************************/\n");
	printf("/*                                    欢迎                                        */\n");
	printf("/*                     此程序可用于求解CNF范式和求解数独                          */\n");
	printf("/*               按下c键进入求解CNF范式模式, 按下p键进入求解数独模式              */\n");
	printf("/*             可通过按下ESC键返回模式选择界面, 再次按下ESC键退出程序             */\n");
	printf("/**********************************************************************************/\n");
}


/**
@brief: 将求解cnf的解答写入文件
@param filename: 写入文件的路径
@param result: 解答
*/
void cnfResultPrint(char* filename, struct Result result)
{
	FILE* fp = fopen(filename, "w");
	fprintf(fp, "%s %d\n", "s", result.isSatisfied);
	fprintf(fp, "%s ", "v");
	for (int i = 1; i <= info.num_literal; i++) {
		fprintf(fp, "%d\t", result.res[i]);
		if (i % 15 == 0) fprintf(fp, "%c", '\n');
	}
	fprintf(fp, "%c", '\n');
	fprintf(fp, "%s %f", "t", result.time);
	printf("/*解答已写入文件:%s\n", filename);
	fclose(fp);
}


/**
@brief: 显示CNF交互环境，获取用户输入
@return: 输入的文件路径名
*/
char* getCnfFileName()
{
	printf("/*请输入cnf文件路径:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
	scanf("%s", filename);
	return filename;
}


/**
@brief: 显示PUZZLE交互环境，获取用户输入
@return: 输入的文件路径名
*/
char* getPuzzleName()
{
	printf("/*请输入Puzzle棋盘文件路径:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
	scanf("%s", filename);
	return filename;
}


/**
@brief: 获得cnf求解后文件的输出路径
@param cnf_filename: cnf输入文件路径
@return: 输出路径
*/
char* cnfOutputName(char* cnf_filename)
{
	char* suffix = strstr(cnf_filename, ".cnf");
	char c[5] = { '.', 'r', 'e', 's', '\0' };
	int suffix_len = strlen(suffix);
	for (int i = 0; i < suffix_len; i++)
		suffix[i] = c[i];
	return cnf_filename;
}


//@brief: 设置模式, 打印相关信息
void modeSet(g_mode _mode)
{
	mode = _mode;
	if(mode == CNF) printf("/*进入求解CNF范式模式\n");
	if (mode == PUZZLE) printf("/*进入求解数独模式(当前数独阶数为%d)\n", puzzle_size);
	if(mode == PUZZLE_PLAY) printf("/*进入游玩数独模式(当前数独阶数为%d)\n", puzzle_size);
	if(mode == DEBUG) printf("/*进入debug模式\n");
	if(mode == ESC) printf("已退出程序\n");
}


/**
@brief: 模式选择函数
*/
void modeChange()
{
	mode = MAIN;
	printf("/*请进行模式选择(按键选择: c - CNF求解模式, p - 数独求解模式, g - 数独游玩模式, d - debug模式, ESC退出): \n");
	char c = getch();
	if (c == 'c' or c == 'C') modeSet(CNF);
	else if (c == 'p' or c == 'P') modeSet(PUZZLE);
	else if (c == 'g' or c == 'G') modeSet(PUZZLE_PLAY);
	else if (c == 'd' or c == 'D') modeSet(DEBUG);
	else if (c == 27) modeSet(ESC);
	else {	printf("/*请检查按键是否错误!\n");	modeChange();	}
}


/**
@brief: 处理动态分配的空间
@param .. : 指向需要被释放空间的各个指针
*/
void handleTrash(char* filename, struct Result result, struct Formula* formula = NULL, int* counter = NULL)
{
	if(filename)	free(filename), filename = 0;
	if(result.res) free(result.res), result.res = 0;
	if (formula) destoryFormula(formula);
	if (counter) free(counter), counter = 0;
}


/**
@brief: 调用cnf求解程序接口
*/
void callCnfSolver()
{
	printSolverVer();
	char* cnf_filename = getCnfFileName();		//获取cnf输入文件路径
	struct Formula* formula = loadFile(cnf_filename);		//读取文件得到CNF公式
	if (!formula) {
		free(cnf_filename);
		return;
	}
	struct Result result = DPLL(formula);		//调用DPLL函数求解CNF公式
	cnf_filename = cnfOutputName(cnf_filename);
	cnfResultPrint(cnf_filename, result);
	handleTrash(cnf_filename, result, formula);
}


/**
@brief: 调用cnf求解模块(决策优化版本)
*/
void callCnfSolverOpti()
{
	printSolverVer();
	char* cnf_filename = getCnfFileName();
	int* counter;
	struct Formula* formula = loadFile_opti1(cnf_filename, &counter);
	if (!formula) {
		free(cnf_filename);
		return;
	}
	struct Result result = DPLLOpti(formula, counter);		//调用DPLL函数求解CNF公式
	cnf_filename = cnfOutputName(cnf_filename);
	cnfResultPrint(cnf_filename, result);
	handleTrash(cnf_filename, result, formula, counter);
}


/**
@brief: 调用cnf求解模块(内存使用优化版本)
*/
void callCnfSolverOptiX()
{
	printSolverVer();
	char* filename = getCnfFileName();
	struct BinVector formula = binVecInit();
	struct Mask mask = maskInit();
	int* counter;
	if (!loadFile(filename, &formula, &mask, &counter)) return;
	else {
		struct Result result = DPLLOptiX(&formula, &mask, counter);
		filename = cnfOutputName(filename);
		cnfResultPrint(filename, result);
		handleTrash(filename, result, NULL);
		freeBinVec(&formula);
	}
}


/**
@brief: 调用数独求解程序接口
*/
void callPuzzleSolver()
{
	char* puzzle_filename = getPuzzleName();		//获取初始棋盘文件
	struct Puzzle p;
	if(!loadPuzzleFromFile(puzzle_filename, &p)) return;		//从文件中读取棋盘
	solvePuzzle(p);		//求解数独
	free(puzzle_filename);		//释放空间
}


/**
@brief: 调用数独游玩程序接口
*/
void callPuzzlePlayer()
{
	struct Puzzle p = generatePuzzle();
	puzzlePlay(p);
}


/**
@brief: debug模式调用接口
*/
void callDebug()
{
	char* filename = getCnfFileName();
	struct Formula* formula = loadFile(filename);
	if (!formula) {
		free(filename);
		return;
	}
	struct Formula* old_formula = copyFormula(formula);
	printFormula(formula);
	struct Result result = DPLL(formula);
	printDebugInfo(result, old_formula);
	destoryFormula(old_formula);
	handleTrash(filename, result, formula);
}


/**
@brief: 主控模块主程序
*/
void display()
{
	printGuide();		//打印说明界面
	modeChange();	//模式选择
	while (mode != ESC){		//当模式不是ESC时循环
		if (mode == CNF) {
			if(cnf_solver_version == 0) callCnfSolver();		//调用CNF求解模块
			if (cnf_solver_version == 1) callCnfSolverOpti();
			if (cnf_solver_version == 2) callCnfSolverOptiX();
		}
		if (mode == PUZZLE) callPuzzleSolver();		//调用数独求解模块
		if (mode == PUZZLE_PLAY) callPuzzlePlayer();	//调用数独游玩模块
		if (mode == DEBUG) callDebug();
		printf("\n/*按下ESC键退回到模式选择, 其他按键则继续当前模式.\n");
		char c = getch();
		if (c == 27) modeChange();
	}
}