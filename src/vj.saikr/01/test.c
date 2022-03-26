/*
 * @Author       : elu
 * @Date         : 2022-01-07 10:58:43
 * @LastEditTime : 2022-01-18 23:29:18
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
// #include "trace.h"


#include<stdio.h>//uncle-lu

int n, top;
int line[100];
//我们利用n，保存当前n所剩没有加过的值，然后对当前所剩数进行搜索
void func(int last)
{
	if(n == 0)
	{
		if(top == 1)    return; //objective 3如何利用top判断前面一项把n全用完了的状态？而且，如果第一项n就用完了，根据题意我们应该怎么操作？
		printf("%d",line[1]);//思考：为什么不直接把line[1]放进for循环里呢？
		for(int i=2; i<=top; ++i)
			printf("+%d", line[i]);
		printf("\n");
		return ;
	}

	for(int i=last; i<=n; ++i)
	{
      	top++;
		line[top] = i;
		n -= i;	//objective 1在进行下一次搜索前要做什么准备？提示：看顶端题解
		func(i);
        n += i;	//objective 2之后呢？
		top--;
	}

	return ;
}

int main()
{
	scanf("%d",&n);
  	top=0;//提示：记得初始化变量！有时候就是因为不初始化变量本地ac交题wa。
	func(1);
	return 0;
}