/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/10                                                     
//* Description: dpll�㷨��������	                          
/**********************************************************/

#pragma once
#include"tools.h"
#include"../parser/parser.h"
#include"../data_struct/data_struct.h"
#include"../../naive_dpll/data_structure/data_struct.h"


/**
@brief: ����ѡȡ���ֽ��и�ֵ������ʽ
@param formula: ָ��ʽ��ָ��
@param res: �������ֵĸ�ֵ����
@param data: ѡȡ������
@param mask: ָ��mask��ָ��
@param level: �������ڲ���
@param counter: ������
*/
void simplify(struct BinVector* formula, short*res, short data, struct Mask* mask, int level, int* counter)
{
	res[abs(data)] = data;
	rmClausesHasLit(formula, data, mask, level, counter);
	rmLitFromFormula(formula, -data, mask, level, counter);
}


/**
@brief: �㷨������
@param formula: ָ��ʽ��ָ��
@param mask: ָ��mask��ָ��
@param res: ����������ָ�ֵ������
@param level: ���߲���
@param counter: ������
@return: ��ʽ�����㷵��true
*/
bool dpllOptiX(struct BinVector* formula, struct Mask* mask, short* res, int level, int* counter)
{
	int selected_data = 0;
	while ((selected_data = chooseUnitData(*formula, *mask))) {
		simplify(formula, res, selected_data, mask, level, counter);
		if (isFormulaVoid(*mask)) return true;
		if (isFormulaHasVoid(*mask)) {
			recoverFormula(*formula, mask, level, counter);
			return false;
		}
	}
	if (isFormulaVoid(*mask)) return true;
	if (isFormulaHasVoid(*mask)) return false;
	
	//selected_data = chooseFIrstData(*formula, *mask);
	selected_data = chooseData(*formula, *mask, counter);
	simplify(formula, res, selected_data, mask, level + 1, counter);
	if (dpllOptiX(formula, mask, res, level+1, counter)) return true;
	else {
		simplify(formula, res, -selected_data, mask, level + 1, counter);
		if(dpllOptiX(formula, mask, res, level+1, counter)) return true;
		else {
			recoverFormula(*formula, mask, level, counter);
			return false;
		}
	}
}


/**
@brief: �����㷨�ӿ�
@param formula: ָ��ʽ��ָ��
@param mask: ָ��mask��ָ��
@param counter: ������
*/
struct Result DPLLOptiX(struct BinVector* formula, struct Mask* mask, int *counter)
{
	struct Result result = initResult();
	float time_start = clock();
	result.isSatisfied = dpllOptiX(formula, mask, result.res, 0, counter);			//�������
	result.time = clock() - time_start;
	return result;
}