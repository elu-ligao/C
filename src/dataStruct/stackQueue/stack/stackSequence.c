#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "stackSequence.h"

/**
 * @brief 新建数据结点
 * 
 * @return ** pElemType 
 */
pElemType newElement()
{
    pElemType e = (pElemType)calloc(1, sizeof(ElemType));
    if(!e)  perror("newElement error.\n");
    return e;
}


/**
 * @brief 新建空栈结点
 * 
 * @return pSqStack 
 */
pSqStack newSqStack()
{
    pSqStack S = (pSqStack)calloc(1, sizeof(SqStack));
    if(!S)  perror("newSqStack error.\n");
    return S;
}


/**
 * @brief  构造一个空栈
 *          base 为栈底指针，初始化后base不变，若base==NULL说明栈不存在
 * @return pSqStack 
 */
pSqStack initStack()
{
    pSqStack S = newSqStack();
    if(!S)  
    {
        perror("initStack newSqStack error.\n");
        return NULL;
    }
    
    S->base = (pElemType)calloc(MAX_STACK_SIZE, sizeof(ElemType));
    if(!S->base)
    {
        perror("calloc base error.\n");
        return NULL;
    }
    S->top = S->base;   // top 初始化为base
    S->stackSize = MAX_STACK_SIZE;
    return S;
}

/**
 * @brief 销毁栈S
 * 
 * @param S 
 * @return  void 
 */
void destroyStack(pSqStack *S)
{
    if(*S){
        free((*S)->base);
        (*S)->base = NULL;
        free(*S);
        *S = NULL;
    }
}

/**
 * @brief 将S清为空栈
 * 
 * @param S 
 * @return ** void 
 */
void clearStack(pSqStack S)
{
    if(S)
    {
        memset(S->base, 0, sizeof(ElemType)*S->stackSize);
        S->top = S->base;
    }
} 

/**
 * @brief 判断栈是否为空
 * 
 * @param S 
 * @return true     S is empty or S is not exist
 * @return false    S is not empty
 */
bool stackEmpty(pSqStack S)
{
    if(!S)  return true;
    return S->top == S->base;
}

/**
 * @brief 返回栈的长度
 * 
 * @param S 
 * @return ** int 
 */
int stackLength(pSqStack S)
{
    if(!S)  return 0;
    return S->top - S->base;
} 


/**
 * @brief Get the Top object
 * 
 * @param S 
 * @return ** pElemType 
 */
pElemType getTop(pSqStack S)
{
    if(!S)  return NULL;
    if(S->top != S->base)   return S->top-1;
    return NULL;
}

/**
 * @brief 将元素S入栈
 * 
 * @param S 
 * @param e 
 * @return ** void 
 */
// int push(pSqStack *S, pElemType e)
// {
//     if(!(*S)) return -1;
//     if((*S)->top - (*S)->base == (*S)->stackSize)    return -1;
//     (*S)->top->iVal = e->iVal;
//     memcpy((*S)->top->sVal, e->sVal, sizeof(e->sVal));
//     (*S)->top++;
//     return 0;
// }

int push(pSqStack S, pElemType e)
{
    if(!S) return -1;
    if(S->top - S->base == S->stackSize)    return -1;
    S->top->iVal = e->iVal;
    memcpy(S->top->sVal, e->sVal, sizeof(e->sVal));
    S->top++;
    return 0;
}


/**
 * @brief 栈顶元素出栈
 * 
 * @param S 
 * @return ** pElemType 
 */
// pElemType pop(pSqStack *S)
// {
//     if(!(*S))  return NULL;
//     if((*S)->top == (*S)->base)   return NULL;
//     (*S)->top--;
//     return (*S)->top;
// } 
pElemType pop(pSqStack S)
{
    if(!S)  return NULL;
    if(S->top == S->base)   return NULL;
    S->top--;
    return S->top;
} 

/**
 * @brief 遍历栈
 * 
 * @param S 
 * @return ** void 
 */
void stackTraverse(pSqStack S)
{
    if(!S)  return;
    int i = 0;
    
    for(i=S->base-S->base; i<S->top-S->base; ++i)
    {
        printf("iVal: %d, sVal = [%s]\n", (S->base+i)->iVal, (S->base+i)->sVal);
    }
}