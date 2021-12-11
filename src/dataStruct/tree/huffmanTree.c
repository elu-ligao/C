#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "huffmanTree.h"


/**
 * @descripttion:   初始化有n个叶子结点的哈夫曼树
 * @return {*}
 * @param {int} n
 */
pHuffmanTree initHuffmanTree(int n)
{
    pHuffmanTree HT = (pHuffmanTree)calloc(n<<1, sizeof(HTNode));
    if(!HT)
    {
        perror("callic for pHuffmanTree error.\n");
        return NULL;
    } 
    HT[0].weight = n;   // 头结点存放叶子结点的个数，总结点数为2n-1
    return HT;
}


/**
 * @descripttion:   创建哈夫曼树
 * @return {*}
 */
pHuffmanTree createHuffmanTree(int *weights, int n)
{
    pHuffmanTree HT = initHuffmanTree(n);
    if(!HT) return NULL;

    int i = 0;
    for(i=0; i<n; ++i)      // 初始化n个结点的值，存放在HT中第1个元素之后
    {
        HT[i+1].weight = weights[i];
    }

    int m = n<<1;
    int *smallNodes = (int *)calloc(2, sizeof(int));
    for(i=n+1; i<m; ++i)
    {
        smallNodes = selectTwoSmallest(HT, i-1);    // 选择权值最小的两个结点作为组成新结点i的子树

        HT[smallNodes[0]].parent = i;   // 更新这两个最小权值结点的父节点为当前创建的结点i
        HT[smallNodes[1]].parent = i;

        HT[i].lchild = smallNodes[0];   // 更新当前结点i的左右子节点为这两个权值最小的结点
        HT[i].rchild = smallNodes[1];

        HT[i].weight = HT[smallNodes[0]].weight + HT[smallNodes[1]].weight; // 更新新结点的权值为选择的两个结点权值之和
    }

    updatePathlen(HT, m-1);  // 更新路径长度

    return HT;
}

void updatePathlen(pHuffmanTree HT, int m)
{
    int left = HT[m].lchild;
    int right = HT[m].rchild;
    if(0 == left || 0 == right) 
        return;

    HT[left].pathlen = HT[m].pathlen + 1; 
    HT[right].pathlen = HT[m].pathlen + 1; 

    updatePathlen(HT, left);
    updatePathlen(HT, right);
}

/**
 * @descripttion:   在1-n之间选择parent=0且权值最小的两个结点并返回
 * @return {*}
 * @param {pHuffmanTree} HT
 * @param {int} n
 */
int *selectTwoSmallest(pHuffmanTree HT, int n)
{
    int *small2 = (int *)calloc(2, sizeof(int));

    int first = 0, second = 0;
    int i = 1;
    for(i=1; i<=n; ++i)
    {
        if(0 == HT[i].parent)
        {
            if(0 == first)  // 先把第一个值赋给first
            {
                first = i;
                continue;
            }
            if(0 == second) // 第二小的值也未确定时
            {
                if(HT[i].weight < HT[first].weight)     // 当前值比第一个值更小
                {
                    second = first; 
                    first = i;
                }
                else 
                {
                    second = i;
                }
                continue;
            }
            if(HT[i].weight < HT[second].weight)    // 当前比second小
            {
                if(HT[i].weight < HT[first].weight) // 且比first小
                {
                    second = first;
                    first = i;
                }
                else 
                {
                    second = i;
                }
                continue;
            }
        }
    }
    small2[0] = first;
    small2[1] = second;

    return small2;
}

/**
 * @descripttion:   输出哈夫曼树数组详细信息
 * @return {*}
 * @param {pHuffmanTree} HT
 */
void showHuffman(pHuffmanTree HT)
{
    int n = HT[0].weight << 1;
    int i = 1;
    printf("index weight pathlen parent lchild rchild:\n");
    for(i=1; i<n; ++i)
    {
        printf("%-5d %-5d %-7d %-6d %-6d %-6d\n", 
        i, HT[i].weight, HT[i].pathlen, HT[i].parent, HT[i].lchild, HT[i].rchild);
    }
}

/**
 * @descripttion:   求结点m的带权路径
 * @return {*}
 * @param {pHuffmanTree} HT
 * @param {int} m
 */
int nodeWPL(pHuffmanTree HT, int m)
{
    return HT[m].weight * HT[m].pathlen;
}

/**
 * @descripttion:   求哈夫曼树的带权路径长度(所有叶子结点的带权路径长度之和)
 * @return {*}
 * @param {pHuffmanTree} HT
 */
int WPL(pHuffmanTree HT)
{
    int n = HT[0].weight;
    int wpl = 0;
    int i = 0;
    for(i=1; i<=n; ++i)
    {
        wpl += nodeWPL(HT, i);
    }
    return wpl;
}

/**
 * @descripttion:   根据哈夫曼树构造哈夫曼编码
 *              以叶子结点出发，向上回溯至根节点为止
 *              回溯时结点时父节点的左分支生成代码0，是右分支生成代码1
 * @return {*}
 * @param {pHuffmanTree} HT
 */
pHuffmanCode createHufmanCode(pHuffmanTree HT)
{
    int num = HT[0].weight;       // 所有叶子结点的数量
    pHuffmanCode HC = (pHuffmanCode)calloc(num+1, sizeof(char *));
    // char cd[num];
    // memset(cd, 0, num);
    char *cd = (char *)calloc(num, sizeof(char));
    sprintf(cd, "%d", num);

    int n = strlen(cd) + 1;
    HC[0] = (char *)calloc(n, sizeof(char));       // 0号元素存放叶子结点的数量
    strcpy(HC[0], cd);
    
    int start = 0;
    int i = 0, cur = 0, parent = 0;
    for(i=1; i<=num; ++i)     
    {
        memset(cd, 0, num); 
        start = num-1;          // 从最后一个位置开始存放，最后一个是结束符
        cur = i;                // 当前结点cur
        parent = HT[i].parent;  // 父节点

        while(0 != parent)                  // 未到根节点循环执行
        {
            --start;                        // 存放位置往前回溯
            if(HT[parent].lchild == cur)    // 如果当前结点是父节点的左孩子，生成代码0
                cd[start] = '0';
            else                            // 为父节点右孩子生成代码1
                cd[start] = '1';
            cur = parent;                   // 往父节点回溯
            parent = HT[parent].parent;
        }

        HC[i] = (char *)calloc(HT[i].pathlen+1, sizeof(char));    // 为第i个结点申请空间存放编码
        strcpy(HC[i], cd+start);
    }

    memset(cd, 0, num);     // 释放临时空间
    free(cd);
    cd = NULL;
    return HC;
}


void showHuffmanCode(pHuffmanCode HC)
{
    if(!HC) return;
    int n = atoi(HC[0]);
    int i = 0;
    printf("index code\n");
    for(i=1; i<=n; ++i)
    {
        printf("%-5d %s\n", i, HC[i]);
    }
}


void freeHT(pHuffmanTree *HT)
{
    if(!HT) return;
    int n = (*HT)[0].weight << 1;
    memset(*HT, 0, sizeof(n * sizeof(HTNode)));
    free(*HT);
    *HT = NULL;
}

void freeHC(pHuffmanCode HC)
{
    if(!HC) return;
    int n = atoi(HC[0]);

    int i = 0;
    for(i=0; i<=n; ++i)
    {
        memset(HC[i], 0, strlen(HC[i])+1);
        free(HC[i]);
        HC[i] = NULL;
    }
    free(HC);
    HC = NULL;
}