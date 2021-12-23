#ifndef _SQ_HEAP_H
#define _SQ_HEAP_H

#include "gcommon.h"
#include <stdbool.h>


#define REALLOC_NUM 200

typedef Edge ElemHeap;
typedef pEdge pElemHeap;

typedef struct Heap
{
    int desc;   // 0 升序 1 降序
    int size;   // 当前元素个数
    int total;  // 总大小
    pElemHeap data; // 数据对象 
} Heap, *pHeap;


pHeap newHeap(int desc, int total);        // 为 pElemHeap 申请 total 大小的 ElemHeap 空间
pHeap reallocHeap(pHeap H);    // 当size == total 时，重新分配 total + REALLOC_NUM 的 ElemHeap 空间
pHeap initHeap(int desc, int total);       // 初始化 Heap
bool heapFull(pHeap H); // 判断 Heap 是否已满 size == total
bool heapEmpty(pHeap H); // 判断 Heap 是否为空
bool insertHeap(pHeap H, pElemHeap e);  // 将对象e 插到 Heap 中合适位置
pElemHeap extractFirst(pHeap H);    // 将 Heap 中最开头的元素弹出
pElemHeap findFirst(pHeap H);   // 返回 Heap 中最开头的元素
void deleteHeap(pHeap H, int k);    // 删除索引为k的元素
void destroyHeap(pHeap *H); // 销毁Heap
void updateHeap(pHeap H, pElemHeap e, int k);   // 更新第k个元素的值为e
void showHeap(pHeap H);

#endif