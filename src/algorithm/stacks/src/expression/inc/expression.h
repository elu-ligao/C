/*
 * @Author       : elu
 * @Date         : 2022-01-13 15:57:17
 * @LastEditTime : 2022-01-14 23:58:02
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include "./stack.h"

#define LEN_EXPRESS_DATA    ( 7 )
#define MIN_EXPRESS_LEN     ( 20 )
#define OPERATOR_TYPE       ( 8 )   // 操作运算符种类(包括括号等)

typedef struct Express
{
    char type;  // '0': 运算符号('+', '-', '*', '/', '^', '(', ')'), '1': 运算数
    char data[LEN_EXPRESS_DATA+1];
} Express, *PtrExp;

PtrExp GenExpArray(int MaxExpSize);     // 申请 MaxExpSize 个空间的Express 结构体数组
PtrExp AnalysisExp(char *exp, int *N);  // 对表达式exp进行解析，并存放于ExpArr数组
PtrElem CreateExpressionTree(PtrElem root, PtrElem left, PtrElem right);    // 以root为根，left,right分别为左右子树建立表达式二叉树
float GetPostfixExpressionValue(Stack S, PtrExp ExpArr, int N); // 获取后缀表达式exp的值
char *FormatExp(char *exp);     // 将exp表达式进行格式化，以空格分开运算数与运算符
float Operate(float a, float b, char opr);
float EvaluateExpTree(PtrElem T);   // 计算表达式树T的值
PtrElem GetPostfixExpressionTree(Stack S, PtrExp ExpArr, int N);    // 获取后缀表达式exp的表达式树
PtrElem GetMidfixExpressionTree(char **sPriorityArr, PtrExp ExpArr, int N);     // 获取中缀表达式exp的表达式树
PtrElem GetPrefixExpressionTree(PtrExp ExpArr, int N, int *k);     // 获取前缀表达式exp的表达式树



void TraversePostRoot2(PtrElem S);   // 后根遍历表达式树
char *TraversePostRoot(PtrElem S);
char *TraversePreRoot(PtrElem S);
void TraverseMidRoot(PtrElem T);
void FreeExpTree(PtrElem *T);






char *GetMidSeq(PtrElem T, char **sPriorityArr, int brackets);  // 获取中序遍历的带括号的表达式序列
// 运算符优先级辅助函数
char **initPriorityArr();   // 初始化运算符的优先级
char checkPriority(char **sPriorityArr, char pre, char post);   // 比较两个运算符优先级大小
int getIndex(char expression);  // 获取运算符在运算符数组中的索引
void freePriority(char **sPriorityArr); // 释放运算符优先级内存

#endif