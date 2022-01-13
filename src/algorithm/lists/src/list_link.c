/*
 * @Author       : elu
 * @Date         : 2022-01-08 16:29:19
 * @LastEditTime : 2022-01-09 16:04:04
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <list.h>

struct Node    // 节点域
{
    ElemType Element;
    Position next;
};

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
    while(P && P->Element != X)
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
    while(P->next && P->next->Element != X)
        P = P->next;
    return P;
}

/**
 * @descripttion:   在 L 中的 P 结点后插入元素 X
 * @return {*}
 * @param {ElemType} X
 * @param {Position} P
 * @param {List} L
 */
void Insert(ElemType X, Position P, List L)
{
    Position TmpCell = (Position)calloc(1, sizeof(struct Node));
    if(NULL == TmpCell)
    {
        perror("calloc for TmpCell error.\n");
        return;
    }
    TmpCell->Element = X;
    TmpCell->next = P->next;
    P->next = TmpCell;
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
        printf("%d ", Retrieve(P));
        P = P->next;
    }
    printf("\n");
}