/*
 * @Author       : elu
 * @Date         : 2022-02-20 23:30:58
 * @LastEditTime : 2022-02-21 23:14:04
 * @LastEditors  : Please set LastEditors
 * @Description  :  优先队列(堆)-二叉堆
 */
#ifndef _BinHeap_H
#define _BinHeap_h

#define MinPQSize   (100)

typedef int  ElementType;

struct HeapStruct
{
    int Capacity;   // 容量
    int Size;       // 现有元素大小
    ElementType *Elements;
};

typedef struct HeapStruct *PriorityQueue;

PriorityQueue Initialize(int MaxElements);
void Destroy(PriorityQueue H);
void Insert(PriorityQueue H, ElementType X);
ElementType DeleteMin(PriorityQueue H);
ElementType FindMin(PriorityQueue H);
int IsEmpty(PriorityQueue H);
int IsFull(PriorityQueue H);
PriorityQueue BuildHeap(ElementType *Arr, int N);
void ShowElements(PriorityQueue H);

#endif