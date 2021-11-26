#ifndef STACK_LINK_H
#define STACK_LINK_H

#define ELEM_CHAR_MAX_LEN   4
#define MAX_STACK_SIZE      20 

typedef struct ElemType     // 数据域
{
    int     iVal;   // int域
    char    sVal[ELEM_CHAR_MAX_LEN];   // 字符串域
}ElemType, *pElemType;

typedef struct StackNode      // 链表栈
{
    pElemType data;      // 数据域
    struct StackNode *next;      // 指针域 
}StackNode, *stackLink;


stackLink initStack();     // 构造一个空栈
void destroyStack(stackLink *S);   // 销毁栈S
void clearStack(stackLink S);      // 将S清为空栈
bool stackEmpty(stackLink S);      // 判断栈是否为空
int  stackLength(stackLink S);     // 返回栈的长度
pElemType getTop(stackLink S);     // 返回栈顶元素
void push(stackLink *S, pElemType e);     // 将元素S入栈
pElemType pop(stackLink *S);       // 栈顶元素出栈
void stackTraverse(stackLink S);    // 遍历栈


#endif