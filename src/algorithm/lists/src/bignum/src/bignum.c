/*
 * @Author       : elu
 * @Date         : 2022-01-11 17:13:54
 * @LastEditTime : 2022-01-12 14:41:07
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 *          i=2;while [[ $i -le 200  ]]; do echo 2000^$i | bc | rev | sed 's/[0-9]\{3\}/&,/g; s/,$//' | rev; let "i*=2"; done
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#include "../inc/bignum.h"
#include "tools.h"


/**
 * @descripttion:   创建多项式链表
 * @return {*}
 * @param {PtrElem} PElemArray
 * @param {int} N
 */
BigNum BigNumCreate(PtrElem PElemArray, int N)
{
    List L = CreateList();
    PtrElem X = (PtrElem)calloc(1, sizeof(ElemType));
    int i = 0;
    for(i=0; i<N; ++i)
    {
        // printf("PElemArray[%d].Coefficient = %.d, Exponent = %d\n", i, PElemArray[i].Coefficient, PElemArray[i].Exponent);
        CopyElementData(X, &PElemArray[i]);
        // printf("X.Coefficient = %d, X.Exponent = %d\n", X->Coefficient, X->Exponent);
        Insert(*X, L, L);
    }
    return L;
}

/**
 * @descripttion:   对多项式A的指数为Exp的结点校验是否要进位
 * @return {*}
 * @param {BigNum} L
 * @param {int} Exp
 */
void BigNumCheckCarry(BigNum P)
{
    // printf("BigNumCheckCarry: P->Element.Coefficient = %d\n", P->Element.Coefficient);
    if(P->Element.Coefficient >= BASE)
    {
        int Exp = P->Element.Exponent;
        int N = (int)P->Element.Coefficient/BASE;
        P->Element.Coefficient -= N * BASE;
        Position Pre = P->pre;
        BigNumCarry(Pre, Exp+1, N);
    }
}

/**
 * @descripttion:   对指数为Exp的大数L进位
 * @return {*}
 * @param {BigNum} L
 * @param {int} Exp
 */
void BigNumCarry(BigNum L, int Exp, int Carry)
{
    // printf("BigNumCarry L.Exp = %d: %p, Exp = %d\n", L->Element.Exponent, L, Exp);
    Position Pre = L;
    if(Pre->Element.Exponent == Exp)    // 如果指数为Exp的结点的前一个结点系数正好比其大1
    {
        Pre->Element.Coefficient += Carry;
        if(Pre->Element.Coefficient >= BASE)
        {
            Carry = (int)Pre->Element.Coefficient/BASE;
            Pre->Element.Coefficient = (int)Pre->Element.Coefficient%BASE;
            BigNumCarry(Pre->pre, Exp+1, Carry);
        }
    }
    else    // 否则新增指数为Exp+1的结点
    {
        Position N = (Position)calloc(1, sizeof(struct Node));
        N->Element.Coefficient = Carry;
        N->Element.Exponent = Exp;
        
        N->next = Pre->next;
        if(Pre->next)
            Pre->next->pre = N;
        Pre->next = N;
        N->pre = Pre;

        // printf("Pre.Exp = %d, Pre.next.Exp = %d\n", Pre->Element.Exponent, Pre->next->Element.Exponent);
    } 
}

/**
 * @descripttion:   多项式 A + B
 * @return {*}
 * @param {List} A
 * @param {List} B
 */
BigNum BigNumAdd(List A, List B)
{
    if(!A || !B)    return A ? A : B;
    Position PA = A->next, PB = B->next;
    if(NULL == PA)  return B;
    if(NULL == PB)  return A;
    List ADD = CreateList();
    Position P = ADD;
    while(P && PA && PB)
    {
        if(PA->Element.Exponent > PB->Element.Exponent)
        {
            P = Insert(PA->Element, P, ADD);
            PA = PA->next;
        }
        else if(PA->Element.Exponent < PB->Element.Exponent)
        {
            P = Insert(PB->Element, P, ADD);
            PB = PB->next;
        }
        else    // 系数相同
        {
            PtrElem X = (PtrElem)calloc(1, sizeof(ElemType));
            X->Exponent = PA->Element.Exponent;
            X->Coefficient = PA->Element.Coefficient + PB->Element.Coefficient;
            P = Insert(*X, P, ADD);
            PA = PA->next;
            PB = PB->next;
        }
        // printf("-----------------------------\n");
        // ShowBigNum(P);
    }

    while(PA)
    {
        P = Insert(PA->Element, P, ADD);
        PA = PA->next;
    }
    while(PB)
    {
        P = Insert(PB->Element, P, ADD);
        PB = PB->next;
    }
    
    return ADD;
}

