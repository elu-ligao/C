/*
 * @Author       : elu
 * @Date         : 2022-01-19 22:33:14
 * @LastEditTime : 2022-01-19 23:59:41
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include<stdio.h>//uncle-lu
//我们对修改的部分进行差分，每次操作只会用两次操作，降低了一维的时间复杂度。
int line[10010];
int change[10010];//修改的部分的差分
int n, q, p;

int main()
{
	scanf("%d", &n);
	for(int i=1; i<=n; ++i)
		scanf("%d", &line[i]);

	scanf("%d", &q);
	for(int i=1; i<=q; ++i)
	{
		int a, b, val;
		scanf("%d %d %d", &a, &b, &val);
		change[a] += val; //objective 1利用差分修改change数组
        change[b+1] += -val; 
	}

	for(int i=2; i<=n; ++i) //objective 2将差分数组抹平
	{
        change[i] += change[i-1];
    }

	scanf("%d", &p);
	for(int i=1;i<=p;++i)
	{
		int temp;
		scanf("%d", &temp);
		printf("%d\n", change[temp]+line[temp] );//objective 3答案是什么？
	}

	return 0;
}