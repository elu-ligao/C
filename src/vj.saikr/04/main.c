/*
 * @Author       : elu
 * @Date         : 2022-01-21 18:43:47
 * @LastEditTime : 2022-01-22 10:14:53
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
/*
 * line : 原数列
 * l, r, d, : 题目要求的 对于每个区间的左端点 右端点 值
 * change : 差分数组 
 * 题解：对采用的订单数进行二分，每次用差分数组优化处理当天需要的教室数并与当天可对外借出的教室数比较检验。-Megumin
 */

int line[1000010], l[1000010], r[1000010], d[1000010], change[1000010];
int n, m;
 
int check(int x)
{
	memset(change,0,sizeof(change));

	for (int i = 1; i <= x; i++) 
	{
		change[l[i]] += d[i];
        change[r[i]+1] -= d[i];
		//obj 1用差分数组对前x个操作进行处理 
	}

	for (int i = 1; i <= n; i++) 
		change[i] += change[i-1]; 
		//obj 2抹平差分数组 

	for (int i = 1; i <= n; i++) 
		if(change[i]>line[i])return false;
		//obj 3什么情况下是发生了问题？ 

	return true;
}

int main()
{
	scanf("%d %d", &n, &m);

	for (int i = 1; i <= n; i++) 
		scanf("%d", &line[i]);

	for (int i = 1; i <= m; i++) 
		scanf("%d %d %d", &d[i], &l[i], &r[i]);

	if(check(n))//obj 4什么情况是全都可以成立 
	{
		printf("0");
		return 0;
	}



	int left = 1, right = m, mid;
	while(left<=right)
	{
		mid = (left + right) >> 1;
		if(check(mid))//obj 5
			left = mid + 1; 
		else
			right = mid - 1; 
	}
	printf("-1\n");
	printf("%d", left);

	return 0;
}
