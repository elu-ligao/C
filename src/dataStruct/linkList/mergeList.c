#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mergeList.h"

/*
 *  input:  pLink H 头指针
 *          int elem 待查找元素
 *  output: 
 *  return: success-0   not exist -1
 */
int search(pLink H, int elem)
{
    pLink p = H->next;
    while(p)
    {
        if(p->expn == elem)
            return 0;
        p = p->next;
    }
    return -1;
}

/*
 *  input:      两个链表
 *  output:     两个链表元素的并集
 *  return:     pLink 并集链表头结点
 *  desc:       合并两个链表，返回他们的并集
*/
pLink mergeList(pLink A, pLink B)
{
    pLink pa = A->next, pb = B->next, r = NULL;
    while(pa->next)
        pa = pa->next;  // 现将pa移动到最后一个结点，对B中的元素采用后插追加的方式插入A
    while(pb)
    {
        if(search(A, pb->expn))     // B 中元素不存在A中
        {
            pa->next = pb;      // 将当前结点插在pa后面
            pb = pb->next;      // 分别后移两个指针
            pa = pa->next;
            pa->next = NULL;
            A->expn += 1;
        }
        else if(search(A, pb->expn) == 0)   // B 中的元素在A中已经存在
        {
            r = pb;             // 先保存当前B的结点
            pb = pb->next;      // pb 指针后移
            free(r);            // 释放结点
            r = NULL;
        }
    }
    free(B);        // 释放B的头结点
    B = NULL;
    return A;
}

/*
 *  input:
 *  output:
 *  return:     pLink   空结点
 *  desc:       新建一个空结点，返回结点指针
*/
pLink newNode()
{
    pLink h = (pLink)calloc(1, sizeof(PNode));
    if(!h)
    {
        perror("newNode error\n");
    }
    return h;
}


/*
 *  input:      int *datas  元素数组指针
 *              int num     元素个数
 *  output:     
 *  return:     pLink       新建链表头结点
 *  desc:       新建链表,带头结点，第一个元素在首元结点，即头结点->next
*/
pLink createLink(int *datas, int num)
{
    pLink head = newNode();
    if(!head)
    {
        return NULL;
    }

    pLink p = NULL, q = head;
    int i = 0;
    for(i=0; i<num; ++i)
    {
        p = newNode();
        if(!p) 
        {
            printf("the %d th num %d not create success.\n", i, datas[i]);
            continue;
        } 
        p->expn = datas[i];
        q->next = p;
        q = q->next;
    }
    if(i != num)
    {
        printf("total elements num is %d, not equal the input num %d\n", i, num);
    }
    head->expn = i;     // 头结点存储总元素个数

    return head;
}

int *randArray(int n)
{
    int i = 0;
    int *datas = (int *)calloc(n, sizeof(int));
    if(NULL == datas)   
    {
        perror("calloc for datas error\n");
        return NULL;
    }
    for(i=0; i<n; ++i)
    {
        datas[i] = rand()%100;
    }
    return datas;
}

void traverseList(pLink H, char *info)
{
    if(!H)  return;
    pLink p = H;
    printf("%s total elements: %d\n", info, p->expn);
    p = p->next;
    while(p)
    {
        printf("%d ", p->expn);
        p = p->next;
    }
    printf("\n");
}

void freeLink(pLink *H)
{
    if(!*H)  return;
    pLink p = (*H)->next;
    pLink t = NULL;
    while(p)
    {
        t = p;
        p = p->next;
        free(t);
        t = NULL;
    }
    free(*H);
    *H = NULL;
}



/*
 * desc: 求两个集合的并集 链表实现 
 *
*/
void union2Lists(int *datasA, int numA, int *datasB, int numB)
{
    pLink A = createLink(datasA, numA);
    traverseList(A, "A");
    pLink B = createLink(datasB, numB);
    traverseList(B, "B");

    A = mergeList(A, B);
    traverseList(A, "After merged A");

    freeLink(&A);
    // printf("A = %p\n", A);
}



/**
 * @brief 将结点p顺序插入链表h
 * input:   pLink h 链表头结点
 *          pLink p 数据结点
 *          int type    是否保留相同的元素 0-去重 1-保留
 * output:  pLink h 顺序插入的链表头结点 
 * return: void
 */
