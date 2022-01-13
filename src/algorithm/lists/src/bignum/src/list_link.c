/*
 * @Author       : elu
 * @Date         : 2022-01-08 16:29:19
 * @LastEditTime : 2022-01-12 10:27:54
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "../inc/list.h"

/**
 * @descripttion:   新建链表，返回链表头结点
 * @return {*}
 */
List CreateList()
{
    List L = (List)calloc(1, sizeof(struct Node));
    if(NULL == L)
    {
        perror("CreateList error.\n");
        return NULL;
    }
    L->Element.Exponent = INT_MAX;
    return L;
}


/**
 * @descripttion:   清空链表
 * @return {*}
 * @param {List} L
 */
List MakeEmpty(List L)
{
    if(NULL == L)   return NULL;
    Position r, P = L->next;
    L->next = NULL;
    while(P)
    {
        r = P;
        P = P->next;
        free(r);
        r = NULL;
    }
    return L;
}

/**
 * @descripttion:   链表是否为空
 * @return {*}
 * @param {List} L
 */
int IsEmpty(List L)
{
    return L->next == NULL;
}

/**
 * @descripttion:   P 是 L 的最后一个元素
 *                  如果 P 为 NULL， 返回 真
 * @return {*}
 * @param {Position} P
 * @param {List} L
 */
int IsLast(Position P, List L)
{
    return NULL == P || P->next == NULL;
}


/**
 * @descripttion:   在 L 中查找元素 X 的位置
 * @return {*}
 * @param {ElemType} X
 * @param {List} L
 */
Position Find(ElemType X, List L)
{
    Position P = L->next;
    while(P && CmpElementData(&P->Element, &X) != 0)
        P = P->next;
    return P;
}

/** 
 * @descripttion:   在 L 中删除元素 X
 * @return {*}
 * @param {ElemType} X
 * @param {List} L
 */
void Delete(ElemType X, List L)
{
    Position P, TmpCell;
    P = FindPrevious(X, L);

    if(!IsLast(P, L))
    {
        TmpCell = P->next;
        P->next = TmpCell->next;
        free(TmpCell);
    }
}

/**
 * @descripttion:   在 L 中确定 X 的前驱
 * @return {*}
 * @param {ElemType} X
 * @param {List} L
 */
Position FindPrevious(ElemType X, List L)
{
    Position P = L;
    while(P->next && CmpElementData(&P->Element, &X) != 0)
        P = P->next;
    return P;
}


void CopyElementData(PtrElem T, PtrElem F)
{
    memcpy(T, F, sizeof(ElemType));
}

typedef List BigNum;
extern void BigNumCarry(BigNum A, int Exp, int n);
extern void BigNumCheckCarry(BigNum A);
void AddElementData(PtrElem p1, PtrElem p2)
{
    p1->Exponent = p2->Exponent;
    p1->Coefficient = p1->Coefficient + p2->Coefficient;
}


int CmpElementData(PtrElem p1, PtrElem p2)
{
    return memcmp(p1, p2, sizeof(ElemType));
}

/**
 * @descripttion:   在 L 中插入元素 X
 *                  按 Element.Exponent 降序， 相等则合并
 * @return {*}      返回插入后X结点的地址
 * @param {ElemType} X
 * @param {List} L
 */
Position Insert(ElemType X, Position L, List Head)
{
    Position TmpCell = (Position)calloc(1, sizeof(struct Node));
    if(NULL == TmpCell)
    {
        perror("calloc for TmpCell error.\n");
        return L;
    }
    Position P = L;
    Position Pre = L;
    
    while(P && P->Element.Exponent > X.Exponent)    // 找到不小于X.Exponent的结点
    {      
        Pre = P;
        P = P->next;  
    }
    // printf("P = %p, P.Exp = %d\n", P, P?P->Element.Exponent:-1);
    if(P == NULL)   // 说明当前应该插在最后面， 是L的第一个结点
    {
        // printf("Exp = %d\n", X.Exponent);
        CopyElementData(&TmpCell->Element, &X);
        TmpCell->next = NULL;

        Pre->next = TmpCell;
        TmpCell->pre = Pre;
    }
    else if(P->Element.Exponent == X.Exponent)  // 指数相同，系数相加
    {
        AddElementData(&P->Element, &X);
        TmpCell = P;
    }
    else    // P结点系数小于X的系数，X应插在P结点前面，即Pre的后面
    {
        CopyElementData(&TmpCell->Element, &X);
        TmpCell->next = P;
        P->pre = TmpCell;

        Pre->next = TmpCell;
        TmpCell->pre = Pre;
    }
    
    BigNumCheckCarry(TmpCell);
    
    if(TmpCell->Element.Coefficient == 0)
    {
        Pre = TmpCell->pre; // 重新赋值 Pre
        // printf("Pre.Exp = %d, TmpCell->next = %d (%p)\n", Pre->Element.Exponent, TmpCell->next?TmpCell->next->Element.Exponent:-1, TmpCell->next);
        Pre->next = TmpCell->next;
        if(TmpCell->next)
            TmpCell->next->pre = Pre;
        // printf("---------- After BigNumCheckCarry\n");
        // TraverseList(Head);
        free(TmpCell);
        return Pre;
    }
    // printf("---------- After BigNumCheckCarry\n");
    // TraverseList(Head);

    return TmpCell;
}

/**
 * @descripttion:   删除链表
 * @return {*}
 * @param {List} L
 */
void DeleteList(List L)
{
    Position P, TmpCell;
    P = L->next;
    L->next = NULL;

    while(P)
    {
        TmpCell = P->next;
        free(P);
        P = TmpCell;
    }
}

/**
 * @descripttion:   返回链表头结点(其实就是L)
 * @return {*}
 * @param {List} L
 */
Position Header(List L)
{
    return L;
}

/**
 * @descripttion:   返回链表中的第一个元素位置
 * @return {*}
 * @param {List} L
 */
Position First(List L)
{
    return L->next;
}

/**
 * @descripttion:   返回结点P 的下一个结点位置
 * @return {*}
 * @param {Position} P
 */
Position Advance(Position P)
{
    return P->next;
}

/**
 * @descripttion:   返回结点P 的元素
 * @return {*}
 * @param {Position} P
 */
ElemType Retrieve(Position P)
{
    return P->Element;
}

/**
 * @descripttion:   遍历链表
 * @return {*}
 * @param {List} L
 */
void TraverseList(List L)
{
    Position P = L->next;
    while(P)
    {
        printf("%d * X^%d | ", Retrieve(P).Coefficient, Retrieve(P).Exponent);
        P = P->next;
    }
    printf("\n");
}