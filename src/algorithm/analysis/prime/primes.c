/*
 * @Author       : elu
 * @Date         : 2022-01-05 15:20:24
 * @LastEditTime : 2022-01-05 19:28:05
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int getSumOfFactor(int n)
{
	int s = 1;
	int i = 0;
	for(i=2; i*i<=n; ++i){
		if(n%i == 0){
			s += i;
			s += n/i;
		}
	}
	return s;
}


int perfectNum(int n)
{
	if(n <= 1){
		return -1;
	}
	int i;
	for(i=2; i<=n; ++i){
		if(i == getSumOfFactor(i)){
			printf("%d\n", i);
		}
	}

	return 0;
}


/**
 * @descripttion:   Erastothenes 筛
 *                  制作2-N的表，找出最小的未被删除的整数i，打印i，然后删除i的倍数，i,2i,3i...
 *                  当i>sqrt(s)结束
 * @return {*}
 * @param {int} LNum
 * @param {int} *nums
 */
char *primeNums(int LNum, int *nums)
{
    if(LNum <= 1)
        return NULL;

    char *s = (char *)calloc(LNum+1, sizeof(char));
    if(NULL == s)
    {
        perror("calloc error");
        return NULL;
    }

    memset(s, '1', LNum);
    s[0] = '0'; // 1 既不是素数也不是合数

    int i, j, k;
    k = 0;
    for(i=2; k<=LNum; ++i)   // 先2的倍数(除2外)肯定是非素数
    {
        k = 2 * i;
        s[k-1] = '0';
    }
    for(i=3; i<=LNum; i+=2) // 再从3开始，判断非素数的奇数倍(偶数倍上面2已经处理过)
    {
        // i 不是素数，则它的倍数也不是素数
        if(s[i-1] == '0') continue; // effective important

        for(j=3,k=i*j; k<=LNum; j+=2,k=i*j) // 删除i的奇数倍(3,5,7...)
        {
            // printf("i=%d, j=%d, k = %d\n", i, j, k);
            s[k-1] = '0';
        }
    }

#if 0
	// int 型数组占用空间比char *少， primes的数量少于LNum/4
	for(i=1,k=0; i<LNum; ++i)
		if(s[i] == '1')	++k;

	int *primes = (int *)calloc(k, sizeof(int));
	for(i=1,k=0; i<LNum; ++i)
		if(s[i] == '1')
			primes[k++] = i+1;
#endif

	return s;
}

int perfectNums(int n)
{
	char *s = primeNums(n, NULL);
	int i;
	for(i=1; i<n; ++i){
		if((i%5 == 0 || i % 27 == 0) && s[i] == '0' && i+1 == getSumOfFactor(i+1)){
			printf("%d\n", i+1);
		}
	}
	return 0;
}



// Mersenne prime 梅森素数 
/*
如果p是质数，且2^p-1也是质数，那么（2^p-1）X2^（p-1）便是一个完全数。
例如p=2，是一个质数，2^p-1=3也是质数，（2^p-1）X2^（p-1）=3X2=6，是完全数。
例如p=3，是一个质数，2^p-1=7也是质数，（2^p-1）X2^（p-1）=7X4=28，是完全数。
例如p=5，是一个质数，2^p-1=31也是质数，（2^p-1）X2^（p-1）=31X16=496是完全数。
但是2^p-1什么条件下才是质数呢?事实上，当2^p-1是质数的时候，称其为梅森素数
*/
int getMersennePrime(int n)
{
	char *s = primeNums(n, NULL);
	long long i, m, p;
	for(i=1; ; ++i){
		p = i+1;
		m = (1<<p)-1;
		if(m > n)	break;
		if(s[m-1] == '1' && s[i] == '1')
        {
			printf("p=%lld, 2^p-1=%lld, 2^(p-1)=%lld, perfectNum=%lld\n", 
            p, m, (long long)1<<i, m*(1<<i));
		}
	}

	return 0;
}


int main(int argc, char *argv[])
{

    int N = 10000;
    if(argc == 2)   N = atoi(argv[1]);

    char *S = primeNums(N, NULL);
    
    int i = 0, cnt = 0;
    for(i=0; i<N; ++i)
        if(S[i] == '1')
        {
            // printf("%-4d ", i+1);
            // if((i+1) % 20 == 0) printf("\n");
             ++cnt;
        }
    printf("\n");

    printf("N = %d, cnt = %d\n", N, cnt); 
	
    
	// float start, cost;
	
	// perfectNums(atoi(argv[1]));

	
	// getMersennePrime(atoi(argv[1]));

	return 0;
}
