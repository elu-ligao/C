/*
 * @Author       : elu
 * @Date         : 2022-02-21 14:42:06
 * @LastEditTime : 2022-02-27 21:01:07
 * @LastEditors  : Please set LastEditors
 * @Description  :  元素从下标1开始存放，0号元素存哨兵
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "../inc/binHeap.h"

/**
 * @descripttion:   初始化
 * @return {*}
 * @param {int} MaxElements
 */
PriorityQueue Initialize(int MaxElements)
{
    if(MaxElements < MinPQSize)
    {
        MaxElements = MinPQSize;
        printf("Priority queue size is too small. Set it to %d.\n", MaxElements);
    }

    PriorityQueue H = (PriorityQueue)calloc(1, sizeof(struct HeapStruct));
    if(!H)
    {
        perror("PriorityQueue calloc error.\n");
        return NULL;
    }
    H->Capacity = MaxElements;
    H->Size = 0;
    // Allocate the array plus one extra for sentinel
    H->Elements = (ElementType *)calloc(H->Capacity+1, sizeof(ElementType));
    if(!H->Elements)
    {
        perror("Calloc for Heap's Elements error.\n");
        return NULL;
    }

    // 设置哨兵，可提高效率
    H->Elements[0] = INT_MIN;
    return H;
}

/**
 * @descripttion:   判断H是否为空， 为空则返回1，非空返回0
 * @return {*}
 * @param {PriorityQueue} H
 */
int IsEmpty(PriorityQueue H)
{
    return 0 == H->Size;
}

/**
 * @descripttion:   判断H是否已满， 已满返回1， 未满返回0
 * @return {*}
 * @param {PriorityQueue} H
 */
int IsFull(PriorityQueue H)
{
    return H->Size == H->Capacity;
}

/**
 * @descripttion:   元素入堆
 * @return {*}
 * @param {PriorityQueue} H
 * @param {ElementType} X
 */
void Insert(PriorityQueue H, ElementType X)
{
    if(IsFull(H))
    {
        printf("Priority queue is full.\n");
        return;
    }

    int i = 0;
    for(i=++H->Size; H->Elements[i>>1] > X; i>>=1)
    {
        H->Elements[i] = H->Elements[i>>1];
    }
    H->Elements[i] = X;
}

/**
 * @descripttion:   return H.Elements[1]
 * @return {*}
 * @param {PriorityQueue} H
 */
ElementType FindMin(PriorityQueue H)
{
    return H->Elements[1];
}

/**
 * @descripttion:  上滤
 * @return {*}
 * @param {PriorityQueue} H
 * @param {int} i
 */
void PercolateUp(PriorityQueue H, int i)
{
    int j = i, Parent = 0;
    ElementType k = 0;
    for(j=i, Parent=j>>1; Parent >= 1; j=Parent, Parent>>=1)
    {
        if(H->Elements[j] < H->Elements[Parent])
        {
            k = H->Elements[j];
            H->Elements[j] = H->Elements[Parent];
            H->Elements[Parent] = k;
        }
    }
}

/**
 * @descripttion:   下滤
 * @return {*}
 * @param {PriorityQueue} H
 * @param {int} i
 */
void PercolateDown(PriorityQueue H, int i)
{
    int j = i, Child = 0;
    ElementType k = 0;
#if 1
    for(j=i, Child=j<<1; Child <= H->Size; j=Child, Child<<=1)
    {
        // Find smaller child
        if(Child != H->Size && H->Elements[Child+1] < H->Elements[Child])
            Child++;

        if(H->Elements[j] > H->Elements[Child])
        {
            k = H->Elements[j];
            H->Elements[j] = H->Elements[Child];
            H->Elements[Child] = k;
        }
    }

#else
    for(j=i, Child=j<<1; Child <= H->Size; j=Child, Child<<=1)
    {
        // Find smaller child
        if(Child != H->Size && H->Elements[Child+1] < H->Elements[Child])
            Child++;

        if(H->Elements[i] > H->Elements[Child])
        {
            H->Elements[j] = H->Elements[Child];
        }
        else
        {
            break;
        }
    }
    H->Elements[j] = H->Elements[i];
#endif
}

/**
 * @descripttion:   删除最小元素
 * @return {*}
 * @param {PriorityQueue} H
 */
ElementType DeleteMin(PriorityQueue H)
{
    if(IsEmpty(H))
    {
        printf("Priority queue is empty.\n");
        return H->Elements[0];
    }

    int i=0, Child=0;
    ElementType MinElement, LastElement;

    MinElement = H->Elements[1];
    LastElement = H->Elements[H->Size--];

    
    for(i=1,Child=i<<1; Child <= H->Size; i=Child, Child=i<<1)
    {
        // Find smaller child
        if(Child != H->Size && H->Elements[Child+1] < H->Elements[Child])
            Child++;

        // Percolate one level
        if(LastElement > H->Elements[Child])
            H->Elements[i] = H->Elements[Child];
        else
            break;
    }

    H->Elements[i] = LastElement;

    return MinElement;
}

/**
 * @descripttion:   下滤法构建堆
 * @return {*}
 * @param {ElementType} *Arr
 * @param {int} N
 */
PriorityQueue BuildHeap(ElementType *Arr, int N)
{
    PriorityQueue H = Initialize(N);
    memcpy(&H->Elements[1], Arr, N*sizeof(ElementType));
    H->Size = N;
    int i = 0;
    for(i=N>>1; i>0; --i)
        PercolateDown(H, i);
    return H;
}

void ShowElements(PriorityQueue H)
{
    int i = 0;
    for(i=1; i<=H->Size; ++i)
    {
        printf("%d ", H->Elements[i]);
        if(i%10 == 0)
            printf("\n");
    }
    printf("\n");
}

void Destroy(PriorityQueue H)
{
    if(H)
    {
        free(H->Elements);
        H->Elements = NULL;
        H->Capacity = 0;
        H->Size = 0;
        free(H);
    }
}