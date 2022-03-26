/*
 * @Author       : elu
 * @Date         : 2022-02-20 18:32:22
 * @LastEditTime : 2022-02-20 23:24:53
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../inc/hashQuad.h"

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

int KeyCompare(char *a, char *b)
{
    return strcmp(a, b);
}

void KeyPut(char **dst, char *src)
{
    *dst = (char *)calloc(strlen(src)+1, sizeof(char));
    strcpy(*dst, src);
}

HashTable InitializeTable(int TableSize)
{
    HashTable H;

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

    // Allocate array of Cells
    H->TheCells = (Cell *)calloc(H->TableSize, sizeof(Cell));
    if(!H->TheCells)
    {
        perror("HashTable TheCells calloc error.\n");
        return NULL;
    }

    return H;
}

/**
 * @descripttion:   假设散列表的大小至少为表中元素个数的两倍，故平方探测法总能实现
 *                  否则要测试i(CollisionNum)的值。此处为平方探测法
 * 
 *                  --> 双散列： 取 R(小于TableSize的一个素数)
 *                      如 R = NextPrime(TableSize>>1)
 *                      hash2(X) = R - (X mod R)
 *                      冲突时分别在距离 hash2(X), 2*hash2(X), 3*hash(X).. 处探测
 * @return {*}
 * @param {HashTable} H
 * @param {ElementType} key
 */
Position Find(HashTable H, ElementType key)
{
    Position CurrentPos;
    int CollisionNum = 0;   // 冲突数

    CurrentPos = HashString(key, H->TableSize);
    
    while(H->TheCells[CurrentPos].Info != Empty && KeyCompare(key, H->TheCells[CurrentPos].Element) != 0)
    {
        // i start whith 1, check ((i+1)^2-i^2)%TableSize 是否为空
        CurrentPos += 2 * (++CollisionNum) - 1;
        if(CurrentPos >= H->TableSize)
            CurrentPos -= H->TableSize;
        
    }
    return CurrentPos;
}

/**
 * @descripttion:   平方探测法
 * @return {*}
 * @param {HashTable} H
 * @param {ElementType} key
 */
HashTable Insert(HashTable H, ElementType key)
{
    Position Pos = Find(H, key);
    
    // 平方探测法
    if(H->TheCells[Pos].Info != Legitimate)     // 该位置可用
    {
        H->TheCells[Pos].Info = Legitimate;
        KeyPut(&H->TheCells[Pos].Element, key);
        H->Size += 1;
    }
    // printf("insert key = [%s], H.Size = %d\n", key, H->Size);
    if((H->Size<<1) >= H->TableSize)
        H = ReHash(H);
    return H;
}


void TraverseHashTable(HashTable H)
{
    int i = 0;
    int cnt = 0;
    while(i < H->TableSize)
    {
        if(H->TheCells[i].Info == Legitimate)
        {
            printf("%s, ", H->TheCells[i].Element);
            if(++cnt % 10 == 0)
                printf("\n");
        }
        ++i;
    }
    printf("\n");
}

/**
 * @descripttion:   再散列时机：
 *                  1. 表填满一半就再散列
 *                  2. 当插入失败时再散列
 *                  3. 途中策略(middle-of-the-road), 当表到达某一个装填因子时进行再散列
 * @return {*}
 * @param {HashTable} H
 */
HashTable ReHash(HashTable H)
{
    // printf("---------- ReHash ----------\n");
    int i, OldSzie = H->TableSize;
    Cell *OldCells = H->TheCells;

    // Get double size new table
    H = InitializeTable(OldSzie<<1);
    
    // Scan througt old table, reinserting into new table
    for(i=0; i<OldSzie; ++i)
    {
        if(OldCells[i].Info == Legitimate)
            Insert(H, OldCells[i].Element);
    }

    free(OldCells);
    return H;
}


ElementType Retrieve(HashTable H, Position P);
void DestroyTable(HashTable H);