/*
 * @Author       : elu
 * @Date         : 2022-01-08 16:33:22
 * @LastEditTime : 2022-01-12 23:07:15
 * @LastEditors  : Please set LastEditors
 * @Description  : 不用到指针，游标法实现链表
 *                 1. CursorSpace 数组作为全局变量
 *                 2. 0号单元控制模拟malloc 和 free， 为头结点
 *                    申请空间即使用 CursorSpace[0].next 的数组单元, 并更新其值为 CursorSpace[0].next.next 作为下一次申请的单元 
 *                    释放空间要及时更新 CursorSpace[0].next 的指向为释放的单元索引
 */
#ifndef _Cursor_H
#define _Cursor_H

#define SpaceSize ( 1000 )

typedef int ElemType;    // 元素域
typedef int PtrNode;   // 节点指针
typedef PtrNode List;   // 链表指针
typedef PtrNode Position;   // 节点位置指针

void InitializeCursorSpace();   // 初始化 CursorSpace next 指向
List CreateList();
void MakeEmpty(List L);     // 清空链表
int IsEmpty(List L);        // 链表是否为空
int IsLast(Position P, List L); // P 是 L 的最后一个元素
Position Find(ElemType X, List L);  // 在 L 中查找元素 X 的位置
void Delete(ElemType X, List L);    // 在 L 中删除元素 X
Position FindPrevious(ElemType X, List L);  // 在 L 中确定 X 的前驱
Position Insert(ElemType X, Position P, List L);    // 在 L 中的 P 结点后插入元素 X
Position GetLastIdx(List L);   // 最后一个元素的位置
ElemType GetLastElement(List L);   // 最后一个元素
void DeleteList(List L);    // 删除链表
Position Header(List L);    // 返回链表头结点(其实就是L)
Position First(List L);     // 返回链表中的第一个元素位置
Position Advance(Position P);   // 返回结点P 的下一个结点位置
ElemType Retrieve(Position P);  // 返回结点P 的元素
void TraverseList(List L);
void ShowCursorSpace(int n);

void MakeCircularLink(List L);    // 使链表为循环链表

#endif