pLink insertSorted(pLink h, pLink p, int type)
{
    if(!p)  return h; // 待插入元素为空， 直接返回， 不操作
    if(!h || !h->next)  // 链表头结点为空或不含任何有效元素，即无头结点，新建头结点，将p直接作为首元结点返回
    {
        if(!h)  h = newNode();
        if(h)
        {
            h->next = p;
            h->expn = 1;   // 更新头结点，链表包含元素个数为1
        }
        return h;
    }
    
    // printf("p->expn = %d, type = %d, ", p->expn, type);
    // traverseList(h, "insertSorted");
    // 在h中找到首个元素不小于p的结点的先导结点t
    pLink t = h;
    while(t->next && t->next->expn < p->expn)  t = t->next;
    // printf("t->expn = %d\n", t->expn);
    // 此时t是元素小于p的元素的最大结点，且t->next->expn >= p->expn，故此时把p插入到t结点后面
    if(0 == type && t->next && t->next->expn == p->expn)    return h;   // 如果是要去重，相等的元素略过 
    p->next = t->next;  // 先将p的next 指向 t的next
    t->next = p;        // 然后t的next 指向p
    h->expn += 1;       // 头结点存储总元素个数
    return h;
}



/*
 * input:   int *datas  数据数组地址
 *          int num     元素数据个数
 *          int type    是否保留相同的元素 0-去重 1-保留
 * output:  pLink   有序链表头结点
 * return:  pLink   有序链表头结点
 * desc:    创建有序链表，返回头结点，第一个元素放在首元结点，头结点存储数据个数 
*/
pLink createLinkSorted(int *datas, int num, int type)
{
    pLink head = newNode();
    if(!head)
    {
        return NULL;
    }

    pLink p = NULL;
    int i = 0;
    for(i=0; i<num; ++i)
    {
        p = newNode();
        if(!p) 
        {
            printf("the %d th num %d not create success.\n", i, datas[i]);
            continue;
        } 
        p->expn = datas[i];
        head = insertSorted(head, p, type);
    }
    if(i != num)
    {
        printf("total elements num is %d, not equal the input num %d\n", i, num);
    }

    return head;
}

/**
 * @brief 合并两个顺序链表
 * input:   pLink A, B  两个已经排好序的顺序链表头结点
 * output:  pLink C     并归好的链表
 * desc:    
 */
pLink mergeListSorted(pLink A, pLink B)
{
    pLink pa = A->next, pb = B->next;
    pLink p = A;
    while(pa && pb)
    {
        if(pa->expn > pb->expn)     // pa > pb 将pb 插入到p.next
        {
            p->next = pb;
            pb = pb->next;
            p = p->next;
        }
        else if(pa->expn <= pb->expn)   // pa <= pb 将pa 插入到p.next
        {
            p->next = pa;
            pa = pa->next;
            p = p->next;
        }
    }
    p->next = pa ? pa : pb;     // 将剩余的所有结点追加到p后面   
    A->expn += B->expn;
   
    free(B);        // 释放B的头结点
    B = NULL;
    return A;   
}

/*
 * desc: 求两个有序链表的并集
 * input: int type    是否保留相同的元素 0-去重 1-保留
 * 
*/
void unionSortedLists(int *datasA, int numA, int *datasB, int numB, int type)
{
    pLink A = createLinkSorted(datasA, numA, type);
    traverseList(A, "Sorted A");
    pLink B = createLinkSorted(datasB, numB, type);
    traverseList(B, "Sorted B");

    A = mergeListSorted(A, B);
    traverseList(A, "After Merged Sorted A");

    freeLink(&A);
    // printf("A = %p\n", A);
}

/*
 *  Usage: mergeList numA numB [seed]
 *  num:    元素个数
 *  seed：  随机种子
*/
int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage: %s numA numB [seed] [type]\n", argv[0]);
        return -1;
    }
    else if(argc == 3)
    {
        srand(time(NULL));
    }
    else if(argc >= 4)
    {
        srand(atoi(argv[3]));
    }

    int numA = atoi(argv[1]);
    int numB = atoi(argv[2]);

    int *datasA = randArray(numA);
    if(!datasA)
    {
        printf("rand datasA error\n");
        return -11;
    }

    int *datasB = randArray(numB);
    if(!datasB)
    {
        printf("rand datasA error\n");
        return -11;
    }

    union2Lists(datasA, numA, datasB, numB);
    printf("\n");
    int type = 1;
    if(argc == 5)   type = atoi(argv[4]);
    unionSortedLists(datasA, numA, datasB, numB, type);

    free(datasA);
    datasA = NULL;
    free(datasB);
    datasB = NULL;

    return 0;
}