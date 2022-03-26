/*
 * @Author       : elu
 * @Date         : 2022-02-21 17:00:28
 * @LastEditTime : 2022-02-21 23:41:24
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "../inc/binHeap.h"

int main(int argc, char *argv[])
{
    int Arr[16] = {150, 80, 40, 30, 10, 70, 110, 100, 20, 90, 60, 50, 120, 140, 130}; 
    int N = 15;
    PriorityQueue H = BuildHeap(Arr, N);

    ShowElements(H);
    Destroy(H);

    H = Initialize(N);
    int i = 0;
    for(i=0; i<N; ++i)
        Insert(H, Arr[i]);
    ShowElements(H);
    Destroy(H);

    return 0;
}