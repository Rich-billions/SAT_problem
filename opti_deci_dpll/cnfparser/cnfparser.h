/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/01                                                     
//* Description: cnf解析优化模块相关头文件		                          
/**********************************************************/

#pragma once
#include"../solver/tool_functions.h"
#include"../../naive_dpll/cnfparser/cnfparser.h"


/**
@brief: 从cnf文件中读取数据构建公式(改进版1)
@param filename: cnf文件名
@param counter: 指向计数器数组的指针
@return: 构建完成的公式
*/
struct Formula* loadFile_opti1(const char* filename, int** counter)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("无法读取文件:%s", filename);
		return NULL;
	}
	else {
		short num = 0;
		int  i = 1;
		struct Formula* formula = initFormula();		//初始化公式
		skip(fp);	//跳过文件注释行
		*counter = initCounter();

		struct Clause* clause = createClause(formula);	//特别处理:添加第一个子句
		while (!feof(fp) and (i <= info.num_clause)) {
			fscanf(fp, "%hd", &num);
			if (num == 0 and i < info.num_clause) {		//遇到行尾，新建一个子句
				clause = createClause(formula);
				i++;
				continue;
			}
			else if (num != 0) {		//给当前子句添加一个文字
				addLiteral(clause, num);
				(*counter)[counterIndex(num)] ++;
			}
			else break;		//读取输入完毕
		}

		fclose(fp);
		return formula;
	}
}