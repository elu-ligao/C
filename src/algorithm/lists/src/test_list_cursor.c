/*
 * @Author       : elu
 * @Date         : 2022-01-09 16:19:28
 * @LastEditTime : 2022-01-09 18:04:10
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cursor.h"



int main(int argc, char *argv[])
{
    InitializeCursorSpace();
    List L = CreateList();
    if(0 == L)  return -1;

    srand(time(NULL));
    int i = 0, X = 0;
    int n = 20;
    Position P = L;
    for(i=0; i<n; ++i)
    {
        X = rand()%20;
        printf("%d ", X);
        P = Insert(X, P, L);
    }
    printf("\n");
    TraverseList(L);

    X = rand()%20;
    printf("X = %d to be deleted.\n", X);
    Delete(X, L);
    
    TraverseList(L);

    X = GetLastIdx(L);
    printf("Last idx is %d\n", X);
    X = GetLastElement(L);
    printf("Last Element is %d\n", X);
    Delete(X, L);
    X = GetLastIdx(L);
    printf("After Delete Last, New Last idx is %d\n", X);
    X = GetLastElement(L);
    printf("After Delete Last, New Last element is %d\n", X);
    TraverseList(L);


    DeleteList(L);
    printf("After DeleteList.\n");

    TraverseList(L);

    return 0;
}