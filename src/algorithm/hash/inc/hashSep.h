/*
 * @Author       : elu
 * @Date         : 2022-02-19 22:44:19
 * @LastEditTime : 2022-02-20 17:50:56
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _HashSep_H
#define _HashSep_H

#define MinTableSize    (101)

typedef char * ElementType;     // 关键字类型

struct ListNode     // 分离链接法 separate chaining
{
    ElementType Element;
    struct ListNode *Next;
};

typedef struct ListNode *Position;

typedef Position List;

struct HashTbl
{
    int TableSize;
    List *TheLists;
};

typedef struct HashTbl *HashTable;

int HashString(const char *key, int TableSize);     // 字符串散列函数

HashTable InitializeTable(int TableSize);   // hash表初始化
void DestroyTable(HashTable H);     // 销毁哈希表
Position Find(HashTable H, ElementType key, int index);    // 查找哈希表中的key
void Insert(HashTable H, ElementType key);      // 往哈希表H中插入key
ElementType Retrieve(Position P);       // 

void TraverseHashTable(HashTable H);





#endif