/*
 * @Author       : elu
 * @Date         : 2022-01-05 12:45:34
 * @LastEditTime : 2022-01-11 18:48:40
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include "tools.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * @descripttion:   随机生成n个0-N之间的数
 * @return {*}
 * @param {int} n
 * @param {int} N
 */
int *getArray(int n, int N)
{
    int *a = (int *)calloc(n+1, sizeof(int));
    int f = 0;
    for(int i=0; i<=n; ++i)
    {
        f = 1; // rand()%2 ? 1 : -1;
        a[i] = f*rand()%(N+1);
    }
    return a;
}

float *getFloatArray(int n, int N)
{
    float *a = (float *)calloc(n+1, sizeof(float));
    int f = 0;
    for(int i=0; i<=n; ++i)
    {
        f = 1; // rand()%2 ? 1 : -1;
        a[i] = f*rand()%(N+1);
        a[i] += rand()%100 / 100.0;
    }
    return a;
}

void showArray(int *A, int n)
{
    printf("Array: ");
    for(int i=0; i<n; ++i)
        printf("%d ", A[i]);
    printf("\n");
}

void showFloatArray(float *A, int n)
{
    printf("Array: ");
    for(int i=0; i<n; ++i)
        printf("%.3f ", A[i]);
    printf("\n");
}