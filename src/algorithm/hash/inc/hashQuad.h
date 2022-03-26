/*
 * @Author       : elu
 * @Date         : 2022-02-20 18:32:30
 * @LastEditTime : 2022-02-20 19:41:26
 * @LastEditors  : Please set LastEditors
 * @Description  :  开放地址法
 */

#ifndef _HASH_QUAD_H
#define _HASH_QUAD_H

#define MinTableSize        (3)

typedef char * ElementType;

enum KindOfEntry
{
    Empty,          // 空的
    Legitimate,     // 正常的
    Deleted         // 懒惰删除
};

struct HashEntry
{
    ElementType         Element;
    enum KindOfEntry    Info;
};

typedef struct HashEntry Cell;

struct HashTbl
{
    int TableSize;
    int Size;
    Cell *TheCells;
};

typedef unsigned int Index;
typedef Index Position;

typedef struct HashTbl *HashTable;

HashTable InitializeTable(int TableSize);
Position Find(HashTable H, ElementType key);
HashTable Insert(HashTable H, ElementType key);
ElementType Retrieve(HashTable H, Position P);
HashTable ReHash(HashTable H);

void TraverseHashTable(HashTable H);
void DestroyTable(HashTable H);

#endif