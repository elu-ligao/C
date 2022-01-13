/*
 * @Author       : elu
 * @Date         : 2022-01-10 16:54:15
 * @LastEditTime : 2022-01-11 17:13:16
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "../inc/polynomial.h"
#include "tools.h"


/**
 * @descripttion:   创建多项式链表
 * @return {*}
 * @param {PtrElem} PElemArray
 * @param {int} N
 */
List PolynomialCreate(PtrElem PElemArray, int N)
{
    List L = CreateList();
    PtrElem X = (PtrElem)calloc(1, sizeof(ElemType));
    int i = 0;
    for(i=0; i<N; ++i)
    {
        // printf("PElemArray[%d].Coefficient = %.2f, Exponent = %d\n", i, PElemArray[i].Coefficient, PElemArray[i].Exponent);
        CopyElementData(X, &PElemArray[i]);
        // printf("X.Coefficient = %.2f, X.Exponent = %d\n", X->Coefficient, X->Exponent);
        Insert(*X, L);
    }
    return L;
}

/**
 * @descripttion:   多项式 A + B
 * @return {*}
 * @param {List} A
 * @param {List} B
 */
List PolynomialAdd(List A, List B)
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
            P = Insert(PA->Element, P);
            PA = PA->next;
        }
        else if(PA->Element.Exponent < PB->Element.Exponent)
        {
            P = Insert(PB->Element, P);
            PB = PB->next;
        }
        else    // 系数相同
        {
            P = Insert(PA->Element, P);
            P = Insert(PB->Element, P);
            PA = PA->next;
            PB = PB->next;
        }
        // printf("-----------------------------\n");
        // ShowPolynomial(P);
    }

    while(PA)
    {
        P = Insert(PA->Element, P);
        PA = PA->next;
    }
    while(PB)
    {
        P = Insert(PB->Element, P);
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
List PolynomialSub(List A, List B)
{
    Position P = B->next;
    while(P)
    {
        P->Element.Coefficient = -P->Element.Coefficient;
        P = P->next;
    }
    return PolynomialAdd(A, B);
}

/**
 * @descripttion:   多项式 A / B
 * @return {*}
 * @param {List} A
 * @param {List} B
 */
List PolynomialMult(List A, List B)
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
            // printf("PA.Coefficient = %.2f, PB.Coefficient = %.2f, Coefficient = %.2f\n", 
            // PA->Element.Coefficient, PB->Element.Coefficient, E.Coefficient);
            P = Insert(E, P);
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
List PolynomialPow(List A, int N)
{
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
        return PolynomialMult(PolynomialPow(PolynomialMult(A,A), N/2), A);
    else
        return PolynomialPow(PolynomialMult(A,A), N/2);
}


/**
 * @descripttion:   输出多项式
 * @return {*}
 * @param {List} L
 */
void ShowPolynomial(List L)
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
    
    int first = 1;
    while(P)
    {
        if(!first)
            printf("+ ");
        else 
            first = 0;
        printf("%.3f * X^%d ", P->Element.Coefficient, P->Element.Exponent);
        
        P = P->next;
    }
    printf("\n");
}


List GenPolynomialList(int n, int N)
{    
    float *FArr =  getFloatArray(n, N);
    int *IArr = getArray(n, N);
    PtrElem PElemArray = (PtrElem)calloc(n, sizeof(ElemType));
    int i = 0;
    for(; i<n; ++i)
    {
        PElemArray[i].Exponent = IArr[i];
        PElemArray[i].Coefficient = FArr[i];
    }

    // showArray(IArr, n);
    // showFloatArray(FArr, n);

    List L = PolynomialCreate(PElemArray, n);
    return L;
}


void DeletePolynomial(List L)
{
    DeleteList(L);
}

List GenPolynomialListA(int n, int N)
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
    // showFloatArray(FArr, n);

    List L = PolynomialCreate(PElemArray, n);
    return L;
}


List GenPolynomialListB(int n, int N)
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
    // showFloatArray(FArr, n);

    List L = PolynomialCreate(PElemArray, n);
    return L;
}
int main(int argc, char *argv[])
{
    srand(time(NULL));

    int n = 10, N = 5;
    int PN = 10;

    if(argc >= 2)
        n = atoi(argv[1]);
    if(argc >= 3)
        N = atoi(argv[2]);
    if(argc >= 4)
        PN = atoi(argv[3]);

    printf("n = %d, N = %d, PN = %d\n", n, N, PN);
    List A = GenPolynomialList(n, N);
    printf("A: ");
    ShowPolynomial(A);
    List B = GenPolynomialList(n, N);
    // printf("B: ");
    // ShowPolynomial(B);
#if 0
    List C = PolynomialAdd(A, B);
    // printf("C: ");
    // ShowPolynomial(C);

    List D = PolynomialMult(A, B);
    // printf("D: ");
    // ShowPolynomial(D);
    
    // DeletePolynomial(C);
    // DeletePolynomial(D);
# endif
    List E = PolynomialPow(A, PN);
    printf("E: ");
    ShowPolynomial(E);

    DeletePolynomial(A);
    DeletePolynomial(B);
    DeletePolynomial(E);

    return 0;
}