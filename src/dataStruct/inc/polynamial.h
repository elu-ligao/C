#ifndef _POLYNAMIAL_H
#define _POLYNAMIAL_H


typedef struct PNode
{
    int     expn;       // 指数
    float   coef;       // 系数
    struct PNode *next;
}PNode, *pLink;



// 随机生成数组
pLink *randArray(int n);
// 遍历链表
void traverseList(pLink H, char *info);
// 释放链表
void freeLink(pLink *H);
// 新建结点
pLink newNode();
// 创建多项式
pLink createPolynamial(pLink *H, int num);
// 多项式加法
pLink polynamialAdd(pLink A, pLink B);
// 多项式减法
pLink polynamialSub(pLink A, pLink B);
// 多项式乘法
pLink polynamialMul(pLink A, pLink B);


#endif