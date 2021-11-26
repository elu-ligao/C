#ifndef _TEST_STACK_LINK_H
#define _TEST_STACK_LINK_H

#include "stackLink_dynamic.h"

// 进制转换
void testConvertBase(int argc, char *argv[]);
void convertBase(pStackLink *S, int num, int base);
char *getConvert(pStackLink *S);


// 括号匹配 
void testBrackets(int argc, char *argv[]);
char *getBrackets();
int metchBrackets(char *sBrackets);



// 表达式求值
void evaluateExpressionLink(int argc, char *argv[]);
char *initExpression(char *sExpression);
void initPriorityArr(char *sPriorityArr[]);
char checkPriority(char *sPriorityArr[], char pre, char post);
int getIndex(char expression);
void evaluateExpression(char *sPriorityArr[], char *sExpression);
bool doExpression(char *sExpression, char **sPriorityArr, int *result);
int operator(int a, char express, int b);


#endif