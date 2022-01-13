/*
 * @Author       : elu
 * @Date         : 2022-01-10 15:13:54
 * @LastEditTime : 2022-01-10 15:39:56
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _QUEUE_RECORD_H
#define _QUEUE_RECORD_H

#define ELEMENT_LEN     ( 3 )
#define MAX_QUEUE_SIZE  ( 10 )

struct ElementType
{
    union 
    {
        char sdata[ELEMENT_LEN+1];
        int  idata;
    };
};
typedef struct ElementType ElemType;
typedef ElemType *PtrElem;

struct QueueRecord
{
    int Capacity;   // 容量
    int Front;      // 队首指针
    int Rear;       // 队尾指针
    int Size;       // 现有元素长度
    PtrElem Array;  // 元素数组
};
typedef struct QueueRecord *Queue;

int IsEmpty(Queue Q);   // 队列是否为空队列
int IsFull(Queue Q);    // 队列是否已满
Queue CreateQueue(int MaxElements); // 创建一个可容纳MaxElements个元素的队列
void DisposeQueue(Queue Q); // 销毁队列
void MakeEmpty(Queue Q);    // 清空队列
void Enqueue(PtrElem X, Queue Q);   // 元素X 入队
PtrElem Front(Queue Q);     // 返回队首元素指针
void Dequeue(Queue Q);      // 出队
PtrElem FrontAndDequeue(Queue Q);   // 元素出队并将其值返回 



#endif