/*
 * @Author       : elu
 * @Date         : 2022-01-08 22:09:38
 * @LastEditTime : 2022-01-09 22:41:04
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "stack_link.h"



/**
 * @descripttion:   判断栈是否为空
 * @return {*}
 * @param {Stack} S
 */
int IsEmpty(Stack S)
{
    return S->next == NULL;
}

/**
 * @descripttion:   创建栈
 * @return {*}
 */
Stack CreateStack()
{
    Stack S = (Stack)calloc(1, sizeof(struct Node));
    if(NULL == S)
    {
        perror("calloc for Stack error.\n");
        return NULL;
    }
    // MakeEmpty(S);
    return S;
}

/**
 * @descripttion:   销毁栈
 * @return {*}
 * @param {Stack} S
 */
void DisposeStack(Stack S)
{
    PtrNode P, TmpCell = S->next;
    S->next = NULL;
    while (TmpCell)
    {
        P = TmpCell->next;
        free(TmpCell);
        TmpCell = P;
    }
}

/**
 * @descripttion:   清空栈
 * @return {*}
 * @param {Stack} S
 */
void MakeEmpty(Stack S)
{
    if(NULL == S)
    {
        perror("Create Stack First.\n");
        return;
    }
    while(!IsEmpty(S))
    {
        printf("%d \n", Top(S)->idata);
        Pop(S);
    }
}

/**
 * @descripttion:   将元素From内容复制给元素To
 * @return {*}
 * @param {PtrElem} To
 * @param {PtrElem} From
 */
void CopyElemData(PtrElem To, PtrElem From)
{
    memcpy(To, From, sizeof(ElemType));
}


/**
 * @descripttion:   元素 X 入栈
 * @return {*}
 * @param {PtrElem} X
 * @param {Stack} S
 */
void Push(PtrElem X, Stack S)
{
    PtrNode TmpCell = (PtrNode)calloc(1, sizeof(struct Node));
    if(NULL == TmpCell)
    {
        perror("calloc for Node error.\n");
        return;
    }
    CopyElemData(&TmpCell->Element, X);
    TmpCell->next = S->next;
    S->next = TmpCell;
}

/**
 * @descripttion:   返回栈顶元素 
 * @return {*}
 * @param {Stack} S
 */
PtrElem Top(Stack S)
{
    return &S->next->Element;
}

/**
 * @descripttion:   将栈顶元素出栈并返回其指针
 * @return {*}
 * @param {Stack} S
 */
void Pop(Stack S)
{
    if(IsEmpty(S))
    {
        perror("Empty Stack.\n");
        return;
    }
    PtrNode P = S->next;
    S->next = P->next;
    free(P);
    P = NULL;
} 

