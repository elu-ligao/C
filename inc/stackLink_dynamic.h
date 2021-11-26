#ifndef _STACK_SEQUENCE_DYNAMIC_H
#define _STACK_SEQUENCE_DYNAMIC_H

#include "stackLink.h"

#define STACK_SEQUENCE_DLL_PATH     "/mnt/e/lg/vscode/lib/libstackLink.so"


// 定义函数指针类型的别名
typedef pStackLink (*initStackHandle)();     // 构造一个空栈
typedef void (*destroyStackHandle)(pStackLink *S);   // 销毁栈S
typedef void (*clearStackHandle)(pStackLink S);      // 将S清为空栈
typedef bool (*stackEmptyHandle)(pStackLink S);      // 判断栈是否为空
typedef int  (*stackLengthHandle)(pStackLink S);     // 返回栈的长度
typedef pElemType (*getTopHandle)(pStackLink S);     // 返回栈顶元素
typedef int (*pushHandle)(pStackLink *S, pElemType e);     // 将元素S入栈
typedef pElemType (*popHandle)(pStackLink *S);       // 栈顶元素出栈
typedef void (*stackTraverseHandle)(pStackLink S);   // 遍历栈
typedef pElemType (*newElementHandle)();            // 新建数据结点

// 声明调用函数指针变量
extern initStackHandle initStackFunc;
extern destroyStackHandle destroyStackFunc;
extern clearStackHandle clearStackFunc;
extern stackEmptyHandle stackEmptyFunc;
extern stackLengthHandle stackLengthFunc;
extern getTopHandle getTopFunc;
extern pushHandle pushFunc;
extern popHandle popFunc;
extern stackTraverseHandle stackTraverseFunc;
extern newElementHandle newElementFunc;


// 初始化bst动态库
int initStackLinkHandle();
void closeStackLinkHandle();

#endif