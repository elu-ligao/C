/*
 * @Author       : elu
 * @Date         : 2022-01-09 22:46:49
 * @LastEditTime : 2022-01-09 23:08:31
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "stack_record.h"



int main(int argc, char *argv[])
{
    ElemType E;
    Stack S = CreateStack(5);
    if(NULL == S)   return -1;

    srand(time(NULL));
    int i = 0, n = 15, X = 0;
    for(; i<n; ++i)
    {
        X = rand()%20;
        printf("%d ", X);
        memset(&E, 0, sizeof(E));
        E.idata = X;
        if(Push(&E, S)) break;
    }
    printf("\n");

    // for(i=0; i<S->Capacity; ++i)
    //     printf("S.Array[%d] = %d\n", i, S->Array[i].idata);

    DisposeStack(S);

    return 0;
}