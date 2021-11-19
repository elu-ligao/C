#ifndef _BST_DYNAMIC_H
#define _BST_DYNAMIC_H

#include "bst.h"

#define BST_DLL_PATH     "/mnt/e/lg/vscode/lib/libbst.so"


/*
dlsym根据动态链接库操作句柄(pHandle)与符号(symbol),返回符号对应的地址。
使用这个函数不但可以获取函数地址，也可以获取变量地址。比如，假设在so中
定义了一个void mytest()函数，那在使用so时先声明一个函数指针：
void (*pMytest)(),然后使用dlsym函数将函数指针pMytest指向mytest函数，
pMytest = (void (*)())dlsym(pHandle, "mytest");
*/

// 定义函数指针类型的别名
// pbstElement (*newBstElementHandle)();
typedef pbstElement(*newBstElementHandle)();           // 新建节点
typedef pbsTree (*newBstNodeHandle)();                 // 创建二叉树节点
typedef pbsTree (*createBSTreeHandle)(pbstElement elemlists, int n);      // 创建二叉树
typedef pbsTree (*searchBstHandle)(pbsTree T, pbstElement key);    // 查找关键字域为key的节点
typedef pbsTree (*minBstHandle)(pbsTree T);            // 返回最小键值的对象
typedef pbsTree (*maxBstHandle)(pbsTree T);            // 返回最大键值的对象
typedef pbsTree (*predecessorBstHandle)(pbsTree T, pbstElement key);     // 返回键值仅次于(更小)给定键的对象
typedef pbsTree (*successorBstHandle)(pbsTree T, pbstElement key);       // 返回键值仅大于(更大)给定键的对象
// typedef void (*outputSortedBstHandle)(pbsTree T, int *index, int rlFlag, int prtFlag);   // 中序遍历(调用前index的值务必设为0)
typedef int (*insertBstHandle)(pbsTree *T, pbstElement e, pbsTree p);   // 插入
typedef int (*deleteBstHandle)(pbsTree *T, pbstElement e);   // 删除
typedef void (*freeBstHandle)(pbsTree *T);                   // 释放
typedef void (*drawBsTreeHandle)(pbsTree T, int prtFlag);                 // 以树的形式打印bst


// 声明调用函数指针变量
extern void *bstHandle;
extern newBstElementHandle newBstElementFunc;
extern newBstNodeHandle newBstNodeFunc;
extern createBSTreeHandle createBSTreeFunc;
extern searchBstHandle searchBstFunc;
extern minBstHandle minBstFunc;
extern maxBstHandle maxBstFunc;
extern predecessorBstHandle predecessorBstFunc;
extern successorBstHandle successorBstFunc;
// extern outputSortedBstHandle outputSortedBstFunc;
extern insertBstHandle insertBstFunc;
extern deleteBstHandle deleteBstFunc;
extern freeBstHandle freeBstFunc;
extern drawBsTreeHandle drawBsTreeFunc;


// 初始化bst动态库
int initBstHandle();
void closeBstHandle();

#endif