/**
 * @descripttion:   多项式 A - B
 * @return {*}
 * @param {List} A
 * @param {List} B
 */
BigNum BigNumSub(List A, List B)
{
    Position P = B->next;
    while(P)
    {
        P->Element.Coefficient = -P->Element.Coefficient;
        P = P->next;
    }
    return BigNumAdd(A, B);
}

/**
 * @descripttion:   多项式 A / B
 * @return {*}
 * @param {List} A
 * @param {List} B
 */
BigNum BigNumMult(List A, List B)
{
    List MUTL = CreateList();
    if(NULL == MUTL)    return NULL;

    Position ZERO = CreateList(); 
    ZERO->Element.Exponent = 0;

    Position PA = A->next;
    if(NULL == PA)
    {
        MUTL->next = ZERO;
        return MUTL;
    } 

    Position PB = B->next;
    if(NULL == PB)
    {
        MUTL->next = ZERO;
        return MUTL;
    }

    Position P = NULL;
    ElemType E;
    while(PA)
    {
        P = MUTL;
        while(PB)
        {
            E.Exponent = PA->Element.Exponent + PB->Element.Exponent;
            E.Coefficient = PA->Element.Coefficient * PB->Element.Coefficient;
            // printf("PA.Exponent = %d, PB.Exponent = %d, Exponent = %d\n", 
            // PA->Element.Exponent, PB->Element.Exponent, E.Exponent);
            // printf("PA.Coefficient = %d, PB.Coefficient = %d, Coefficient = %d\n", 
            // PA->Element.Coefficient, PB->Element.Coefficient, E.Coefficient);
            P = Insert(E, P, MUTL);
            PB = PB->next;
        }
        PA = PA->next;
        PB = B->next;
    }

    return MUTL;
}


/**
 * @descripttion:   多项式的N次幂
 * @return {*}
 * @param {List} A
 * @param {int} N
 */
BigNum BigNumPow(List A, int N)
{
    // printf("BigNumPow N = %d, ((N)&0x01) = %d\n", N, ((N)&0x01));
    List POWN = CreateList();
    if(NULL == POWN)    return NULL;

    if(0 == N)
    {
        Position ZERO = CreateList(); 
        ZERO->Element.Exponent = 0;
        POWN->next = ZERO;
        return POWN;
    }   
    else if(1 == N)
        return A;
    
    if(((N)&0x01) == 1) // N 是奇数
        return BigNumMult(BigNumPow(BigNumMult(A,A), N/2), A);
    else
        return BigNumPow(BigNumMult(A,A), N/2);
}

int GetBaseLen()
{
    int zeros = -1;
    int base = BASE;
    while(base)
    {
        zeros++;
        base /= 10;
    }
    return zeros;
}

/**
 * @descripttion:   输出多项式
 * @return {*}
 * @param {List} L
 */
void ShowBigNum(List L)
{
    Position P = L;
    if(NULL == P)
    {
        printf("0\n");
        return;
    }
    if(P->Element.Exponent == INT_MAX)
    {
        P = P->next;
        if(NULL == P)
        {
            printf("0\n");
            return;
        }
    }  
    
    int zeros = GetBaseLen();
    int first = 1;
    int exp = P->Element.Exponent;
    while(P)
    {
        // printf("\nexp: %d, coef: %d\n", P->Element.Exponent, P->Element.Coefficient);
        if(0 == first)
        {
            if(P->Element.Exponent < exp)
            {
                printf("%0*d", zeros, 0);
            }
            else
            {
                printf("%0*d", zeros, P->Element.Coefficient);
                P = P->next;
            }
        }
        else
        {
            first = 0;
            if(P->Element.Coefficient != 0)
                printf("%d", P->Element.Coefficient);
            P = P->next;
        }       
        exp--;
    }
    while(exp-- >= 0)
        printf("%0*d", zeros, 0);
    printf("\n");
}


BigNum GenBigNumList(int n, int N)
{    
    int *FArr =  getArray(n, N);
    int *IArr = getArray(n, N);
    PtrElem PElemArray = (PtrElem)calloc(n, sizeof(ElemType));
    int i = 0;
    for(; i<n; ++i)
    {
        PElemArray[i].Exponent = IArr[i];
        PElemArray[i].Coefficient = FArr[i];
    }

    // printf("Exp : ");
    // showArray(IArr, n);
    // printf("Coef: ");
    // showArray(FArr, n);

    BigNum L = BigNumCreate(PElemArray, n);

    TraverseList(L);

    return L;
}

