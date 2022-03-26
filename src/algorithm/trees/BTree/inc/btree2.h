/*
 * @Author       : elu
 * @Date         : 2022-02-17 17:56:31
 * @LastEditTime : 2022-02-18 14:06:49
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _BTREE_H
#define _BTREE_H

#define MAXLEVEL    (10)
#define MAX_M       (12)

typedef char KEYTYPE;

typedef struct BTNode
{
    int n;      // 节点中关键字个数
    struct BTNode *parent;  // 指向双亲节点
    KEYTYPE *keys;  // 关键字数组指针，m+1 个空间(正常最多m-1个关键字，插入时有可能增加一，此时要分裂节点)
    struct BTNode **childs; // 子树指针数组， m+1个空间(正常最多m个子树，插入时有可能增加一，此时要分裂节点)
} BTNode, *BTree;

typedef struct
{
    BTree pt;   // 指向找到的节点
    int i;  // 0..m-1， 在节点中的关键字序号
    int tag;    // 1.查找成功， 0.查找失败
} SearchResult; // B-树的查找结果类型

SearchResult BTreeSearch(BTree T, KEYTYPE key);   // B-树的搜索
BTree BTreeAllocNode();
BTree BTreeCreate(int m);   // B-树的创建
BTree BTreeInsert(BTree T, BTree q, int i, KEYTYPE k);  // B-树的插入,将k插入作为节点q的第i个关键字

void BTreeTraverse(BTree T);


struct Keyinfo  // 共level 层关键字，每层node个节点， 每节点n个关键字
{
    int levle;
    int node;
    int n;
    int flag;   // 是否有值
    KEYTYPE keys[MAX_M];    
} ;

struct NodeLevelInfo
{
    int level;      // 总共level层
    int nodeNum;    // 总节点数量
    int *nodes;     // 每层含有的node数量
    struct Keyinfo *keyinfo;    
};

#endif