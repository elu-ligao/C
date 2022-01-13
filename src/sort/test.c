/*
 * @Author       : elu
 * @Date         : 2022-01-02 11:37:10
 * @LastEditTime : 2022-01-03 17:44:34
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "sorts.h"

void getArray(SqList *L);
float getTime();
int diffArray(SqList *L1, SqList *L2);
int checkArray(SqList *L);

void genSLListFromSqList(SqList *L, SLList *S);
void checkSqSLLists(SqList *L, SLList *S);

int main(int argc, char *argv[])
{
    SqList *L = (SqList *)calloc(1, sizeof(SqList));
    SqList *L2 = (SqList *)calloc(1, sizeof(SqList));
    // SqList *L3 = (SqList *)calloc(1, sizeof(SqList));
    // SqList *L4 = (SqList *)calloc(1, sizeof(SqList));

    SLList *S = (SLList *)calloc(1, sizeof(SLList));
    
    getArray(L);
    memcpy(L2, L, sizeof(SqList));
    // memcpy(L3, L, sizeof(SqList));
    // memcpy(L4, L, sizeof(SqList));

    genSLListFromSqList(L, S);

    printf("L.length = %d\n", (*L).length);

    float start = getTime();
    quickSort(L);
    float cost = getTime() - start;
    printf("quickSort cost: %.6f s\n", cost);

    start = getTime();
    heapSort(L2);
    cost = getTime() - start;
    printf("heapSort cost: %.6f s\n", cost);

    // start = getTime();
    // int ddk = 5;
    // int dk[5] = {11, 7, 5, 3, 1};
    // shellSort(L3, dk, ddk);
    // cost = getTime() - start;
    // printf("shellSort cost: %.6f s\n", cost);

    // start = getTime();
    // mergeSort(L4);
    // cost = getTime() - start;
    // printf("mergeSort cost: %.6f s\n", cost);

    // start = getTime();
    // binsertSort(L);
    // cost = getTime() - start;
    // printf("binsertSort cost: %.6f s\n", cost);

    // start = getTime();
    // insertSort(L);
    // cost = getTime() - start;
    // printf("insertSort cost: %.6f s\n", cost);


    // start = getTime();
    // bubbleSort(L2);
    // cost = getTime() - start;
    // printf("bubbleSort cost: %.6f s\n", cost);


    // start = getTime();
    // selectSort(L2);
    // cost = getTime() - start;
    // printf("selectSort cost: %.6f s\n", cost);

    start = getTime();
    radixSort(S);
    cost = getTime() - start;
    printf("radixSort cost: %.6f s\n", cost);
    checkSqSLLists(L, S);

    diffArray(L, L2);
    // showArray(L);

    checkArray(L);
    checkArray(L2);
    // checkArray(L3);
    // checkArray(L4);

    free(L);    L = NULL;
    free(L2);   L2 = NULL;
    // free(L3);   L3 = NULL;
    // free(L4);   L4 = NULL;
    free(S);   S = NULL;

    return 0;
}

void checkSqSLLists(SqList *L, SLList *S)
{
    int i = 0, k = 0;
    for(i=1, k=(*S).r[k].next; i<=MAXSIZE; ++i, k=(*S).r[k].next)
    {
        if(atoi((*S).r[k].keys) != (*L).r[i].key)
        {
            printf("(*S).r[k].keys = %s, L.r[%d].key = %d\n", (*S).r[k].keys, i, (*L).r[i].key);
            return;
        }
    }
}


int checkArray(SqList *L)
{
    int i = 0;
    for(i=2; i<=(*L).length; ++i)
    {
        if((*L).r[i].key < (*L).r[i-1].key)
        {
            printf("r[%d].key = %d, r[%d].key = %d\n", i-1, (*L).r[i-1].key, i, (*L).r[i].key);
            return -1;
        }
    }
    return 0;
}

int diffArray(SqList *L1, SqList *L2)
{
    int i = 1;
    for(i=1; i<=L1->length; ++i)
    {
        if(L1->r[i].key != L2->r[i].key)
        {
            printf("i = %d, L1.r.key = %d, L2.r.key = %d\n", i, L1->r[i].key, L2->r[i].key);
            return 1;
        }
    }
    // printf("same\n");
    return 0;
}

void getArray(SqList *L)
{
    int i = 0;
    srand(time(NULL));
    for(i=1; i<=MAXSIZE; ++i)
    {
        (*L).r[i].key = rand()%MAXSIZE;
    }
    (*L).length = MAXSIZE;
}

float getTime()
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday (&tv , &tz);

    float secs = tv.tv_sec % 1000 + tv.tv_usec / 1000000.0; 
    // printf("secs = %.6f\n", secs);
    return secs;
}

void showArray(SqList *L)
{
    int i = 0;
    int row = 0;
    printf("%-3d ", ++row);
    for(i=1; i<=(*L).length; ++i)
    {
        printf("%-6d ", (*L).r[i].key);
        if(i%20 == 0)
        {
            printf("\n%-3d ", ++row);
        }   
    }
    printf("\n");
}



void genSLListFromSqList(SqList *L, SLList *S)
{
    int k = 0, n = (*L).length;
    int i = n;
    while(i)
    {
        i /= 10;
        ++k;
    }    
    for(i=1; i<=n; ++i)
    {
        sprintf((*S).r[i].keys, "%0*d", k, (*L).r[i].key);
    }
    (*S).keynum = k;
    (*S).recnum = n;
}