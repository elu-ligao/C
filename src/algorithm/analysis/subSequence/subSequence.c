/*
 * @Author       : elu
 * @Date         : 2022-01-04 19:09:21
 * @LastEditTime : 2022-01-05 13:06:25
 * @LastEditors  : Please set LastEditors
 * @Description  : 子序列问题
 */
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include <limits.h>
#include <time.h>


/**
 * @descripttion:   最大子序列和
 * @return {*}
 * @param {int} *A
 * @param {int} N
 */
int MaxSubsequenceSum(int *A, int N)
{
    int ThisSum, MaxSum, i;
    ThisSum = MaxSum = 0;

    for(i=0; i<N; ++i)
    {
        ThisSum += A[i];
        if(ThisSum > MaxSum)
            MaxSum = ThisSum;
        else if(ThisSum < 0)
            ThisSum = 0;
    }
    return MaxSum;
}

/**
 * @descripttion:   最小子序列和
 * @return {*}
 * @param {int} *A
 * @param {int} N
 */
int MinSubsequenceSum(int *A, int N)
{
    int ThisSum, MinSum, i;
    ThisSum = 0;
    MinSum = INT_MAX;

    for(i=0; i<N; ++i)
    {
        ThisSum += A[i];
        if(ThisSum < MinSum)
            MinSum = ThisSum;
        else if(ThisSum > 0)
            ThisSum = 0;
    }
    return MinSum;
}

/**
 * @descripttion:   最小的正子序列和
 *                  例如：4，-1，5，-2，-1，2，6，-2，从该序列中选出一个子序列，
 *                  使这个子序列的和>0，并且这个和是所有和>0的子序列中最小的。
 *                  -1，5，-2，-1，序列和为1，是最小的
 * @return {*}
 * @param {int} *A
 * @param {int} N
 */
int MinSubPositiveSeqSumN2(int *A, int N)
{
    int i = 0, j = 0;
    int ThisSum = 0, MinSum = INT_MAX;
    for(i=0; i<N; ++i)
    {
        ThisSum = 0;
        for(j=i; j<N; ++j)
        {
            ThisSum += A[j];
            if(ThisSum > 0 && ThisSum < MinSum)
                MinSum = ThisSum;
        }
    }
    return MinSum;
}

typedef struct 
{
    int seqSum;  // 第idx个元素的前缀和
    int idx;        // 在原数组A中的索引
} PrefixData, *pPrefixData;

typedef struct 
{
    int seqSum;
    int start;      
    int end;
    int size;
} MinSumSeq, *pMinSumSeq;

static int cmpPrefixSum(const void *p1, const void *p2)
{
    return ((pPrefixData)p1)->seqSum - ((pPrefixData)p2)->seqSum;
}

static int cmpMinSumSeq(const void *p1, const void * p2)
{
    pMinSumSeq a = (pMinSumSeq)p1;
    pMinSumSeq b = (pMinSumSeq)p2;
    if(a->seqSum < b->seqSum)   return -1;      // 先比较seqSum
    else if(a->seqSum > b->seqSum)  return 1;
    else 
    {                                           // 再比较size包含元素的长度 逆序
        if(a->size < b->size)   return 1;
        else if(a->size > b->size)  return -1;
        else    return a->start - b->start; // 最后比较在原数组中的索引
    }
}

// 前缀和
pMinSumSeq MinSubPositiveSeqSumPrefixSum(int *A, int N)
{
    pPrefixData PrefixSumInfo = (pPrefixData)calloc(N, sizeof(PrefixData));
    pMinSumSeq MinSumInfo = (pMinSumSeq)calloc(N, sizeof(MinSumSeq));
    int i, j, k, pre;
    PrefixSumInfo[0].seqSum = A[0];
    PrefixSumInfo[0].idx = 0;
    for(i=1; i<N; ++i)
    {
        PrefixSumInfo[i].seqSum = PrefixSumInfo[i-1].seqSum + A[i];
        PrefixSumInfo[i].idx = i;
    }

    qsort(PrefixSumInfo, N, sizeof(PrefixData), cmpPrefixSum);

    for(i=0; i<N; ++i)  // 初始化为最大值
        MinSumInfo[i].seqSum = INT_MAX;
        
    for(i=0; i<N; ++i)
    {
        pre = 1;
        j = PrefixSumInfo[i].seqSum - PrefixSumInfo[i-pre].seqSum;
        while(j>0 && i-pre >= 0)    // 当前前序和i和其前面的更小的各前序和比较，找出idx顺序一致的最小的前序和差即为最小子序列
        {
            if(i>0 && j>0 && j < MinSumInfo[i].seqSum)  // 两个前序和比较，此时要判断前序和的idx
            {
                k = PrefixSumInfo[i].idx - PrefixSumInfo[i-pre].idx;
                if(k > 0)   // idx顺序一致才符合要求
                {
                    MinSumInfo[i].seqSum = j;
                    MinSumInfo[i].start = PrefixSumInfo[i-pre].idx+1;
                    MinSumInfo[i].end = PrefixSumInfo[i].idx;
                    MinSumInfo[i].size = k;
                    break;
                }
            }

            ++pre; 
            j = PrefixSumInfo[i].seqSum - PrefixSumInfo[i-pre].seqSum;
        }

        if(PrefixSumInfo[i].seqSum > 0 && PrefixSumInfo[i].seqSum < MinSumInfo[i].seqSum) // 单独自身的前缀和
        {
            MinSumInfo[i].seqSum = PrefixSumInfo[i].seqSum;
            MinSumInfo[i].start = 0;
            MinSumInfo[i].end = PrefixSumInfo[i].idx;
            MinSumInfo[i].size = PrefixSumInfo[i].idx + 1;
        }
    }

    qsort(MinSumInfo, N, sizeof(MinSumSeq), cmpMinSumSeq);

    return MinSumInfo;
}


