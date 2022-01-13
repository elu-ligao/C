/*
 * @Author       : elu
 * @Date         : 2022-01-08 22:35:42
 * @LastEditTime : 2022-01-09 23:07:39
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "stack_record.h"

/**
 * @descripttion:   判断栈是否为空
 * @return {*}
 * @param {Stack} S
 */
int IsEmpty(Stack S)
{
    return S->TopOfStack == EmptyTOS;
}

/**
 * @descripttion:   判断栈是否已满
 * @return {*}
 * @param {Stack} S
 */
int IsFull(Stack S)
{
    return S->TopOfStack == S->Capacity-1;
}

/**
 * @descripttion:   创建栈
 * @return {*}
 */
Stack CreateStack(int MaxElements)
{
    if(MaxElements < MinStackSize)
    {
        printf("%d is too small, and set the size with %d\n", MaxElements, MinStackSize);
        MaxElements = MinStackSize;
    }
    Stack S = (Stack)calloc(1, sizeof(struct StackRecord));
    if(NULL == S)
    {
        perror("calloc for Stack error.\n");
        return NULL;
    }

    S->Array = (PtrElem)calloc(MaxElements, sizeof(ElemType));
    if(NULL == S->Array)
    {
        perror("calloc for Stack's Array error.\n");
        return NULL;
    }

    S->Capacity = MaxElements;
    MakeEmpty(S);

    return S;
}

/**
 * @descripttion:   销毁栈
 * @return {*}
 * @param {Stack} S
 */
void DisposeStack(Stack S)
{
    if(NULL != S)
    {
        MakeEmpty(S);
        free(S->Array); S->Array = NULL;
        free(S);    S = NULL;
    }
}

/**
 * @descripttion:   清空栈
 * @return {*}
 * @param {Stack} S
 */
void MakeEmpty(Stack S)
{
    S->TopOfStack = EmptyTOS;
    memset(S->Array, 0, sizeof(ElemType)*S->Capacity);
}

/**
 * @descripttion:   将元素From复制到To
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
int Push(PtrElem X, Stack S)
{
    if(IsFull(S))
    {
        printf("Full Stack. X = %d[%s] Push fail.\n", X->idata, X->sdata);
        return -1;
    }
    S->TopOfStack += 1;
    CopyElemData(&S->Array[S->TopOfStack], X);
    // printf("Push X = %d Success.\n", X->idata);
    return 0;
}

/**
 * @descripttion:   返回栈顶元素 
 * @return {*}
 * @param {Stack} S
 */
PtrElem Top(Stack S)
{
    if(IsEmpty(S))
    {
        perror("Empty Stack.\n");
        return NULL;
    }
    
    return &S->Array[S->TopOfStack];
}

/**
 * @descripttion:   将栈顶元素出栈
 * @return {*}
 * @param {Stack} S
 */
void Pop(Stack S)
{
    if(IsEmpty(S))
        perror("Empty Stack.\n");
    else
    {
        memset(&S->Array[S->TopOfStack], 0, sizeof(ElemType));
        S->TopOfStack--;
    }
}

/**
 * @descripttion:   将栈顶元素出栈并返回其指针 
 * @return {*}
 * @param {Stack} S
 */
PtrElem TopAndPop(Stack S)
{
    if(IsEmpty(S))
    {
        perror("Empty Stack.\n");
        return NULL;
    }
    PtrElem E = (PtrElem)calloc(1, sizeof(ElemType));
    CopyElemData(E, &S->Array[S->TopOfStack]);
    S->TopOfStack--;
    return E;
}