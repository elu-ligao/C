#ifndef STACK_LINK_H
#define STACK_LINK_H

#include <stdbool.h>

#define ELEM_CHAR_MAX_LEN   3

typedef struct ElemType         // 数据域
{
    int     iVal;       // int域
    char    sVal[ELEM_CHAR_MAX_LEN+1];      // 字符串域
}ElemType, *pElemType;

typedef struct StackNode        // 链栈结点
{
    pElemType data;             // 数据域指针
    struct StackNode *next;     // 链栈指针
}StackNode, *pStackLink;


pElemType newElement();         // 新建数据结点
pStackLink newSqStack();         // 新建空栈结点
 
pStackLink initStack();     // 构造一个空栈
void destroyStack(pStackLink *S);    // 销毁栈S
void clearStack(pStackLink *S);       // 将S清为空栈
bool stackEmpty(pStackLink S);       // 判断栈是否为空
int  stackLength(pStackLink S);      // 返回栈的长度
pElemType getTop(pStackLink S);      // 返回栈顶元素
int push(pStackLink *S, pElemType e);     // 将元素S入栈
pElemType pop(pStackLink *S);         // 栈顶元素出栈
void stackTraverse(pStackLink S);    // 遍历栈


#endif