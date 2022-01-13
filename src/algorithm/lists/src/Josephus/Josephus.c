/*
 * @Author       : elu
 * @Date         : 2022-01-12 16:14:43
 * @LastEditTime : 2022-01-13 00:15:36
 * @LastEditors  : Please set LastEditors
 * @Description  : 约瑟夫环问题
 *                  N个人从1到N进行编号，围坐成一个圈，从1号开始报数，报到M的人退出圈子，下一个从新从1开始报数
 *                  第M个退出，最后剩下的人取胜，求最后获胜者的编号(最后获胜的只剩一个人，报数必是1)
 *  N = 9, M = 4
 *      1 2 3 4 5 6 7 8 9 
 *      1 2 3 x     第一轮，4出局， 1号报1，f(9)=(f(8)+4)%9=1，排第一个位置
 *              1 2 3 x     第二轮，8出局，5号报1, 共有8人，剩余7人，f(8)=(f(7)+4)%8=6
 *                      9 1 2 3 5 6 7   七个人，f(7)=(f(6)+4)%7=2，排第二个位置
 *                      1 2 3 x     第三轮，3号出局，9号报1，共7人，剩6人
 *                              5 6 7 9 1 2     六个人，f(6)=(f(5)+4)%6=5，排第五个位置
 *                              1 2 3 x     第四轮，9号出局，5号报1，共6人，剩5人。 
 * 
 *      1 2 5 6 7   五个人，f(5) = (f(4) + 4) % 5 = 1，排第一个位置
 *      1 2 3 x
 *              7 1 2 5     四个人，f(4) = (f(3) + 4) % 4 = 2，排第二个位置
 *              1 2 3 x
 *                      7 1 2 7     三个人，f(3) = (f(2) + 4) % 3 = 2，排第二个位置
 *                      1 2 3 x
 *                        1 2 1 2   两个人，f(2) = (f(1) + 4) % 2 = 1，排第一个位置
 *                        1 2 3 x
 *                        1     一个人f(1) = 1, 排第一个位置
 *                        1
 */                     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @descripttion:   
 * @return {*}      k个人中重新编号的解
 * @param {int} m   报m的人退出
 * @param {int} k   k 个人报数
 * @param {int} last    已知k-1个人的解是第last的编号
 */
int josephus(int m, int k, int last)
{
    return (last + m) % k;
}

void solutionJosephus(int n, int m)
{
    int f1 = 0, fn = 0;
    int i = 0;
    for(i=2; i<=n; ++i)
    {
        fn = josephus(m, i, f1);
        f1 = fn;
    }
    printf("%d\n", fn+1);  // 返回的是下标，实际编号从1开始，故+1
}

typedef struct Node
{
    int no;
    int pre, next;
} Node, *PtrNode;

void solution(int n, int m)
{
    PtrNode N = (PtrNode)calloc(n, sizeof(Node));
    if(NULL == N)
    {
        perror("calloc for PtrNode error.\n");
        return;
    }

    int i = 0;
    for(i=0; i<n; ++i)
    {
        N[i].no = i+1;
        N[i].pre = i-1;
        N[i].next = i+1;
    }
    N[0].pre = n-1;
    N[n-1].next = 0;

    int k = 0;
    int P = 0;
    while(k<n-1)
    {
        for(i=1; i<m; ++i)
            P = N[P].next;
        // printf("%d ", N[P].no);     // out
        N[N[P].pre].next = N[P].next;   // delete P
        N[N[P].next].pre = N[P].pre;
        
        P = N[P].next;  // next restart
        ++k;
    }
    printf("\n");
    printf("%d\n", N[P].no);
}

int main(int argc, char *argv[])
{
    int n = 10, m = 7;
    if(argc >= 2)   n = atoi(argv[1]);
    if(argc >= 3)   m = atoi(argv[2]);

    printf("n = %d, m = %d\n", n, m);

    solutionJosephus(n, m);

    solution(n, m);

}