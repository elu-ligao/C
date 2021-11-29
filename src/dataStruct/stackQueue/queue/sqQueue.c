#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "SqQueue.h"

/**
 * @Descripttion:  初始化创建空队列时，令front=rear=0
 * @Params      : 
 * @Return      :  Q and fail with NULL
 */
pSqQueue initQueue()   
{
    pSqQueue Q = (pSqQueue)calloc(1, sizeof(SqQueue));
    if(!Q)
    {
        perror("initQueue error for calloc memory.\n");
        return NULL;
    }
    Q->base = (pElemType *)calloc(MAX_QUEUE_SIZE, sizeof(pElemType));
    if(!Q->base)
    {
        perror("calloc for base memory error.\n");
        return NULL;
    }
    int i = 0;
    for(; i<MAX_QUEUE_SIZE; ++i)
    {
        Q->base[i] = (pElemType)calloc(1, sizeof(ElemType));
        if(!Q->base[i]) return NULL;
    }
    Q->front = Q->rear = 0;
    return Q;
}

/**
 * @Descripttion:   销毁队列
 * @Params      : 
 * @Return      :   
 * @param {pSqQueue} *Q
 */
void destroyQueue(pSqQueue *Q)      
{
    clearQueue(*Q);
    if(*Q)
    {
        if((*Q)->base)
        {
            int i = 0;
            for(; i<MAX_QUEUE_SIZE; ++i)
            {
                if((*Q)->base[i])
                {
                    free((*Q)->base[i]);
                    (*Q)->base[i] = NULL;
                }
            }
            (*Q)->base = NULL;
        }
        free(*Q);
        *Q = NULL;
    }
}

/**
 * @Descripttion:   清空队列
 * @Params      : 
 * @Return      : 
 * @param {pSqQueue} Q
 */
void clearQueue(pSqQueue Q)         
{
    if(Q)
    {
        if(Q->base)
        {
            int i = 0;
            for(; i<MAX_QUEUE_SIZE; ++i)
            {
                memset(Q->base[i], 0, sizeof(ElemType));
            }
        }
        Q->front = Q->rear = 0;
    }
}

/**
 * @Descripttion:   队列是否为空队列
 * @Params      : 
 * @Return      :   true for Q is Empty, false for Q is not Exist or not Empty
 *                  Q->rear == Q->front
 * @param {pSqQueue} Q
 */
bool queueIsEmpty(pSqQueue Q)      
{
    if(!Q)  return false;
    
    return Q->rear == Q->front;
}

/**
 * @Descripttion:   队列是否已满
 * @Return      :   true is full false not full 
 *                  (rear-1)%MAX == front
 * @param {pSqQueue} Q
 */
bool queueIsFull(pSqQueue Q)      
{
    if(!Q)  return false;
    
    return (Q->rear + 1)%MAX_QUEUE_SIZE == Q->front;
}

/**
 * @Descripttion:   队列中元素的个数
 * @Return      :   (rear-front+MAX)%MAX
 * @param {pSqQueue} Q
 */
int  queueLength(pSqQueue Q)       
{
    if(!Q)  return 0;
    return (Q->rear - Q->front + MAX_QUEUE_SIZE)%MAX_QUEUE_SIZE;
}

/**
 * @Descripttion:   获取队头元素
 * @Return      :   pElemType 队头元素对象
 * @param {pSqQueue} Q
 */
pElemType getHead(pSqQueue Q)       
{
    if(!Q || queueIsEmpty(Q))  return NULL;
    return Q->base[Q->front];
}

/**
 * @Descripttion:   将元素e入队
 * @Return      :   0 for success -1 for fail
 * @param {pSqQueue} Q
 * @param {pElemType} e
 */
int  enQueue(pSqQueue Q, pElemType e)   
{
    if(queueIsFull(Q)) return -1;
    memcpy(Q->base[Q->rear]->name, e->name, sizeof(e->name)-1);
    Q->base[Q->rear]->age = e->age;
    Q->base[Q->rear]->sex[0] = e->sex[0];
    Q->rear = (Q->rear+1)%MAX_QUEUE_SIZE;
    return 0;
}


/**
 * @Descripttion:   队头元素出队 
 * @Return      :   pElemType 队头元素对象指针
 * @param {pSqQueue} Q
 */
pElemType deQueue(pSqQueue Q)       
{
    if(queueIsEmpty(Q))    return NULL;
    pElemType e = Q->base[Q->front];
    Q->front = (Q->front+1)%MAX_QUEUE_SIZE;
    return e;
}

/**
 * @Descripttion:   遍历队列元素
 * @Return      : 
 * @param {pSqQueue} Q
 */
void queueTraverse(pSqQueue Q)      
{
    if(!Q)  return;
    int front = Q->front;
    while(front != Q->rear)
    {
        printf("name: %s, age: %3d, sex: %c\n", Q->base[front]->name, Q->base[front]->age, Q->base[front]->sex[0]);
        front = (front+1)%MAX_QUEUE_SIZE;
    }
}