/**
 * @descripttion:   将N转换为BigNum 类型
 * @return {*}
 * @param {int} N
 */
BigNum BigNumFormat(int N)
{
    PtrElem PElemArray = (PtrElem)calloc(1, sizeof(ElemType));
    PElemArray[0].Coefficient = N;
    BigNum L = BigNumCreate(PElemArray, 1);
    return L;
}

/**
 * @descripttion:   将数字串s转换为BigNum 类型
 * @return {*}
 * @param {char} *s
 */
BigNum BigNumFormatString(char *s)
{
    int sLen = strlen(s);
    int bLen = GetBaseLen();
    int n = (sLen+bLen-1)/bLen;
    PtrElem PElemArray = (PtrElem)calloc(n, sizeof(ElemType));
    int i = 0, idx = 0;
    char *ptr;
    for(i=0; i<n; ++i)
    {
        idx = sLen-bLen;
        if(idx < 0) idx = 0;
        ptr = s+idx;
        PElemArray[i].Exponent = i;
        PElemArray[i].Coefficient = atoi(ptr);
        s[idx] = '\0';
        sLen -= bLen;
    }
    BigNum L = BigNumCreate(PElemArray, n);
    return L;
}

void DeleteBigNum(List L)
{
    DeleteList(L);
}

BigNum GenBigNumListA(int n, int N)
{    
    int IArr[] = {-5, 1, -17, 16, -17, 1, -20, 18, -12, -5, -14, -1, 19, -11, 3, -10, 10, 18, 1, 15};
    float FArr[] = {-13.330,16.770, 13.810, -15.510, 9.680, 18.590, 6.100, 7.050, -15.670, -6.040, 2.870, -14.920, -0.500, 16.900, -12.200, -7.150, -3.860, -0.530, -10.610, 14.990};
  
    PtrElem PElemArray = (PtrElem)calloc(n, sizeof(ElemType));
    int i = 0;
    for(; i<n; ++i)
    {
        PElemArray[i].Exponent = IArr[i];
        PElemArray[i].Coefficient = FArr[i];
    }

    // showArray(IArr, n);
    // showArray(FArr, n);

    List L = BigNumCreate(PElemArray, n);
    return L;
}


BigNum GenBigNumListB(int n, int N)
{   
    int IArr[] = {-14,-15,-15,0,17,20,5,19,-12,-16,6,-5,14,-3,-17,-6,-16,0,1,-17};
    float FArr[] = {6.200,3.400,15.430,9.850,17.530,5.430,-18.070,6.120,-10.460,3.590,-17.940,-18.160,11.570,-11.250,14.780,8.650,-3.600,9.820,8.710,18.070};
    
    PtrElem PElemArray = (PtrElem)calloc(n, sizeof(ElemType));
    int i = 0;
    for(; i<n; ++i)
    {
        PElemArray[i].Exponent = IArr[i];
        PElemArray[i].Coefficient = FArr[i];
    }

    // showArray(IArr, n);
    // showArray(FArr, n);

    List L = BigNumCreate(PElemArray, n);
    return L;
}



int main(int argc, char *argv[])
{
    srand(time(NULL));

    int n = 10, N = 5;
    int PN = 10;
    char s[20] = {0};
    if(argc >= 2)
        n = atoi(argv[1]);
    if(argc >= 3)
        N = atoi(argv[2]);
    if(argc >= 4)
        // PN = atoi(argv[3]);
        strcpy(s, argv[3]);


    printf("n = %d, N = %d, PN = %d\n", n, N, PN);
    
    List A = BigNumFormatString(s);
    // List A = BigNumFormat(PN);
    // List A = GenBigNumList(n, N);
    printf("A: ");
    ShowBigNum(A);
    TraverseList(A);
#if 0
    List B = GenBigNumList(n, N);
    printf("B: ");
    ShowBigNum(B);
    List C = BigNumAdd(A, B);
    printf("C: ");
    ShowBigNum(C);

    List D = BigNumMult(A, B);
    // printf("D: ");
    // ShowBigNum(D);
    
    // DeleteBigNum(C);
    // DeleteBigNum(D);
    
# endif
    List E = BigNumPow(A, 1234);
    printf("E: ");
    ShowBigNum(E);
    TraverseList(E);
    DeleteBigNum(E);


    

    DeleteBigNum(A);
    // DeleteBigNum(B);

    return 0;
}