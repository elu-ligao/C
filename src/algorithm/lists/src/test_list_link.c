/*
 * @Author       : elu
 * @Date         : 2022-01-09 15:28:45
 * @LastEditTime : 2022-01-09 16:02:31
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

struct Node    // 节点域
{
    ElemType Element;
    Position next;
};

int main(int argc, char *argv[])
{
    List L = CreateList();
    if(NULL == L)
        return -1;
    srand(time(NULL));
    int i = 0, X = 0;
    for(i=0; i<20; ++i)
    {
        X = rand()%20;
        printf("%d ", X);
        Insert(X, L, L);
    }
    printf("\n");

    TraverseList(L);

    X = rand()%20;
    printf("X = %d to be deleted.\n", X);
    Delete(X, L);
    
    TraverseList(L);

    DeleteList(L);
    printf("After DeleteList.\n");

    TraverseList(L);

    return 0;
}