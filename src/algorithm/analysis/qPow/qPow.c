/*
 * @Author       : elu
 * @Date         : 2022-01-04 17:48:47
 * @LastEditTime : 2022-01-05 18:59:02
 * @LastEditors  : Please set LastEditors
 * @Description  : 快速幂 X^N
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

/**
 * @descripttion:   迭代快速幂
 * @return {*}
 * @param {long} X
 * @param {unsigned int} N
 */
long QPow(long X, unsigned int N)
{
    if(0 == N)  return 1;

    if((N&0x01) == 0)   // N 是偶数
        return QPow(X*X, N>>1);
    else 
        return QPow(X*X, N>>1) * X;
}

// 非迭代快速幂
long QPow2(long X, unsigned int N)
{
    if(0 == N)  return 1;

    int s[20] = {0};
    int k = 0;
    while(N)
    {
        s[k++] = N;
        N >>= 1;
    }
    
    long P = X;
    int i = k-1;
    for(; i>=1; --i)
    {
        if(s[i-1] == 2*s[i])
            P = P * P;
        else if(s[i-1] > 2*s[i])    // s[i-1] == 2*s[i] + 1
            P = P * P * X;
    }

    return P;
}

int countTimes(unsigned int N)
{
    int cnt = 0;
    while(N>1)
    {
        cnt += (N & 0x01) + 1;
        N >>= 1;
    }
    return cnt;
}

/**
 * @descripttion:   多项式加法
 *                  sigma(A[i]*X^i)
 * @return {*}
 * @param {int} *A
 * @param {int} n
 */
double PolynomicAddition(int *A, int n, int X)
{
    double *M = (double *)calloc(n, sizeof(double));

    int i = 0;
    M[0] = 1;
    for(i=1; i<=n; ++i)
        M[i] = M[i-1] * X;

    double Res = 0;
    for(i=0; i<=n; ++i)
        Res += A[i] * M[i];

    return Res;
}

double PolynomicAdditionWithQPow(int *A, int n, int X)
{
    int i = 0;
    double Res = 0;
    for(i=0; i<=n; ++i)
        Res += A[i] * QPow(X, i);

    return Res;
}

/**
 * @descripttion:   Horner 法则
 * @return {*}
 * @param {int} *A
 * @param {int} n
 * @param {int} X
 */ 
double Horner(int *A, int n, int X)
{
    double Res = 0;
    int i = 0;
    for(i=n; i>=0; --i)
    {
        // printf("X = %d, Res = %f, A[%d] = %d, ", X, Res, i, A[i]);
        Res = X * Res + A[i];
        // printf("Res = %f\n", Res);
    }

    return Res;
}

float getTime()
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday (&tv , &tz);

    float secs = tv.tv_sec % 1000 + tv.tv_usec / 1000000.0; 
    // printf("secs = %.6f\n", secs);
    return secs;
}

int *getArray(int n, int N)
{
    int *a = (int *)calloc(n+1, sizeof(int));
    for(int i=0; i<=n; ++i)
        a[i] = rand()%(N+1);
    return a;
}

void showArray(int *A, int n)
{
    printf("Array: ");
    for(int i=0; i<n; ++i)
        printf("%d ", A[i]);
    printf("\n");
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int n = 4, N = 10, X = 3;
    char ch = 0;
    while ((ch = getopt(argc, argv, "n:N:X:")) != -1)
    {
    // printf("optind: %d\n", optind);
        switch (ch) 
        {
        case 'n':
            n  = atoi(optarg);
            break;
        case 'N':
            N  = atoi(optarg);
            break;
        case 'X':
            X  = atoi(optarg);
            break;
        }
    }    

    printf("n = %d, N = %d, X = %d\n", n, N, X);
    int *A = getArray(n, N);
    // int A[5] = {2, 1, 0, 8, 4};
    // showArray(A, n+1);

    float start, cost;

    start = getTime();
    double p1 = PolynomicAddition(A, n, X);
    cost = getTime() - start;
    printf("PolynomicAddition cost: %.6f\n", cost);

    start = getTime();
    double p2 = PolynomicAdditionWithQPow(A, n, X);
    cost = getTime() - start;
    printf("PolynomicAdditionWithQPow cost: %.6f\n", cost);
    
    start = getTime();
    double p3 = Horner(A, n, X);
    cost = getTime() - start;
    printf("Horner cost: %.6f\n", cost);

    printf("p1 = %.6lf\np2 = %.6lf\np3 = %.6lf\n", p1, p2, p3);

    return 0;
}