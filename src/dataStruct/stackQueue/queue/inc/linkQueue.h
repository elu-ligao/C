/*
 * @Author       : elu
 * @Date         : 2021-11-29 08:36:11
 * @LastEditTime : 2021-11-29 22:54:34
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _LINK_QUEUQ_H
#define _LINK_QUEUQ_H

#include <stdbool.h>

typedef struct ElemType
{
    int  age;
    char name[23];
    char sex;
}ElemType, *pElemType;

typedef struct QueueNode
{
    pElemType data;
    struct QueueNode *next;
}QueueNode, *pQueueNode;

typedef struct LinkQueue
{
    pQueueNode front;
    pQueueNode rear;
    int size;   // 队列中含有的元素个数
}LinkQueue, *pLinkQueue;

/**
 *  初始化队列使头指针和尾指针都指向头结点
 *  然后头指针指向这个头结点不动，每插入一个元素，在最后面插入，即插在rear后面，将rear指向新元素
 *  头指针一直指向头结点，尾指针非空一直指向最后入队的元素，即队尾，为空则也指向头结点与front相等
*/
pQueueNode newQueueNode();            // 新建结点
pLinkQueue initQueue();   // 初始化创建空队列
void destroyQueue(pLinkQueue *Q);     // 销毁队列
void clearQueue(pLinkQueue Q);        // 清空队列
bool queueIsEmpty(pLinkQueue Q);      // 队列为空队列
int  queueLength(pLinkQueue Q);       // 队列中元素的个数
pElemType getHead(pLinkQueue Q);      // 获取队头元素
int  enQueue(pLinkQueue Q, pElemType e);  // 将元素e入队
pElemType deQueue(pLinkQueue Q);      // 队头元素出队 
void queueTraverse(pLinkQueue Q);     // 遍历队列元素

#endif