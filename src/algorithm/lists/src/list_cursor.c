/*
 * @Author       : elu
 * @Date         : 2022-01-08 16:29:29
 * @LastEditTime : 2022-01-12 23:28:48
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cursor.h"


struct Node
{
    ElemType Element;
    Position next;
};

// 全局变量
struct Node CursorSpace[ SpaceSize ];


/**
 * @descripttion:   模拟申请内存，返回下一个可用的数组单元索引
 * @return {*}
 */
static Position CursorAlloc()
{
    Position P = CursorSpace[0].next;   
    if(0 == P)
    {
        perror("CursorAlloc error. It is Full.\n");
        return 0;
    }
    CursorSpace[0].next = CursorSpace[P].next;  // 更新下一次申请的可用单元索引
    CursorSpace[P].next = 0;
    return P;
}

/**
 * @descripttion:   模拟释放内存，更新P的next为现在可使用的下一个可用单元索引
 *                  并将P置为下一次可申请使用的数组单元索引
 * @return {*}
 * @param {Position} P
 */
static void CursorFree(Position P)
{
    CursorSpace[P].next = CursorSpace[0].next;  
    CursorSpace[P].Element = 0;
    CursorSpace[0].next = P;
}

/**
 * @descripttion:   新建链表
 * @return {*}
 */
List CreateList()
{
    return CursorAlloc();
}


/**
 * @descripttion:   初始化 CursorSpace next 指向
 * @return {*}
 */
void InitializeCursorSpace()
{
    int i = 0;
    for(i=0; i<SpaceSize-1; ++i)
        CursorSpace[i].next = i+1;
    CursorSpace[i].next = 0;
}

/**
 * @descripttion:   清空链表
 * @return {*}
 * @param {List} L
 */
void MakeEmpty(List L)
{
    Position r, P = CursorSpace[L].next;
    while(P)
    {
        r = CursorSpace[P].next;
        CursorFree(P);
        P = r;
    }
    CursorSpace[L].Element = 0;
    CursorSpace[L].next = 0;
}

/**
 * @descripttion:   链表是否为空
 * @return {*}
 * @param {List} L
 */
int IsEmpty(List L)
{
    return CursorSpace[L].next == 0;
}

/**
 * @descripttion:   P 是 L 的最后一个元素
 * @return {*}
 * @param {Position} P
 * @param {List} L
 */
int IsLast(Position P, List L)
{
    return CursorSpace[P].next == 0;
}

/**
 * @descripttion:   在 L 中查找元素 X 的位置
 * @return {*}
 * @param {ElemType} X
 * @param {List} L
 */
Position Find(ElemType X, List L)
{
    Position P = CursorSpace[L].next;
    while(P && CursorSpace[P].Element != X)
        P = CursorSpace[P].next;
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
    P = FindPrevious(X, L);     // 找到 X 的前驱
    // printf("CursorSpace[P].next = %d, CursorSpace[L].Element = %d\n",
    // CursorSpace[P].next, CursorSpace[L].Element);
    if(CursorSpace[P].next == CursorSpace[L].Element)   // X 是最后一个结点， 更新L的最后一个结点为P
    {
        CursorSpace[L].Element = P;
    }

    if(!IsLast(P, L))   // 前驱不是最后一个元素，即X存在，则将X所在索引给0.next
    {
        TmpCell = CursorSpace[P].next;
        CursorSpace[P].next = CursorSpace[TmpCell].next;
        CursorFree(TmpCell);
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
    while(CursorSpace[P].next != 0 && CursorSpace[ CursorSpace[P].next ].Element != X)
        P = CursorSpace[P].next;
    return P;
}

/**
 * @descripttion:   在 L 中的 P 结点后插入元素 X
 * @return {*}
 * @param {ElemType} X
 * @param {Position} P
 * @param {List} L
 */
Position Insert(ElemType X, Position P, List L)
{
    Position TmpCell = CursorAlloc();
    if(0 == TmpCell)
    {
        perror("Full.\n");
        return -1;
    }
    CursorSpace[TmpCell].Element = X;
    CursorSpace[TmpCell].next = CursorSpace[P].next;
    // if(CursorSpace[L].Element == 0 || P == CursorSpace[L].Element)  // 如果P是最后一个结点
    if(IsLast(P, L))
        CursorSpace[L].Element = TmpCell;   // 头结点存放最后一个结点的地址   
    CursorSpace[P].next = TmpCell;
    return TmpCell;  // 返回插入元素的位置
}

/**
 * @descripttion:   删除链表
 * @return {*}
 * @param {List} L
 */
void DeleteList(List L)
{
    MakeEmpty(L);
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
    return CursorSpace[L].next;
}

/**
 * @descripttion:   返回结点P 的下一个结点位置
 * @return {*}
 * @param {Position} P
 */
Position Advance(Position P)
{
    return CursorSpace[P].next;
}

/**
 * @descripttion:   返回结点P 的元素
 * @return {*}
 * @param {Position} P
 */
ElemType Retrieve(Position P)
{
    return CursorSpace[P].Element;
}

void TraverseList(List L)
{
    Position P = CursorSpace[L].next;
    int i = 20;
    while(P)
    {
        printf("%d ", CursorSpace[P].Element);
        P = CursorSpace[P].next;
        if(--i == -2)
            break;
    }
    printf("\n");
}

void ShowCursorSpace(int n)
{
    int i = 0;
    printf("idx Elem Next\n");
    for(i=0; i<n; ++i)
    {
        printf("%-3d %-4d %d\n", i, CursorSpace[i].Element, CursorSpace[i].next);
    }
}

/**
 * @descripttion:   最后一个元素的位置
 * @return {*}
 * @param {List} L
 */
Position GetLastIdx(List L)
{
    return CursorSpace[L].Element;
}

/**
 * @descripttion:   最后一个元素
 * @return {*}
 * @param {List} L
 */
ElemType GetLastElement(List L)
{
    return CursorSpace[ CursorSpace[L].Element ].Element;
}

/**
 * @descripttion:   使链表为循环链表
 * @return {*}
 */
void MakeCircularLink(List L)
{
    CursorSpace[ CursorSpace[L].Element ].next = CursorSpace[L].next;
}