/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/21                                                     **/
/** Description: 工具函数定义所在头文件                        **/
/**********************************************************/

#pragma once
#include"naive_implementation/data_structure/data_struct.h"

//CNF总体信息结构体
struct ProblemInfo
{
	int num_clause;
	int num_literal;
}info;


//计数器结构体
struct Counter
{
	int positive;
	int negative;
};


/**
@brief: 初始化一个子句
@return: 指向初始化子句的指针
*/
struct Clause* initClause()
{
	struct Clause* clause = (struct Clause*) malloc(sizeof(struct Clause));			//动态分配子句空间
	if (!clause) {
		printf("malloc error.\nFileName:data_struct\nFunc:initClause\n");
		return NULL;
	}
	else {
		clause->head = (struct Literal*)malloc(sizeof(struct Literal));			//动态分配子句首尾空间
		clause->tail = (struct Literal*)malloc(sizeof(struct Literal));
		if (!clause->head or !clause->tail) {
			printf("malloc error.\nFileName:data_struct\nFunc:initClause\n");
			return NULL;
		}
		else {
			clause->head->nextLiteral = clause->tail;			//初始化子句
			clause->head->isHead = true;
			clause->head->isTail = false;
			clause->head->beforeLiteral = NULL;
			clause->tail->beforeLiteral = clause->head;
			clause->tail->isHead = false;
			clause->tail->isTail = true;
			clause->tail->nextLiteral = NULL;
			clause->len = 0;
			return clause;
		}
	}
}


/**
@brief: 初始化一个公式
@calls: initClause()
@return: 指向初始化公式的指针
*/
struct Formula* initFormula()
{
	struct Formula* formula = (struct Formula*)malloc(sizeof(struct Formula));		//动态分配公式空间
	if (!formula) {
		printf("malloc error.\nFileName:data_struct\nFunc:initFormula\n");
		return NULL;
	}
	else {
		formula->head = initClause();			//初始化公式
		formula->tail = initClause();
		formula->head->nextClause = formula->tail;
		formula->head->isFirstClause = true;
		formula->head->isLastClause = false;
		formula->head->beforeClause = NULL;
		formula->tail->beforeClause = formula->head;
		formula->tail->isFirstClause = false;
		formula->tail->isLastClause = true;
		formula->tail->nextClause = NULL;
		formula->num_clause = 0;
		return formula;
	}
}


/**
@brief: 给一个子句尾部加上一个文字
@param clause: 需要添加文字的指向子句的指针
@param data: 添加文字的序号
*/
void addLiteral(struct Clause* clause, int data)
{
	struct Literal* tl = clause->tail;
	struct Literal* temp = (struct Literal*)malloc(sizeof(struct Literal));			//动态分配文字空间
	if (temp) {
		temp->data = data;			//初始化文字
		temp->isTail = false;
		temp->isHead = false;
		tl->beforeLiteral->nextLiteral = temp;		//添加文字
		temp->beforeLiteral = tl->beforeLiteral;
		temp->nextLiteral = tl;
		tl->beforeLiteral = temp;
		clause->len++;
	}
	else {
		printf("malloc error.\n FileName:data_struct.h\nFunc:addLiteral\n");
	}
}


/**
@brief: 删除指定子句中的指定文字
@param clause: 指向指定子句的指针
@param data: 需要删除的文字的序号
*/
void deleteLiteral(struct Clause* clause, int data)
{
	struct Literal* curr = clause->head->nextLiteral;
	struct Literal* target = curr;
	while (!curr->isTail) {
		if (curr->data == data) {		//定位需要删除的文字
			target = curr;
			target->beforeLiteral->nextLiteral = target->nextLiteral;		//删除对应文字
			target->nextLiteral->beforeLiteral = target->beforeLiteral;
			curr = curr->nextLiteral;
			free(target);
			clause->len--;
		}
		else curr = curr->nextLiteral;
	}
}


