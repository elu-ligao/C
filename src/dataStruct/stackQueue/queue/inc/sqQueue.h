/*
 * @Author       : elu
 * @Date         : 2021-12-11 23:06:20
 * @LastEditTime : 2021-12-16 18:57:20
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _SQ_QUEUE_H
#define _SQ_QUEUE_H

#include <stdbool.h>


#define MAX_QUEUE_SIZE   20
typedef struct ElemType
{
    char name[22];
    char sex[2];
    int  age;
}ElemType, *pElemType;

// 循环队列
typedef struct SqQueue
{
    pElemType *base;     // 基地址
    int front;          // 头指针
    int rear;           // 尾指针
}SqQueue, *pSqQueue;


/*
 *  1. 初始化创建空队列时，令front=rear
 *  2. 每当插入新元素时，队尾指针rear+1，每当删除头元素时，头指针front+1
 *  3. 非空队列中，头指针始终指向队头元素，尾指针始终指向队尾元素的下一个位置
 */

pSqQueue initQueue();   // 初始化创建空队列时，令front=rear=0
void destroyQueue(pSqQueue *Q);     // 销毁队列
void clearQueue(pSqQueue Q);        // 清空队列
bool queueIsEmpty(pSqQueue Q);      // 队列为空队列
bool queueIsFull(pSqQueue Q);       // 队列为满队列
int  queueLength(pSqQueue Q);       // 队列中元素的个数
pElemType getHead(pSqQueue Q);      // 获取队头元素
int  enQueue(pSqQueue Q, pElemType e);  // 将元素e入队
pElemType deQueue(pSqQueue Q);      // 队头元素出队 
void queueTraverse(pSqQueue Q);     // 遍历队列元素


#endif