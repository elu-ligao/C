#ifndef _TEST_SQSTACK_H
#define _TEST_SQSTACK_H

#include "stackSequence_dynamic.h"

// 进制转换
void testConvertBase(pSqStack *S, int argc, char *argv[]);
void convertBase(pSqStack *S, int num, int base);
char *getConvert(pSqStack *S);



// 括号匹配 
void testBrackets(pSqStack S, int argc, char *argv[]);
char *getBrackets();
int metchBrackets(pSqStack S, char *sBrackets);



// 表达式求值
void evaluateExpressionSq(int argc, char *argv[]);
char *initExpression(char *sExpression);
void initPriorityArr(char *sPriorityArr[]);
char checkPriority(char *sPriorityArr[], char pre, char post);
int getIndex(char expression);
void evaluateExpression(char *sPriorityArr[], char *sExpression);
bool doExpression(pSqStack OPTR, pSqStack OPND, char *sExpression, char **sPriorityArr, int *result);
int operator(int a, char express, int b);



#endif