/**
@brief: 给指定公式尾部创建一个子句
@param formula: 指定的公式
@return: 指向新添加子句的指针
*/
struct Clause* createClause(struct Formula* formula)
{
	struct Clause* tl = formula->tail;
	struct Clause* temp = initClause();			//初始化子句
	if (temp) {
		temp->isFirstClause = temp->isLastClause = false;		//添加子句
		tl->beforeClause->nextClause = temp;
		temp->beforeClause = tl->beforeClause;
		temp->nextClause = tl;
		tl->beforeClause = temp;
		formula->num_clause++;
		return temp;
	}
	else {
		printf("malloc error.\n FileName:data_struct.h\nFunc:addClause\n");
		return NULL;
	}
}


/**
@brief: 给公式末尾添加一个子句
@param formula: 指向公式的指针
@param clause: 指向被添加子句的指针
*/
void addClause(struct Formula* formula, struct Clause* clause)
{
	struct Clause* tl = formula->tail;
	struct Clause* temp = clause;
	if (temp) {
		temp->isFirstClause = temp->isLastClause = false;		//添加子句
		tl->beforeClause->nextClause = temp;
		temp->beforeClause = tl->beforeClause;
		temp->nextClause = tl;
		tl->beforeClause = temp;
		formula->num_clause++;
	}
	else {
		printf("malloc error.\n FileName:data_struct.h\nFunc:addClause\n");
	}
}


/**
@brief: 复制公式
@param formula: 指向被复制公式的指针
@calls: initFormula(), addClause(), addLiteral()
@return: 指向复制后公式的指针
*/
struct Formula* copyFormula(struct Formula* formula)
{
	struct Formula* formula_copy = initFormula();		//初始化公式空间
	struct Clause* curr_clause = formula->head->nextClause;
	struct Clause* curr_clause_copy = formula_copy->head;
	while (!curr_clause->isLastClause) {			//遍历复制公式
		curr_clause_copy = createClause(formula_copy);
		struct Literal* curr_literal = curr_clause->head->nextLiteral;
		while (!curr_literal->isTail) {
			addLiteral(curr_clause_copy, curr_literal->data);
			curr_literal = curr_literal->nextLiteral;
		}
		curr_clause = curr_clause->nextClause;
	}
	return formula_copy;
}


/**
@brief: 删除一个子句
@param clause: 指向被删除子句的指针
*/
void destroyClause(struct Clause* clause)
{
	struct Literal* curr = clause->tail;
	struct Literal* prev = curr->beforeLiteral;
	while (prev != NULL) {		//遍历删除文字
		free(curr);
		curr = prev;
		prev = prev->beforeLiteral;
	}
	clause->len = 0;
	free(clause->head);
}


/**
@brief: 删除公式, 归还动态分配内存
@param formula: 指向被删除公式的指针
@calls: destroyClause()
*/
void destoryFormula(struct Formula* formula)
{
	struct Clause* curr = formula->tail;
	struct Clause* prev = curr->beforeClause;
	while (prev != NULL) {		//遍历删除子句集
		destroyClause(curr);
		curr = prev;
		prev = prev->beforeClause;
	}
	formula->num_clause = 0;
	destroyClause(formula->head);
}


/**
@brief: 判断子句中有无指定的文字
@param clause: 指向需要判断的子句的指针
@param data: 指定文字的序号
@return: 找到返回true, 否则false
*/
bool hasData(struct Clause* clause, int data)
{
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {		//遍历判断
		if (curr->data == data) return true;
		else curr = curr->nextLiteral;
	}
	return false;
}


/**
@brief: 判断公式中是否有空子句
@param formula: 指向公式的指针
@return: 有空子句返回true, 否则返回false
*/
bool hasVoidClause(struct Formula* formula)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历判断
		if (curr->len == 0) return true;
		else curr = curr->nextClause;
	}
	return false;
}


/**
@brief: 删除公式里所有子句所包含的特定文字
@param formula: 指向需要删除子句文字的公式的指针
@param data: 需要删除的文字的序号
@calls: deleteLiteral()
@return: 被删除的文字的序号
*/
int removeLiteralFromFormula(struct Formula* formula, int data)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历删除
		deleteLiteral(curr, data);
		curr = curr->nextClause;
	}
	return data;
}


