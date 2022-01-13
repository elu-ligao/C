/*
 * @Author       : elu
 * @Date         : 2022-01-04 17:19:26
 * @LastEditTime : 2022-01-04 17:47:00
 * @LastEditors  : Please set LastEditors
 * @Description  : 最大公因数
 *                  定理： 如果 M > N, 则 M mod N < M/2.
 * 
 */
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>


/**
 * @descripttion:   最大公因数
 * @return {*}
 * @param {unsigned int} M
 * @param {unsigned int} N
 */
unsigned int Gcd(unsigned int M, unsigned int N)
{
    unsigned int Rem = 0;

    while(N>0)
    {
        Rem = M % N;
        M = N;
        N = Rem;
    }
    return M;
}


/**
 * @descripttion:   估计两个随机数互素的概率
 *                  计算两个随机选取并小于或等于N的互异正整数互素的概率(当N增大时趋于6/pi^2)
 * @return {*}
 * @param {int} N
 */
double PercentageOfPrimePairs(int N)
{
    int Rel = 0, Tot = 0;
    int i, j;
    for(i=1; i<=N; ++i)
    {
        for(j=i+1; j<=N; ++j)
        {
            ++Tot;
            // int g = Gcd(i, j);
            // printf("%d\n", g);
            // if(1 == g) 
            if(1 == Gcd(i, j))
                ++Rel;
        }
    }
    return (double)Rel/Tot;
}


int main(int argc, char *argv[])
{
    int N = 10000;
    if(argc == 2)
        N = atoi(argv[1]);

    double p = PercentageOfPrimePairs(N);
    printf("Pertentage of relatively prime pairs is: %.6f\n", p);
    printf("6/pi^2 = %.6f\n", 6/(M_PI*M_PI));
    return 0;
}
