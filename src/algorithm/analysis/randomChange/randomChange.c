/*
 * @Author       : elu
 * @Date         : 2022-01-04 12:40:36
 * @LastEditTime : 2022-01-04 17:15:11
 * @LastEditors  : Please set LastEditors
 * @Description  : 生成前N个自然数的一个随机置换
 *                  1. 生成随机数A[i]直到它不同于已生成的A[0..i-1]
 *                  2. 用一个used[N]数组来判断第i次的随机数A[i]是否已经生成过
 *                  3. 先生成1..N的序列， 再循环N-1次随机交换两个位置的数据
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>


int * fun01(int N, int *A);
int * fun02(int N, int *A);
int * fun03(int N, int *A);
void quickSort(int *L, int N);
int checkArray(int *L, int N);

float getTime()
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday (&tv , &tz);

    float secs = tv.tv_sec % 1000 + tv.tv_usec / 1000000.0; 
    // printf("secs = %.6f\n", secs);
    return secs;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int N = 250;
    if(argc == 2)
    {
        N = atoi(argv[1]);
    }

    float start = getTime();
    int *a01 = (int *)calloc(N+1, sizeof(int));
    if(N > 10000)
        fun01(10000, a01+1);
    else
        fun01(N, a01+1);
    
    float cost = getTime() - start;
    printf("fun01 cost: %.6f\n", cost);

    start = getTime();
    int *a02 = (int *)calloc(N+1, sizeof(int));
    fun02(N, a02+1);
    cost = getTime() - start;
    printf("fun02 cost: %.6f\n", cost);

    start = getTime();
    int *a03 = (int *)calloc(N+1, sizeof(int));
    fun03(N, a03+1);
    cost = getTime() - start;
    printf("fun03 cost: %.6f\n", cost);

    quickSort(a01, N>10000?10000:N);
    // quickSort(a02, N);
    // quickSort(a03, N);

    // printf("check a01\n");
    checkArray(a01, N>10000?10000:N);
    // printf("check a02\n");
    // checkArray(a02, N);
    // printf("check a03\n");
    // checkArray(a03, N);

    // for(int i=1; i<=N; ++i)
    // {
    //     printf("%d ", a03[i]);
    //     if(i%30 == 0)   printf("\n");
    // }
    // printf("\n");

    return 0;
}

int * fun01(int N, int *A)
{
    int i = 0, j = 0, k = 0;
    int a = 0;
    for(i=0; i<N; ++i)
    {
        k = 1;
        while(k)
        {
            k = 0;
            a = rand()%N + 1;
            for(j=0; j<i; ++j)
            {
                if(A[j] == a)
                {
                    k = 1;
                    break;
                }
            }
        }
        A[i] = a;      
    }
    return A;
}

int * fun02(int N, int *A)
{
    char *s = (char *)calloc(N+1, sizeof(char));

    int i = 0, k = 0;
    for(i=0; i<N; ++i)
    {
        #if 0

        while(1)
        {
            k = rand()%N + 1;
            if(s[k] != '1')
            {
                s[k] = '1';
                A[i] = k;
                break;
            }
        }

        #else

        for(k=rand()%N+1; s[k]=='1'; k=rand()%N+1)  ;
        s[k] = '1';
        A[i] = k;

        #endif
    }
    return A;
}

void Swap(int *a, int *b)
{
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

int RandInt(int i, int j)
{
    if(i > j)
        Swap(&i, &j);
    j -= i;
    return rand()%j+i;
}

int * fun03(int N, int *A)
{
    int i = 0;
    for(i=0; i<N; ++i)
        A[i] = i+1;

    for(i=1; i<N; ++i)
        Swap(&A[i], &A[RandInt(0, i)]);

    return A;
}


int partition(int *L, int low, int high)
{
    L[0] = L[low];  // 用子表的第一个记录作为枢纽值
    while(low < high)
    {
        while(low < high && L[high] >= L[0])  --high;
        L[low] = L[high];   // 将比枢轴小的记录移动到前面
        while(low < high && L[low] <= L[0]) ++low;
        L[high] = L[low];   // 将比枢轴大的记录移动到后面
    }
    L[low] = L[0];  // 枢轴位置赋值
    return low;
}

void qSort(int *L, int low, int high)
{
    int pivotloc = 0;   // 枢轴位置
    if(low < high)
    {
        // 将L[low, hign] 一分为二，pivotloc是枢轴位置，前面元素的均小于L[pivotloc]，后面元素为大于L[pivotloc]的元素
        pivotloc = partition(L, low, high); 
        qSort(L, low, pivotloc);    // 对左子表递归排序
        qSort(L, pivotloc+1, high); // 对右子表递归排序
    }
}

void quickSort(int *L, int N)
{
    qSort(L, 1, N);
}

int checkArray(int *L, int N)
{
    int i = 0;
    for(i=2; i<=N; ++i)
    {
        if(L[i] < L[i-1])
        {
            printf("r[%d] = %d, r[%d] = %d\n", i-1, L[i-1], i, L[i]);
            return -1;
        }
    }
    return 0;
}