/**
@brief: 删除公式里所有包含指定文字的子句
@param formula: 指向需要删除子句的公式的指针
@param data: 需要删除的文字的序号
@calls: hasData()
@return: 被删除的文字的序号
*/
int removeClauseHasLiteral(struct Formula* formula, int data)
{
	struct Clause* traget;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历删除
		if (hasData(curr, data)) {
			curr->beforeClause->nextClause = curr->nextClause;
			curr->nextClause->beforeClause = curr->beforeClause;
			traget = curr;
			curr = curr->nextClause;
			free(traget);
			formula->num_clause--;
		}
		else curr = curr->nextClause;
	}
	return data;
}


/**
@brief: 判断一个子句是否为单子句
@param clause: 指向需要判断的子句的指针
@return: 为单子句则为true, 否则为false
*/
bool isUnitClause(struct Clause* clause)
{
	return (clause->len == 1) ? true : false;
}


/**
@brief: 选取公式的第一个子句的第一个文字
@param formula: 指向公式的指针
@return: 选取的文字的序号
*/
int selectFirstData(struct Formula* formula)
{
	return	formula->head->nextClause->head->nextLiteral->data;
}


/**
@brief: 从公式中选取一个单子句并返回其文字序号, 若不存在单子句则返回0
@param formula: 指向公式的指针
@calls: isUnitClause()
@return: 单子句的文字序号, 若不存在单子句返回0
*/
int selectDataFromUnitClause(struct Formula* formula)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历，直到找到一个单子句
		if (isUnitClause(curr)) return curr->head->nextLiteral->data;
		else curr = curr->nextClause;
	}
	return 0;
}


/**
@brief: 评估子句真假
@param clause: 指向需要评估子句的指针
@param var: 变元值数组
@return: 子句为真则为true, 否则为false
*/
bool evaluateClause(struct Clause* clause, int* var)
{
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {		//当存在文字序号与所赋的值同号时表明为真
		if (curr->data * var[abs(curr->data)] >= 0) return true;
		else curr = curr->nextLiteral;
	}
	return false;
}


/**
@brief: 评估公式的真假
@param formula: 指向需要评估公式的指针
@param var: 变元值数组
@calls: evaluateClause()
@return: 公式为真则为true, 否则为false
*/
bool evaluateFormula(struct Formula* formula, int* var)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历，只要有子句不满足则整个公式即不满足
		bool flag = evaluateClause(curr, var);
		if (!flag) return false;
		else {
			curr = curr->nextClause;
		}
	}
	return true;
}


/**
@brief: 单子句传播规则
@param formula: 指向公式的指针
@param data: 单子句的文字序号
*/
void unitClauseRule(struct Formula* formula, int data)
{
	struct Clause* target;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {			//一次遍历完成单子句传播规则
		if (hasData(curr, data)) {
			curr->beforeClause->nextClause = curr->nextClause;
			curr->nextClause->beforeClause = curr->beforeClause;
			target = curr;
			curr = curr->nextClause;
			free(target);
			formula->num_clause--;
		}
		else {
			deleteLiteral(curr, -data);
			curr = curr->nextClause;
		}
	}
}


/**
@brief: 初始化一个计数器
@return: 初始化后的计数器数组
*/
struct Counter* initCounter()
{
	struct Counter* counter = (struct Counter*)malloc(sizeof(struct Counter) * (info.num_literal + 1));
	memset(counter, 0, sizeof(struct Counter) * (info.num_literal + 1));
	return counter;
}


/**
@brief: 从计数器中选取出现次数最多的文字序号
@param counter: 计数器数组
@return: 出现次数最多的文字序号
*/
int argmaxFromCounter(struct Counter* counter)
{
	int i = 1, max_count = -1, argmax = 0;
	for (; i <= info.num_literal; i++) {
		if (counter[i].positive > max_count) {
			max_count = counter[i].positive;
			argmax = i;
		}
		if (counter[i].negative > max_count) {
			max_count = counter[i].negative;
			argmax = -i;
		}
	}
	counter[abs(argmax)].positive = 0;
	counter[abs(argmax)].negative = 0;
	return argmax;
}


struct Counter* copyCounter(struct Counter* counter)
{
	struct Counter* counter_copy = initCounter();
	for (int i = 0; i <= info.num_literal; i++)
		counter_copy[i] = counter[i];
	return counter_copy;
}

