/*
 * @Author       : elu
 * @Date         : 2022-02-19 23:02:41
 * @LastEditTime : 2022-02-20 19:47:21
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../inc/hashSep.h"

/**
 * @descripttion:   字符串散列函数
 * @return {*}
 * @param {const char *} key
 * @param {int} TableSize
 */
int HashString(const char * key, int TableSize)
{
    unsigned int HashVal = 0;

    while(*key != '\0')
        HashVal = (HashVal << 5) + *key++;

    return HashVal % TableSize;
}

int IsPrime(int n)
{
    int i = 0;
    int ok = 1;

    for(i=3; i*i<n; i+=2)
    {
        if(n%i == 0)
        {
            ok = 0;
            break;
        }
    }

    return ok;
}

int NextPrime(int n)
{
    if(n%2)
        n += 2;
    else
        n += 1;

    while(!IsPrime(n))
        n += 2;

    return n;
}

/**
 * @descripttion:   hash表初始化
 * @return {*}
 * @param {int} TableSize
 */
HashTable InitializeTable(int TableSize)
{
    HashTable H;
    int i = 0;

    if(TableSize < MinTableSize)
    {
        TableSize = MinTableSize;
        printf("TableSize is too small, set it to %d.\n", TableSize);
    }

    // Allocate table
    H = (HashTable)calloc(1, sizeof(struct HashTbl));
    if(!H)
    {
        perror("HashTable calloc error.\n");
        return NULL;
    }
    H->TableSize = NextPrime(TableSize);

    // Allocate array of lists
    H->TheLists = (List *)calloc(H->TableSize, sizeof(List));
    if(!H->TheLists)
    {
        perror("HashTable's TheLists calloc error.\n");
        return NULL;
    }

    // Allocate list headers
    for(i=0; i<H->TableSize; ++i)
    {
        H->TheLists[i] = (List)calloc(1, sizeof(struct ListNode));
        if(!H->TheLists[i])
        {
            perror("HashTable's TheLists's header calloc error.\n");
            return NULL;
        }
    }

    return H;
}




int KeyCompare(char *a, char *b)
{
    return strcmp(a, b);
}

void KeyPut(char *dst, char *src)
{
    strcpy(dst, src);
}

/**
 * @descripttion:   查找哈希表中的key
 * @return {*}
 * @param {HashTable} H
 * @param {ElementType} key
 */
Position Find(HashTable H, ElementType key, int index)
{
    Position P;
    List L;

    L = H->TheLists[ index ];
    P = L->Next;

    while(P && KeyCompare(P->Element, key))
        P = P->Next;

    return P;
}

/**
 * @descripttion:   往哈希表H中插入key
 * @return {*}
 * @param {HashTable} H
 * @param {ElementType} key
 */
void Insert(HashTable H, ElementType key)
{
    Position P, NewCell;
    List L;

    int index = HashString(key, H->TableSize);
    // printf("key = [%s], index = %d\n", key, index);

    P = Find(H, key, index);
    if(NULL == P)
    {
        NewCell = (Position)calloc(1, sizeof(struct ListNode));
        if(!NewCell)
        {
            perror("calloc for new ListNode error.\n");
            return;
        }
        NewCell->Element = (ElementType)calloc(strlen(key)+1, sizeof(char));
        L = H->TheLists[ index ];
        NewCell->Next = L->Next;
        KeyPut(NewCell->Element, key);
        L->Next = NewCell;
    }
    return;
}



/**
 * @descripttion:   返回P的key值
 * @return {*}
 * @param {Position} P
 */
ElementType Retrieve(Position P)
{
    return P->Element;
}


void TraverseHashTable(HashTable H)
{   
    int i = 0;
    List L = NULL;

    for(i=0; i<H->TableSize; ++i)
    {
        L = H->TheLists[i];
        if(L->Next)
        {
            while(L->Next)
            {
                printf("%s ", L->Next->Element);
                L = L->Next;
            }
            printf("\n");
        }
    }
}

void DestroyTable(HashTable H);     // 销毁哈希表