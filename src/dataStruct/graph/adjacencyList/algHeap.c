#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "algHeap.h"

#if 1


/**
 * @descripttion:   为 pElemHeap 申请 total 大小的 ElemHeap 空间
 * @return {*}
 * @param {int} desc    0 小的放最前面  1 大的放前面
 * @param {int} total   开辟 total 的 ElemHeap 空间
 */
pHeap newHeap(int desc, int total)
{
    pHeap H = (pHeap)calloc(1, sizeof(Heap));
    if(!H)
    {
        perror("newHeap error.\n");
        return NULL;
    }
    H->size = 1;
    H->desc = desc;
    H->total = total;
    H->data = (pElemHeap)calloc(total+1, sizeof(ElemHeap));
    if(!H->data)
    {
        perror("calloc for pElemHeap error.\n");
        return NULL;
    }
    H->vexs = (int *)calloc(total, sizeof(int));
    return H;
}


/**
 * @descripttion:   当size > total 时，重新分配 total + REALLOC_NUM 的 ElemHeap 空间
 * @return {*}
 */
pHeap reallocHeap(pHeap H)
{
    H->total += REALLOC_NUM;
    H->data = (pElemHeap)realloc(H->data, (H->total + 1) * sizeof(ElemHeap));
    if(!H->data)
    {
        perror("realloc for pElemHeap error.\n");
        return NULL;
    }
    H->vexs = (int *)realloc(H->vexs, (H->total)*sizeof(int));
    return H;
} 

/**
 * @descripttion:   初始化 Heap
 * @return {*}
 * @param {int} desc
 * @param {int} total
 */
pHeap initHeap(int desc, int total)
{
    pHeap H = newHeap(desc, total);
    return H;
}

/**
 * @descripttion:   判断 Heap 是否已满 size == total
 * @return {*}
 * @param {pHeap} H
 */
bool heapFull(pHeap H)
{
    return H->size > H->total;
}

void bubbleUp(pHeap H, int idx)
{
    int phead = -1, chead = -1;
    ElemHeap t;
    int parent = idx >> 1;
    if(H->desc == 1)    // 最大的在头元素
    {
        while(parent >= 1 && H->data[parent].lowcost < H->data[idx].lowcost)
        {
            phead = H->data[parent].head;
            chead = H->data[idx].head;
            memcpy(&t, &H->data[parent], sizeof(ElemHeap));
            memcpy(&H->data[parent], &H->data[idx], sizeof(ElemHeap));
            H->vexs[chead] = parent;
            memcpy(&H->data[idx], &t, sizeof(ElemHeap));
            H->vexs[phead] = idx;
            idx = parent;
            parent = idx >> 1;
        }
    }
    else        // 最小的元素在前面
    {
        while(parent >= 1 && H->data[parent].lowcost > H->data[idx].lowcost)
        {
            phead = H->data[parent].head;
            chead = H->data[idx].head;
            memcpy(&t, &H->data[parent], sizeof(ElemHeap));
            memcpy(&H->data[parent], &H->data[idx], sizeof(ElemHeap));
            H->vexs[chead] = parent;
            memcpy(&H->data[idx], &t, sizeof(ElemHeap));
            H->vexs[phead] = idx;
            idx = parent;
            parent = idx >> 1;
        }
    }

}


/**
 * @descripttion:   将对象e 插到 Heap 中合适位置
 * @return {*}
 * @param {pHeap} H
 * @param {pElemHeap} e
 */
bool insertHeap(pHeap H, pElemHeap e)
{
    // printf("H.size = %d, H.total = %d\n", H->size, H->total);
    if(heapFull(H)) return false;

// printf("-- insert e->head = %d, H.vex[%d] = %d\n", e->head, H->data[e->head].head, H->vexs[H->data[e->head].head]);
    int idx = H->size;
    H->size += 1;
    H->vexs[e->head] = idx;
    memcpy(&H->data[idx], e, sizeof(ElemHeap));

    if(idx == 1)    return true;

    bubbleUp(H, idx);
// printf("++ insert e->head = %d, H.vex[%d] = %d\n", e->head, H->data[e->head].head, H->vexs[H->data[e->head].head]);

    return true;
}


bool heapEmpty(pHeap H)
{
    return H->size == 1;
}


