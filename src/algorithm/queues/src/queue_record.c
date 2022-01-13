/*
 * @Author       : elu
 * @Date         : 2022-01-10 15:31:36
 * @LastEditTime : 2022-01-10 15:54:49
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue_record.h"



/**
 * @descripttion:   队列是否为空队列
 * @return {*}
 * @param {Queue} Q
 */
int IsEmpty(Queue Q)
{
    return Q->Size == 0;
}

/**
 * @descripttion:   队列是否已满
 * @return {*}
 * @param {Queue} Q
 */
int IsFull(Queue Q)
{
    return Q->Size == Q->Capacity;
}

/**
 * @descripttion:   创建一个可容纳MaxElements个元素的队列
 * @return {*}
 * @param {int} MaxElements
 */
Queue CreateQueue(int MaxElements)
{
    Queue Q = (Queue)calloc(1, sizeof(struct QueueRecord));
    if(NULL == Q)
    {
        perror("CreateQueue error.\n");
        return NULL;
    }
    if(MaxElements < MAX_QUEUE_SIZE)
    {
        printf("%d is to small, %d is seted to be default.\n", MaxElements, MAX_QUEUE_SIZE);
        MaxElements = MAX_QUEUE_SIZE;
    }
    Q->Array = (PtrElem)calloc(MaxElements, sizeof(ElemType));
    if(NULL == Q->Array)
    {
        perror("calloc for Queue.Array error.\n");
        return NULL;
    }

    Q->Capacity = MaxElements;
    
    return Q;
}

/**
 * @descripttion:   销毁队列
 * @return {*}
 * @param {Queue} Q
 */
void DisposeQueue(Queue Q)
{
    if(NULL == Q)   return;
    MakeEmpty(Q);
    free(Q->Array);
    Q->Array = NULL;
    free(Q);
    Q = NULL;
}

/**
 * @descripttion:   清空队列
 * @return {*}
 * @param {Queue} Q
 */
void MakeEmpty(Queue Q)
{
    memset(Q->Array, 0, Q->Capacity*sizeof(ElemType));
    Q->Front = 0;
    Q->Rear = 0;
    Q->Size = 0;
}

void CopyElementData(PtrElem To, PtrElem From)
{
    memcpy(To, From, sizeof(ElemType));
}

int Succ(int Value, Queue Q)
{
    if(++Value == Q->Capacity)
        Value = 0;
    return Value;
}

/**
 * @descripttion:   元素X 入队
 * @return {*}
 * @param {PtrElem} X
 * @param {Queue} Q
 */
void Enqueue(PtrElem X, Queue Q)
{
    if(IsFull(Q))
    {
        perror("Full Queue.\n");
        return;
    }
    Q->Size++;
    Q->Rear = Succ(Q->Rear, Q);
    CopyElementData(&Q->Array[Q->Rear], X);
}

/**
 * @descripttion:   返回队首元素指针
 * @return {*}
 * @param {Queue} Q
 */
PtrElem Front(Queue Q)
{
    return &Q->Array[Q->Front];
}

/**
 * @descripttion:   出队
 * @return {*}
 * @param {Queue} Q
 */
void Dequeue(Queue Q)
{
    memset(&Q->Array[Q->Front], 0, sizeof(ElemType));
    Q->Front++;
    Q->Size--;
}

/**
 * @descripttion:   元素出队并将其值返回 
 * @return {*}
 * @param {Queue} Q
 */
PtrElem FrontAndDequeue(Queue Q)
{
    PtrElem E = (PtrElem)calloc(1, sizeof(ElemType));
    if(NULL == E)   return NULL;

    CopyElementData(E, &Q->Array[Q->Front]);
    Dequeue(Q);
    return E;
}