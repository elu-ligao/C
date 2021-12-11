#ifndef _STRING_LIST_DYNAMIC_H
#define _STRING_LIST_DYNAMIC_H

#include "stringList.h"


#define STRINGLIST_DLL_PATH     "/mnt/e/lg/vscode/lib/libstringList.so"


// 定义函数指针类型的别名
typedef int (*indexBFHandle)(char *S, char *T, int pos);     // BF匹配算法

typedef int (*indexKMPHandle)(char *S, char *T, int pos);       // KMP 匹配算法
typedef int *(*getNextHandle)(char *T);         // next移动位数数组
typedef int *(*getNextValHandle)(char *T);      // 改良next数组

// 声明调用函数指针变量
extern indexBFHandle indexBFFunc;

extern indexKMPHandle indexKMPFunc;
// extern getNextHandle getNextFunc;
// extern getNextValHandle getNextValFunc;

// 初始化bst动态库
int initStringListHandle();
void closeStringListHandle();

#endif