#ifndef _MERGE_LIST_H
#define _MERGE_LIST_H

typedef struct PNode
{
    int     expn;       // 指数
    // float   coef;       // 系数
    struct PNode *next;
}PNode, *pLink;

// 在聊表H 中查找元素elem
int search(pLink H, int elem);
// 将链表B合并到链表A中，只对B中元素去重
pLink mergeList(pLink A, pLink B);
// 新建结点
pLink newNode();
// 根据已知元素创建链表
pLink createLink(int *datas, int num);
// 随机生成int数组
int *randArray(int n);
// 遍历链表
void traverseList(pLink H, char *info);
// 释放俩表
void freeLink(pLink *H);
// 将B链表合并到链表A
void union2Lists(int *datasA, int numA, int *datasB, int numB);
// 对链表的有序插入 type:0-不允许有重复元素 1-允许重复元素
pLink insertSorted(pLink h, pLink p, int type);
// 创建有序链表
pLink createLinkSorted(int *datas, int num, int type);
// 合并有序链表A、B
pLink mergeListSorted(pLink A, pLink B);
// 合并有序链表A、B
void unionSortedLists(int *datasA, int numA, int *datasB, int numB, int type);


#endif