void bubbleDown(pHeap H, int k)
{
    int change, left, right, parent = k;
    bool b = true;
    ElemHeap t;
    int phead = -1, chead = -1;
    if(H->desc == 1)    // 大的在前面
    {
        while(b)
        {
            left = parent << 1;
            if(left >= H->size)  break;
            right = left + 1;
            if(right >= H->size)
                change = left;
            else if(H->data[left].lowcost > H->data[right].lowcost)
                change = left;
            else
                change = right;

            if(H->data[parent].lowcost < H->data[change].lowcost)
            {
                phead = H->data[parent].head;
                chead = H->data[change].head;
                memcpy(&t, &H->data[parent], sizeof(ElemHeap));
                memcpy(&H->data[parent], &H->data[change], sizeof(ElemHeap));
                H->vexs[chead] = parent;
                memcpy(&H->data[change], &t, sizeof(ElemHeap));
                H->vexs[phead] = change;
                parent = change;
            }
            else
                b = false;
        }
    }
    else
    {
        while(b)
        {
            left = parent << 1;
            if(left >= H->size)  break;
            right = left + 1;
            if(right >= H->size)
                change = left;
            else if(H->data[left].lowcost < H->data[right].lowcost)
                change = left;
            else
                change = right;

            if(H->data[parent].lowcost > H->data[change].lowcost)
            {
                phead = H->data[parent].head;
                chead = H->data[change].head;
                memcpy(&t, &H->data[parent], sizeof(ElemHeap));
                memcpy(&H->data[parent], &H->data[change], sizeof(ElemHeap));
                H->vexs[chead] = parent;
                memcpy(&H->data[change], &t, sizeof(ElemHeap));
                H->vexs[phead] = change;
                parent = change;
            }
            else
                b = false;
        }
    }

}

/**
 * @descripttion:   将 Heap 中最开头的元素弹出
 * @return {*}
 * @param {pHeap} H
 */
pElemHeap extractFirst(pHeap H)
{
    if(heapEmpty(H))    return NULL;

    pElemHeap e = (pElemHeap)calloc(1, sizeof(ElemHeap));
    memcpy(e, &H->data[1], sizeof(ElemHeap));
    
    H->size -= 1;       // 弹出首位元素，size-1
    int idx = H->size;  // 此时idx是当前的最后一个元素
    int extractidx = H->data[1].head;
// printf("-- extract k = %d, H.vex[%d] = %d\n", extractidx, H->data[extractidx].head, H->vexs[H->data[extractidx].head]);
    H->vexs[extractidx] = -1;
    memcpy(&H->data[1], &H->data[idx], sizeof(ElemHeap));
    int chead = H->data[idx].head;
    H->vexs[chead] = 1;
    memset(&H->data[idx], 0, sizeof(ElemHeap));

    bubbleDown(H, 1);
// printf("++ extract k = %d, H.vex[%d] = %d\n", extractidx, H->data[extractidx].head, H->vexs[H->data[extractidx].head]);

    return e;
}

/**
 * @descripttion:   返回 Heap 中最开头的元素
 * @return {*}
 * @param {pHeap} H
 */
pElemHeap findFirst(pHeap H)
{
    return &H->data[1];
}


/**
 * @descripttion:   删除索引为k的元素
 * @return {*}
 * @param {pHeap} H
 * @param {int} k
 */
pElemHeap deleteHeap(pHeap H, int k)
{
    if(k < 1 || k >= H->size)   return NULL;
    H->size -= 1;
    int idx = H->size;
    pElemHeap e = (pElemHeap)calloc(1, sizeof(ElemHeap));
    memcpy(e, &H->data[k], sizeof(ElemHeap));
    
// printf("-- delete k = %d, H.vex[%d] = %d\n", k, H->data[k].head, H->vexs[H->data[k].head]);
    H->vexs[H->data[k].head] = -1;
    memcpy(&H->data[k], &H->data[idx], sizeof(ElemHeap));
    H->vexs[H->data[idx].head] = k;

    bubbleDown(H, k);
// printf("++ delete k = %d, H.vex[%d] = %d\n", k, H->data[k].head, H->vexs[H->data[k].head]);

    return e;
}


void destroyHeap(pHeap *H)
{
    free((*H)->data);
    (*H)->data = NULL;
    free(*H);
    *H = NULL;
}

void showHeap(pHeap H)
{
    int i, n = H->size;
    for(i=1; i<n; ++i)
    {
        printf("%d: size=%d, head=%d, tail=%d, lowcost=%d\n", i, H->size, H->data[i].head, H->data[i].tail, H->data[i].lowcost);
    }
}
#endif