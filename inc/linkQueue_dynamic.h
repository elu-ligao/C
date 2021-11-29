/*
 * @Author       : elu
 * @Date         : 2021-11-29 15:21:45
 * @LastEditTime : 2021-11-29 23:02:13
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _STACK_SEQUENCE_DYNAMIC_H
#define _STACK_SEQUENCE_DYNAMIC_H

#include "linkQueue.h"

#define MAX_QUEUE_SIZE   20
#define LINK_QUEUE_DLL_PATH     "/mnt/e/lg/vscode/lib/liblinkQueue.so"


// 定义函数指针类型的别名
typedef pQueueNode (*newQueueNodeHandle)();
typedef pLinkQueue (*initQueueHandle)();   // 初始化创建空队列时，令front=rear=0
typedef void (*destroyQueueHandle)(pLinkQueue *Q);     // 销毁队列
typedef void (*clearQueueHandle)(pLinkQueue Q);        // 清空队列
typedef bool (*queueIsEmptyHandle)(pLinkQueue Q);      // 队列为空队列
typedef int  (*queueLengthHandle)(pLinkQueue Q);       // 队列中元素的个数
typedef pElemType (*getHeadHandle)(pLinkQueue Q);      // 获取队头元素
typedef int  (*enQueueHandle)(pLinkQueue Q, pElemType e);  // 将元素e入队
typedef pElemType (*deQueueHandle)(pLinkQueue Q);      // 队头元素出队 
typedef void (*queueTraverseHandle)(pLinkQueue Q);     // 遍历队列元素

// 声明调用函数指针变量
extern newQueueNodeHandle newQueueNodeFunc;
extern initQueueHandle initQueueFunc;
extern destroyQueueHandle destroyQueueFunc;
extern clearQueueHandle clearQueueFunc;
extern queueIsEmptyHandle queueIsEmptyFunc;
extern queueLengthHandle queueLengthFunc;
extern getHeadHandle getHeadFunc;
extern enQueueHandle enQueueFunc;
extern deQueueHandle deQueueFunc;
extern queueTraverseHandle queueTraverseFunc;


// 初始化bst动态库
int initLinkQueueHandle();
void closeLinkQueueHandle();

#endif