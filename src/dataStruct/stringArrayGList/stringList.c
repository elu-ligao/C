#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stringList.h"

int indexBF(char *S, char *T, int pos)
{
    int i = pos, j = 0;
    int lenS = strlen(S), lenT = strlen(T);

    // int k = 0;
    while (i<lenS && j<lenT)
    {
        // ++k;
        if(S[i] == T[j])
        {
            ++i; ++j;
        }
        else
        {
            i = i - j + 1;
            j = 0;
        }
    }
    // printf("k = %d\n", k); 
    if(j == lenT)
    {
        return i - j;
    }
    return -1;
}


int indexKMP(char *S, char *T, int pos)
{
    int *next = getNextVal(T);
    if(!next)   return -2;

    int lenS = strlen(S), lenT = strlen(T);

    int i = 0, j = -1;

    // int k = 0;
    while (i<lenS && j<lenT)
    {
        // ++k;
        if(-1 == j || S[i] == T[j])
        {
            ++i; ++j;
        }
        else
            j = next[j];
    }
    // printf("k = %d\n", k); 
    if(j == lenT)
        return i - j;

    return -1;
}

int *getNext(char *T)
{
    int lenT = strlen(T);
    int *next = (int *)calloc(lenT, sizeof(T));
    if(!next)   
    {
        perror("calloc for next array error.\n");
        return NULL;
    }
    
    int i = 0, j = -1;
    next[0] = -1;
    
    while (i < lenT-1)
    {
        if(-1 == j || T[i] == T[j])
        {
            next[++i] = ++j;
        }
        else
            j = next[j];
    }
    
    return next;
}

int *getNextVal(char *T)
{
    int lenT = strlen(T);
    int *next = (int *)calloc(lenT, sizeof(T));
    if(!next)   
    {
        perror("calloc for next array error.\n");
        return NULL;
    }
    
    int i = 0, j = -1;
    next[0] = -1;
 
    while (i < lenT-1)
    {
        if(-1 == j || T[i] == T[j])
        {
            ++i; ++j;
            if(T[i] != T[j])
                next[i] = j;
            else
                next[i] = next[j];
        }
        else
            j = next[j];
    }
    
    return next;   
}