void showMinSumInfo(pMinSumSeq sumInfo, int n)
{
    int i = 0;
    for(i=0; i<n; ++i)
    {
        if(sumInfo[i].seqSum == INT_MAX)    continue;   // 到该元素的子序列无正序列和，在原始数组中的索引为 sumInfo[i].end 
        printf("i = %d, seqSum = %d, size = %d, start = %d, end = %d\n", 
        i, sumInfo[i].seqSum, sumInfo[i].size, sumInfo[i].start, sumInfo[i].end);
        break;      // 打印首行， 全部打印注释掉break
    }
}


static double Max(double a, double b)
{
    return (a > b) ? a : b;
}
static double Min(double a, double b)
{
    return (a < b) ? a : b;
}

/**
 * @descripttion:   最大的子序列乘积
 * @return {*}
 * @param {int} *A
 * @param {int} N
 */
double MaxSubsequenceMul(double *A, int N)
{
    double *positive = (double *)calloc(N+1, sizeof(double));
    double *negavite = (double *)calloc(N+1, sizeof(double));
    double maxVal = 0;

    int i = 0;
    for(i=0; i<N; ++i)
    {
        if(A[i] > 0.005)
        {
            positive[i+1] = Max(positive[i]*A[i], A[i]);
            maxVal = Max(maxVal, positive[i+1]);
            negavite[i+1] = Min(negavite[i]*A[i], A[i]);
        }
        else if(A[i] < 0.005)
        {
            positive[i+1] = Max(negavite[i]*A[i], 0);
            maxVal = Max(maxVal, positive[i+1]);
            negavite[i+1] = Min(positive[i]*A[i], A[i]);
        }
        else 
        {
            positive[i+1] = 0;
            negavite[i+1] = 0;
        }
        // printf("maxVal = %.3f\n", maxVal);
    }

    return maxVal;
}



int main(int argc, char *argv[])
{
    srand(time(NULL));
    int N = 8;
    int n = 20;
    if(argc == 2)
        N = atoi(argv[1]);
    else if(argc == 3)
    {
        N = atoi(argv[1]);
        n = atoi(argv[2]);
    }
    int *A = getArray(N, n);
    // int A[] = {4, -1, 5, -2, -1, 2, 6, -2};
    // int A[] = {4, 19, 10, -9, 10, -19, -4, 7};

    // showArray(A, N);

    int maxSum = MaxSubsequenceSum(A, N);
    printf("MaxSubsequenceSum = %d\n", maxSum);

    int minSum = MinSubsequenceSum(A, N);
    printf("MinSubsequenceSum = %d\n", minSum);

    float start, cost;
    start = getTime();
    int minSubPostiveSum = MinSubPositiveSeqSumN2(A, N);
    cost = getTime() - start;
    printf("MinSubPositiveSeqSumN2 = %d, cost: %.6f\n", minSubPostiveSum, cost);

    start = getTime();
    pMinSumSeq minSumSeq = MinSubPositiveSeqSumPrefixSum(A, N);
    cost = getTime() - start;
    printf("MinSubPositiveSeqSumPrefixSum = %d, cost: %.6f\n", minSumSeq[0].seqSum, cost);
    showMinSumInfo(minSumSeq, N);

    N = 20, n = 20;
    double *F = getFloatArray(N, n);
    // showFloatArray(F, N);
    double mul = MaxSubsequenceMul(F, N);
    printf("MaxSubsequenceMul = %.3f\n", mul);

    return 0;
}