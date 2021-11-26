#ifndef STACK_SEQUENCE_H
#define STACK_SEQUENCE_H

#include <stdbool.h>

#define ELEM_CHAR_MAX_LEN   3
#define MAX_STACK_SIZE      20 

typedef struct ElemType     // 数据域
{
    int     iVal;   // int域
    char    sVal[ELEM_CHAR_MAX_LEN+1];   // 字符串域
}ElemType, *pElemType;

typedef struct SqStack      // 顺序栈
{
    pElemType base;     // 栈底指针
    pElemType top;      // 栈顶指针
    int stackSize;      // 栈最大容量 
}SqStack, *pSqStack;


pElemType newElement();     // 新建数据结点
pSqStack newSqStack();      // 新建空栈结点
 
pSqStack initStack();     // 构造一个空栈
void destroyStack(pSqStack *S);   // 销毁栈S
void clearStack(pSqStack S);      // 将S清为空栈
bool stackEmpty(pSqStack S);      // 判断栈是否为空
int  stackLength(pSqStack S);     // 返回栈的长度
pElemType getTop(pSqStack S);     // 返回栈顶元素
int push(pSqStack S, pElemType e);     // 将元素S入栈
pElemType pop(pSqStack S);       // 栈顶元素出栈
void stackTraverse(pSqStack S);   // 遍历栈


#endif