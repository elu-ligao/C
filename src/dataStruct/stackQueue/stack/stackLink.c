#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "stackLink.h"

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
 * @return pStackLink 
 */
pStackLink newStackLink()
{
    pStackLink S = (pStackLink)calloc(1, sizeof(StackNode));
    if(!S)  
    {
        perror("newStackLink error.\n");
        return NULL;
    }
    S->data = newElement();
    if(!S->data)
    {
        perror("newElement error.\n");
        return NULL;
    }
    return S;
}


/**
 * @brief  构造一个空栈
 *          S 始终指向栈顶，采用前插入的方式插入数据，若S==NULL说明栈为空
 * @return pStackLink 
 */
pStackLink initStack()
{
    return NULL;
}

/**
 * @brief 销毁栈S
 * 
 * @param S 
 * @return  void 
 */
void destroyStack(pStackLink *S)
{

    pStackLink p = *S;
    while(p)
    {
        *S = (*S)->next;
        free(p->data);
        p->data = NULL;
        p = NULL;
        p = *S;
    }
}

/**
 * @brief 将S清为空栈
 * 
 * @param S 
 * @return ** void 
 */
void clearStack(pStackLink *S)
{
    destroyStack(S);
} 

/**
 * @brief 判断栈是否为空
 * 
 * @param S 
 * @return true     S is empty or S is not exist
 * @return false    S is not empty
 */
bool stackEmpty(pStackLink S)
{
    if(!S)  return true;
    return false;
}

/**
 * @brief 返回栈的长度
 * 
 * @param S 
 * @return ** int 
 */
int stackLength(pStackLink S)
{
    if(!S)  return 0;
    int i = 0;
    pStackLink p = S;
    while(p)
    {
        ++i;
        p = p->next;
    }
    return i;
} 


/**
 * @brief Get the Top object
 * 
 * @param S 
 * @return ** pElemType 
 */
pElemType getTop(pStackLink S)
{
    if(!S)  return NULL;
    return S->data;
}

/**
 * @brief 将元素S入栈
 * 
 * @param S 
 * @param e 
 * @return ** void 
 */
int push(pStackLink *S, pElemType e)
{
    pStackLink p = newStackLink();
    if(!p)  return -1;
    p->data->iVal = e->iVal;
    memcpy(p->data->sVal, e->sVal, sizeof(e->sVal));
    p->next = *S;
    *S = p;
    return 0;
}


/**
 * @brief 栈顶元素出栈
 * 
 * @param S 
 * @return ** pElemType 
 */
pElemType pop(pStackLink *S)
{
    if(!*S)  return NULL;
    
    pElemType e = newElement();
    e->iVal = (*S)->data->iVal;
    memcpy(e->sVal, (*S)->data->sVal, sizeof(e->sVal));

    pStackLink p = *S;
    *S = (*S)->next;
    
    free(p->data);
    p->data = NULL;
    free(p);
    p = NULL;

    return e;
} 

/**
 * @brief 遍历栈
 * 
 * @param S 
 * @return ** void 
 */
void stackTraverse(pStackLink S)
{
    if(!S)  return;
    pStackLink p = S;
    while(p)
    {
        printf("iVal: %d, sVal = [%s]\n", p->data->iVal, p->data->sVal);
        p = p->next;
    }
}