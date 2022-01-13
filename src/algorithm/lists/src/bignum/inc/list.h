/*
 * @Author       : elu
 * @Date         : 2022-01-08 16:18:53
 * @LastEditTime : 2022-01-12 01:36:54
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _LIST_H
#define _LIST_H


typedef struct ElementType    // 元素域
{
    int Coefficient;  // 系数
    int Exponent;       // 指数
}ElemType, *PtrElem;

struct Node    // 节点域
{
    ElemType Element;
    struct Node *next, *pre;
};

typedef struct Node *PtrNode;   // 节点指针
typedef PtrNode List;   // 链表指针
typedef PtrNode Position;   // 节点位置指针

List CreateList();          // 新建链表
List MakeEmpty(List L);     // 清空链表
int IsEmpty(List L);        // 链表是否为空
int IsLast(Position P, List L); // P 是 L 的最后一个元素
Position Find(ElemType X, List L);  // 在 L 中查找元素 X 的位置
void Delete(ElemType X, List L);    // 在 L 中删除元素 X
Position FindPrevious(ElemType X, List L);  // 在 L 中确定 X 的前驱
Position Insert(ElemType X, Position Ptr, List L);    // 在 L 中插入元素 X
void DeleteList(List L);    // 删除链表
Position Header(List L);    // 返回链表头结点(其实就是L)
Position First(List L);     // 返回链表中的第一个元素位置
Position Advance(Position P);   // 返回结点P 的下一个结点位置
ElemType Retrieve(Position P);  // 返回结点P 的元素
void TraverseList(List L);

int CmpElementData(PtrElem p1, PtrElem p2);
void CopyElementData(PtrElem T, PtrElem F);

#endif