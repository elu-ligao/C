#ifndef _AVL_DYNAMIC_H
#define _AVL_DYNAMIC_H

#include "avl.h"

#define AVL_DLL_PATH     "/mnt/e/lg/vscode/lib/libavl.so"


/*
dlsym根据动态链接库操作句柄(pHandle)与符号(symbol),返回符号对应的地址。
使用这个函数不但可以获取函数地址，也可以获取变量地址。比如，假设在so中
定义了一个void mytest()函数，那在使用so时先声明一个函数指针：
void (*pMytest)(),然后使用dlsym函数将函数指针pMytest指向mytest函数，
pMytest = (void (*)())dlsym(pHandle, "mytest");
*/

// 定义函数指针类型的别名
typedef pavlElement(*newAvlElementHandle)();           // 新建节点
typedef pavlTree (*newAvlNodeHandle)();                 // 创建二叉树节点
typedef pavlTree (*createAVLTreeHandle)(pavlElement elemlists, int n);      // 创建二叉树
typedef pavlTree (*searchAvlHandle)(pavlTree T, pavlElement key);    // 查找关键字域为key的节点
typedef pavlTree (*minAvlHandle)(pavlTree T);            // 返回最小键值的对象
typedef pavlTree (*maxAvlHandle)(pavlTree T);            // 返回最大键值的对象
typedef pavlTree (*predecessorAvlHandle)(pavlTree T, pavlElement key);     // 返回键值仅次于(更小)给定键的对象
typedef pavlTree (*successorAvlHandle)(pavlTree T, pavlElement key);       // 返回键值仅大于(更大)给定键的对象
// typedef void (*outputSortedAvlHandle)(pavlTree T, int *index, int rlFlag, int prtFlag);   // 中序遍历(调用前index的值务必设为0)
typedef pavlTree (*insertAvlHandle)(pavlTree *root, pavlElement e, int rlFlag, pavlTree p);   // 插入
typedef int (*deleteAvlHandle)(pavlTree *T, pavlElement e);   // 删除
typedef void (*freeAvlHandle)(pavlTree *T);                   // 释放
typedef void (*drawAvlTreeHandle)(pavlTree T, int prtFlag);                 // 以树的形式打印avl
typedef pavlTree (*selectAvlHandle)(pavlTree T, int index);     // 根据索引返回元素值
typedef int (*rankAvlHandle)(pavlTree T, pavlElement e);        // 根据元素值返回索引
typedef int (*isBalanceTreeHandle)(pavlTree T);     // 判断是否为平衡树 
typedef void (*lrMirrorHandle)(pavlTree T);     // 左右镜像



// 声明调用函数指针变量
extern void *avlHandle;
extern newAvlElementHandle newAvlElementFunc;
extern newAvlNodeHandle newAvlNodeFunc;
extern createAVLTreeHandle createAVLTreeFunc;
extern searchAvlHandle searchAvlFunc;
extern minAvlHandle minAvlFunc;
extern maxAvlHandle maxAvlFunc;
extern predecessorAvlHandle predecessorAvlFunc;
extern successorAvlHandle successorAvlFunc;
// extern outputSortedAvlHandle outputSortedAvlFunc;
extern insertAvlHandle insertAvlFunc;
extern deleteAvlHandle deleteAvlFunc;
extern freeAvlHandle freeAvlFunc;
extern drawAvlTreeHandle drawAvlTreeFunc;
extern selectAvlHandle selectAvlFunc;
extern rankAvlHandle rankAvlFunc;
extern isBalanceTreeHandle isBalanceTreeFunc;
extern lrMirrorHandle lrMirrorFunc;


// 初始化avl动态库
int initAvlHandle();
void closeAvlHandle();

#endif