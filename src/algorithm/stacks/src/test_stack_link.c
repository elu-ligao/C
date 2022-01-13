/*
 * @Author       : elu
 * @Date         : 2022-01-09 20:25:06
 * @LastEditTime : 2022-01-09 22:41:11
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "stack_link.h"



int main(int argc, char *argv[])
{
    ElemType E;
    Stack S = CreateStack();
    if(NULL == S)   return -1;

    srand(time(NULL));
    int i = 0, n = 10, X = 0;
    for(; i<n; ++i)
    {
        X = rand()%20;
        printf("%d ", X);
        memset(&E, 0, sizeof(E));
        E.idata = X;
        Push(&E, S);
    }
    printf("\n");

    MakeEmpty(S);

    return 0;
}