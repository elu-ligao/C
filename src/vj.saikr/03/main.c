/*
 * @Author       : elu
 * @Date         : 2022-01-20 15:37:40
 * @LastEditTime : 2022-01-20 17:31:36
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include<stdio.h>//uncle-lu

/*
 * line : 所有网线的长度
 * mx : 最长的网线
 */
int line[10010];
int n,k,mx,l,r,mid,ans;

/* count函数:
 * 作用:
 *		统计根据x长度能截出多少网线
 *
 * 变量:
 *		x : 枚举出的答案长度
 */
/*
	 *
	 * 二分答案，根据枚举出的长度去算截出的数量，再根据数量调整长度--Megumin
	 *
	 */
int count(int x)
{
	int sum = 0;
	for(int i=1;i<=n;++i)
		sum += line[i]/x;
	return sum;
}

int main()
{
	double temp;
	scanf("%d %d",&n,&k);
  	r=0;l=1;
	for(int i=1;i<=n;++i)
	{
		scanf("%lf",&temp);
		line[i] = (int)(temp*100); // objective 4将长度由米变成厘米
		if(line[i] > r)r = line[i];
	}

	

	ans=0;
	while(l<=r)
	{
		mid = (l+r)/2;
		if((mx = count(mid)) < k)r = mid-1;//objective 1什么情况是不够的，需要减少目标长度的？提示：见上文count函数介绍
		else
		{
			if(mx >= k)  	//objective 2处理二分
            { 
              	l = mid + 1;
              	ans = mid; 
            }
		}
	}

	printf("%.2lf",ans/100.0);// objective3 将长度由厘米变成米
	return 0;
}