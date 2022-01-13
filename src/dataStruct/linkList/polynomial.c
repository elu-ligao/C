#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "polynamial.h"

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

pLink *randArray(int n)
{
    int i = 0;
    pLink *datas = (pLink *)calloc(n, sizeof(pLink));
    if(NULL == datas)   
    {
        perror("calloc for datas error\n");
        return NULL;
    }
    
    for(i=0; i<n; ++i)
    {
        datas[i] = (pLink)calloc(1, sizeof(PNode));
        if(!datas[i])   continue;
        int f = rand()%100 > 49 ? 1 : -1; 
        datas[i]->expn = rand()%n * f;        
        datas[i]->coef = f * rand()%10000 / 100.0;
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
        printf("%+.2f*x^%d ", p->coef, p->expn);
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


void insertPolynamial(pLink h, pLink e)
{
    pLink p = h;
    pLink r = NULL;
    float coef = 0;
    while (p->next && p->next->expn < e->expn)
        p = p->next;        // 先将p移到指数不大于e指数的位置
    if(!p->next)    // p没有指数比e指数大的项，直接把e接到p的next
    {
        p->next = e;
        h->expn += 1;    // 头结点存放元素个数
    }
    else if(p->next->expn == e->expn)   // 指数相等，系数做加法
    {
        coef = p->next->coef + e->coef;
        if(coef > -0.005 && coef < 0.005)   // 系数相加为0
        {
            r = p->next;    // 释放p.next 和 e
            p->next = p->next->next;    // 先将p.next 指向 p.next.next
            freeLink(&r);  
        }
        else
        {
            p->next->coef = coef;
        } 
        freeLink(&e);   // 已经对e处理完毕，可以释放e
    }
    else if(p->next->expn > e->expn)    // 将e插到p后面
    {
        e->next = p->next;
        p->next = e;
        h->expn += 1;    // 头结点存放元素个数
    }

}

pLink createPolynamial(pLink *H, int num)
{
    pLink head = newNode();     // 头结点
    if(!head)   return NULL;
    int i = 0;
    for(i=0; i<num; ++i)
    {
        insertPolynamial(head, H[i]);
    }

    return head;
}

/**
 * @brief 对多项式链表A,B相加，不改动A,B链表，为后续运算保留原始数据
 * 
 * @param A 
 * @param B 
 * @return pLink 
 */
pLink polynamialAdd(pLink A, pLink B)
{
    pLink pa = A->next;
    pLink pb = B->next;
    pLink head = newNode(), r = NULL;
    if(!head)   return NULL;
    pLink p = head;
    float coef = 0;
    while(pa && pb)
    {
        if(pa->expn == pb->expn)    // 指数相等，系数相加
        {
            coef = pa->coef + pb->coef;
            if(coef > -0.005 && coef < 0.005 && pa->expn != 0)   // 相加系数为0
            {
                // 可以释放当前结点，考虑后面还有其他运算，一致到最后释放
                pa = pa->next;  
                pb = pb->next;
            }   // end coef == 0
            else    // 系数不为0，将系数赋值给pa.coef (释放pb当前结点，考虑后面还有其他运算，一致到最后释放)
            {
                p->next = newNode();
                if(!p->next)  return NULL;
                p->next->expn = pa->expn;
                p->next->coef = coef;
                p = p->next;
                pa = pa->next;
                pb = pb->next;
                head->expn += 1;
            }   // end coef != 0
        }   // end pa == pb
        else if (pa->expn < pb->expn)   // pa 的指数更小，将pa当前结点数据接到新链表后
        {
            p->next = newNode();
            if(!p->next)  return NULL;
            p->next->expn = pa->expn;
            p->next->coef = pa->coef;
            p = p->next;
            pa = pa->next;
            head->expn += 1;
        }   // end pa < pb
        else    // pb 的指数更小，将pb当前结点数据接到新链表后
        {
            p->next = newNode();
            if(!p->next)  return NULL;
            p->next->expn = pb->expn;
            p->next->coef = pb->coef;
            p = p->next;
            pb = pb->next;
            head->expn += 1;
        }   // end else pa > pb
        // traverseList(head, "head");
    }   // end while
    // p->next = pa ? pa : pb;     // 将未处理完的数据追加到新链表
    r = pa ? pa : pb;
    while(r)
    {
        p->next = newNode();
        if(!p->next)  return NULL;
        p->next->expn = r->expn;
        p->next->coef = r->coef;
        p = p->next;
        r = r->next;
        head->expn += 1;
    }
    return head;
}


/**
 * @brief 对多项式链表A,B相加，不改动B链表，结果保存在A链表中并作为返回
 * 
 * @param A 
 * @param B 
 * @return pLink A
 */
pLink polynamialAdd2(pLink A, pLink B)
{
    A = A ? A : newNode();
    if(!A)  return NULL;

    pLink pa = A->next;
    pLink pb = B;
    pLink p = A, r = NULL;
    float coef = 0;
    int num = 0;
    while(pa && pb)
    {
        if(pa->expn == pb->expn)    // 指数相等，系数相加
        {
            coef = pa->coef + pb->coef;
            if(coef > -0.005 && coef < 0.005 && pa->expn != 0)   // 相加系数为0
            {
                // 可以释放当前结点
                r = pa;
                pa = pa->next;  
                free(r);
                r = NULL;

                r = pb;
                pb = pb->next;
                free(r);
                r = NULL;
            }   // end coef == 0
            else    // 系数不为0，将系数赋值给pa.coef (释放pb当前结点，)
            {
                p->next->coef = coef;
                p = p->next;
                num++;
                
                pa = pa->next;

                r = pb;
                pb = pb->next;
                free(r);
                r = NULL;
            }   // end coef != 0
        }   // end pa == pb
        else if (pa->expn < pb->expn)   // pa 的指数更小，将pa当前结点数据接到新链表后
        {
            p = p->next;
            num++;

            pa = pa->next;
        }   // end pa < pb
        else    // pb 的指数更小，将pb当前结点数据接到新链表后
        {
            r = pb;
            pb = pb->next;
            r->next = p->next->next;
            p->next = r;
            p = p->next;
            num++;
        }   // end else pa > pb
    }   // end while
    // p->next = pa ? pa : pb;     // 将未处理完的数据追加到新链表
    r = pa ? pa : pb;
    while(r)
    {
        p->next = r;
        p = p->next;
        r = r->next;
        num++;
    }
    A->expn = num;
    return A;
}


pLink getOpposite(pLink H)
{
    if(!H)  return NULL;
    pLink p = H;
    while(p->next)
    {
        p->next->coef *= -1;
        p = p->next;
    }
    return H;
}

pLink polynamialSub(pLink A, pLink B)
{
    // modify B.coef whit -coef
    B = getOpposite(B);

    pLink H = polynamialAdd(A, B);

    B = getOpposite(B);

    return H;
}


pLink polynamialMul(pLink A, pLink B)
{
    pLink head = NULL;
    pLink pb = B->next, pa = A->next, p = NULL;;
    while (pb)
    {
        pa = A->next;
        while (pa)
        {
            p = newNode();
            if(!p)  return NULL;
            p->expn = pa->expn + pb->expn;
            p->coef = pa->coef * pb->coef;
            head = polynamialAdd2(head, p);
            pa = pa->next;
        }
        pb = pb->next;
    }

    return head;
}


pLink *testArrayA(int n)
{
    int i = 0;
    pLink *datas = (pLink *)calloc(n, sizeof(pLink));
    if(NULL == datas)   
    {
        perror("calloc for datas error\n");
        return NULL;
    }
    int expn[] = {-5, 1, -17, 16, -17, 1, -20, 18, -12, -5, -14, -1, 19, -11, 3, -10, 10, 18, 1, 15};
    float coef[] = {-13.330,16.770, 13.810, -15.510, 9.680, 18.590, 6.100, 7.050, -15.670, -6.040, 2.870, -14.920, -0.500, 16.900, -12.200, -7.150, -3.860, -0.530, -10.610, 14.990};
    for(i=0; i<n; ++i)
    {
        datas[i] = (pLink)calloc(1, sizeof(PNode));
        if(!datas[i])   continue;
        datas[i]->expn = expn[i];        
        datas[i]->coef = coef[i];
    }
    return datas;
}
pLink *testArrayB(int n)
{
    int i = 0;
    pLink *datas = (pLink *)calloc(n, sizeof(pLink));
    if(NULL == datas)   
    {
        perror("calloc for datas error\n");
        return NULL;
    }
    int expn[] = {-14,-15,-15,0,17,20,5,19,-12,-16,6,-5,14,-3,-17,-6,-16,0,1,-17};
    float coef[] = {6.200,3.400,15.430,9.850,17.530,5.430,-18.070,6.120,-10.460,3.590,-17.940,-18.160,11.570,-11.250,14.780,8.650,-3.600,9.820,8.710,18.070};
    for(i=0; i<n; ++i)
    {
        datas[i] = (pLink)calloc(1, sizeof(PNode));
        if(!datas[i])   continue;
        datas[i]->expn = expn[i];        
        datas[i]->coef = coef[i];
    }
    return datas;
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

    pLink *datasA = randArray(numA);    // testArrayA(numA); //
    if(!*datasA)
    {
        printf("rand datasA error\n");
        return -11;
    }

    pLink *datasB = randArray(numB);    // testArrayB(numA); //
    if(!*datasB)
    {
        printf("rand datasB error\n");
        return -12;
    }

    pLink A = createPolynamial(datasA, numA);
    if(!A)  
    {
        perror("createPolynamial A error.\n");
        return -21;
    }
    // traverseList(A, "A");

    pLink B = createPolynamial(datasB, numB);
    if(!B)  
    {
        perror("createPolynamial B error.\n");
        return -22;
    }
    // traverseList(B, "B");

    pLink polyAdd = polynamialAdd(A, B);
    // traverseList(polyAdd, "polyAdd");

    pLink polySub = polynamialSub(A, B);
    // traverseList(polySub, "polySub");

    // polyAdd = polynamialAdd(A, B);       // check B is not modified
    // traverseList(polyAdd, "polyAdd");

    pLink polyMul = polynamialMul(A, B);
    traverseList(polyMul, "polyMul");
    // printf("%s total elements: %d\n", "polyMul", polyMul->expn);

    freeLink(&A);
    freeLink(&B);
    freeLink(&polyAdd);
    freeLink(&polySub);
    freeLink(&polyMul);

    return 0;
}