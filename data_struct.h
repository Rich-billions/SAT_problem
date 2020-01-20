/***********************************************************/
/** author: Zhang DY                                                     **/
/** date: 2020/01/20                                                     **/
/** description: CNF公式存储数据结构与相关函数实现    **/
/**********************************************************/

#pragma once
#include<stdio.h>
#include<stdlib.h>
#define DEBUG

//------------------------data structure---------------------//
struct Literal
{
	int data = 0;
	struct Literal* nextLiteral = NULL;
	struct Literal* beforeLiteral = NULL;
	bool isHead = false;
	bool isTail = false;
};

struct Clause
{
	struct Literal* head;
	struct Literal* tail;
	struct Clause* nextClause = NULL;
	struct Clause* beforeClause = NULL;

	bool isFirstClause = false;
	bool isLastClause = false;
	int len = 0;
};

struct Formula
{
	struct Clause* head;
	struct Clause* tail;

	int num_clause = 0;
};

//-------------------------some functions---------------------------//
struct Clause* initClause()
{
	/**
	@brief: 初始化一个子句
	@return: 指向初始化子句的指针
	*/
	struct Clause* clause = (struct Clause*) malloc(sizeof(struct Clause));
	if (!clause) {
		printf("malloc error.\nFileName:data_struct\nFunc:initClause\n");
		return NULL;
	}
	else {
		clause->head = (struct Literal*)malloc(sizeof(struct Literal));
		clause->tail = (struct Literal*)malloc(sizeof(struct Literal));
		if (!clause->head or !clause->tail) {
			printf("malloc error.\nFileName:data_struct\nFunc:initClause\n");
			return NULL;
		}
		else {
			clause->head->nextLiteral = clause->tail;
			clause->head->isHead = true;
			clause->head->isTail = false;
			clause->tail->beforeLiteral = clause->head;
			clause->tail->isHead = false;
			clause->tail->isTail = true;
			clause->len = 0;
			return clause;
		}
	}
}

struct Formula* initFormula()
{
	/**
	@brief: 初始化一个公式
	@calls: initClause()
	@return: 指向初始化公式的指针
	*/
	struct Formula* formula = (struct Formula*)malloc(sizeof(struct Formula));
	if (!formula) {
		printf("malloc error.\nFileName:data_struct\nFunc:initFormula\n");
		return NULL;
	}
	else {
		formula->head = initClause();
		formula->tail = initClause();
		formula->head->nextClause = formula->tail;
		formula->head->isFirstClause = true;
		formula->head->isLastClause = false;
		formula->tail->beforeClause = formula->head;
		formula->tail->isFirstClause = false;
		formula->tail->isLastClause = true;
		formula->num_clause = 0;
		return formula;
	}
}

void addLiteral(struct Clause* clause, int data)
{
	/**
	@brief: 给一个子句尾部加上一个文字
	@param clause: 需要添加文字的指向子句的指针
	@param data: 添加文字的序号
	*/
	struct Literal* tl = clause->tail;
	struct Literal* temp = (struct Literal*)malloc(sizeof(struct Literal));
	if (temp) {
		temp->data = data;
		temp->isTail = false;
		temp->isHead = false;
		tl->beforeLiteral->nextLiteral = temp;
		temp->beforeLiteral = tl->beforeLiteral;
		temp->nextLiteral = tl;
		tl->beforeLiteral = temp;
		clause->len++;
	}
	else {
		printf("malloc error.\n FileName:data_struct.h\nFunc:addLiteral\n");
	}
}

struct Clause* addClause(struct Formula* formula)
{
	/**
	@brief: 给指定公式尾部添加一个公式
	@param formula: 指定的公式
	@return: 指向新添加公式的指针
	*/
	struct Clause* tl = formula->tail;
	struct Clause* temp = initClause();
	if(temp){
		temp->isFirstClause = temp->isLastClause = false;
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

void destroyClause(struct Clause* clause)
{
	/**
	@brief: 删除一个子句
	@param clause: 指向被删除子句的指针
	*/
	struct Literal* curr = clause->tail;
	struct Literal* prev = curr->beforeLiteral;
	while (prev != NULL) {
		free(curr);
		curr = prev;
		prev = prev->beforeLiteral;
	}
	clause->len = 0;
	free(clause->head);
}

bool hasData(struct Clause* clause, int data)
{
	/**
	@brief: 判断子句中有无指定的文字
	@param clause: 指向需要判断的子句的指针
	@param data: 指定文字的序号
	@return: 找到返回true, 否则false
	*/
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {
		if (curr->data == data) return true;
		else curr = curr->nextLiteral;
	}
	return false;
}

int removeLiteral(struct Formula* formula, int data)
{
	/**
	@brief: 删除公式里所有包含指定文字的子句
	@param formula: 指向需要删除子句的公式的指针
	@param data: 需要删除的文字的序号
	@calls: hasData()
	@return: 被删除的文字的序号
	*/
	struct Clause* traget;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {
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

bool isUnitClause(struct Clause* clause)
{
	/**
	@brief: 判断一个子句是否为单子句
	@param clause: 指向需要判断的子句的指针
	@return: 为单子句则为true, 否则为false
	*/
	return (clause->len == 1) ? true : false;
}

bool evaluateClause(struct Clause* clause, int* var)
{
	/**
	@brief: 评估子句真假
	@param clause: 指向需要评估子句的指针
	@param var: 变元值数组
	@return: 子句为真则为true, 否则为false
	*/
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {
		if (curr->data * var[abs(curr->data)] > 0) return true;
		else curr = curr->nextLiteral;
	}
	return false;
}

bool evaluateFormula(struct Formula* formula, int* var)
{
	/**
	@brief: 评估公式的真假
	@param formula: 指向需要评估公式的指针
	@param var: 变元值数组
	@calls: evaluateClause()
	@return: 公式为真则为true, 否则为false
	*/
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {
		if (!evaluateClause(curr, var)) return false;
		else curr = curr->nextClause;
	}
	return true;
}

//-------------------------functions for debug-----------------------------------//
#ifdef DEBUG
void printFormula(struct Formula* formula)
{
	int i = 1;
	struct Clause* currClause = formula->head->nextClause;
	while (!currClause->isLastClause) {
		struct Literal* currLit = currClause->head->nextLiteral;
		printf("Formula %d:\n", i++);
		while (!currLit->isTail) {
			printf("%d ", currLit->data);
			currLit = currLit->nextLiteral;
		}
		std::cout << currClause->len<<'\n';
		currClause = currClause->nextClause;
	}
}
#endif // DEBUG

