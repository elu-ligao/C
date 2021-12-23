#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphQueue.h"


/**
 * @descripttion:   新建结点
 * @return {*}  pQueueNode 指针
 */
pQueueNode newQueueNode()
{
    pQueueNode p = (pQueueNode)calloc(1, sizeof(QueueNode));
    if(!p)
    {
        perror("newQueueNode calloc memory error.\n");
        return NULL;
    }
    p->data = (pElemType)calloc(1, sizeof(ElemType));
    if(!p->data)
    {
        perror("calloc Element Node memory error.\n");
        return NULL;
    }
    return p;
}


/**
 * @descripttion:   初始化创建空队列
 * @return {*}  pLinkQueue 指针
 */
pLinkQueue initQueue()
{
    pLinkQueue Q = (pLinkQueue)calloc(1, sizeof(LinkQueue));
    if(!Q)
    {
        perror("initQueue calloc memory error.\n");
        return NULL;
    }
    Q->front = newQueueNode();
    if(!Q->front)   return NULL;
    Q->rear = Q->front;
    return Q;
} 

/**
 * @descripttion:   销毁队列
 * @return {*}
 * @param {pLinkQueue} *Q
 */
void destroyQueue(pLinkQueue *Q)
{
    if(!(*Q))   return;
    if((*Q)->front == (*Q)->rear)
    {
        free(*Q);
        *Q = NULL;
        return;
    }
    pQueueNode p = (*Q)->front->next;
    pQueueNode r = NULL;
    while(p)
    {
        r = p->next;
        free(p);
        p = NULL;
        p = r;
    }
    (*Q)->front = NULL;
    (*Q)->rear = NULL;
    free(*Q);
    *Q = NULL;
} 

/**
 * @descripttion:   清空队列
 * @return {*}
 * @param {pLinkQueue} Q
 */
void clearQueue(pLinkQueue Q)
{
    if(!Q)   return;
    if(Q->front == Q->rear)
    {
        return;
    }
    pQueueNode p = Q->front->next;
    pQueueNode r = NULL;
    while(p)
    {
        r = p->next;
        free(p);
        p = NULL;
        p = r;
    }
    Q->rear = Q->front;
    Q->size = 0;
}

/**
 * @descripttion:   队列为空队列
 * @return {*}
 * @param {pLinkQueue} Q
 */
bool queueIsEmpty(pLinkQueue Q)
{
    if(!Q)  return false;
    return Q->front == Q->rear;
}

/**
 * @descripttion:   队列中元素的个数
 * @return {*}
 * @param {pLinkQueue} Q
 */
int  queueLength(pLinkQueue Q)
{
    if(!Q)  return 0;
    return Q->size;
}

/**
 * @descripttion:   获取队头元素
 * @return {*}
 * @param {pLinkQueue} Q
 */
pElemType getHead(pLinkQueue Q)
{
    pElemType e = (pElemType)calloc(1, sizeof(ElemType));
    if(!e)
    {
        perror("calloc for Element memory error.\n");
        return NULL;
    }  
    e->idx = Q->front->next->data->idx;
    return e;
}

/**
 * @descripttion:   将元素e入队
 * @return {*}
 * @param {pLinkQueue} Q
 * @param {pElemType} e
 */
int enQueue(pLinkQueue Q, pElemType e)
{
    pQueueNode q = newQueueNode();
    if(!q)  return -1;
    q->data->idx = e->idx;
    strcpy(q->data->vex, e->vex);
    Q->rear->next = q;
    Q->rear = q;
    Q->size += 1;
    
    return 0;
}

/**
 * @descripttion:   队头元素出队 
 * @return {*}
 * @param {pLinkQueue} Q
 */
pElemType deQueue(pLinkQueue Q)
{
    if(queueIsEmpty(Q)) return NULL;
    pElemType e = (pElemType)calloc(1, sizeof(ElemType));
    if(!e)
    {
        perror("calloc for Element memory error.\n");
        return NULL;
    }  
    pQueueNode q = Q->front->next;
    e->idx = q->data->idx;
    strcpy(e->vex, q->data->vex);
    Q->front->next = q->next;
    if(Q->rear == q)        // 最后一个元素被删除了
    {
        Q->rear = Q->front; // 重置rear = front
    }
    free(q);
    q = NULL;
    Q->size -= 1;
    
    return e;
}

/**
 * @descripttion:   遍历队列元素
 * @return {*}
 * @param {pLinkQueue} Q
 */
void queueTraverse(pLinkQueue Q)
{
    if(queueIsEmpty(Q)) return;
    pQueueNode q = Q->front->next;
    while(q)
    {
        printf("%d[%s] ", q->data->idx, q->data->vex);
        q = q->next;
    }
    printf("\n");
}
