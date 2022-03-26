/*
 * @Author       : elu
 * @Date         : 2022-02-13 22:37:55
 * @LastEditTime : 2022-02-17 17:56:22
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef B_TREE_H
#define B_TREE_H

typedef char KEYTYPE;    // 关键字类型

struct BTNode   // B树节点结构
{
    int n;      // 关键字个数
    int leaf;   // 叶节点标志 1：是叶节点， 0： 内部节点
    int h;      // 高度
    KEYTYPE *keys;  // 关键字数组，从下标1开始存放，0下标不用
    struct BTNode **childs;  // 关键字子树指针数组，从下标1开始存放，0下标不用
};

struct BTreeNode    // BTree Search result
{
    struct BTNode *node;   // BTree 节点
    int i;         // 关键字索引(从1开始)
};


typedef struct BTreeNode *Position;
typedef struct BTNode *BTree;


Position BTreeSearch(BTree T, KEYTYPE k);   // 搜索B树
BTree BTreeCreate(int t);    // 创建一棵空的B树
BTree BTreeAllocateNode();  // 申请一个B树节点
void BTreeSplitChild(BTree x, int i);   // 分裂B树中的节点，x 是非满的内部节点， i满足 x.ci 为满关键字子节点
void BTreeInsert(BTree *T, KEYTYPE k);  // 沿树单程下行方式向B树插入关键字k
void BTreeInsertNonFull(BTree x, KEYTYPE k);    // 将关键字插入非满的节点x

void BTreeTraverse(BTree T);    // 遍历B树


BTree BTreeDelete(BTree T, KEYTYPE k);
BTree BTreeDeleteNonLeafNode(BTree T, int i);   // 内部节点删除关键字
BTree BTreeDeleteInChild(BTree X, int i, KEYTYPE k);    // 当前节点不含关键字，在X.ci子树中删除关键字k
BTree BTreeDeleteLeafNode(BTree X, int i);   // 删除当前节点X的第i个关键字
BTree BTreeUnionChild(BTree X, int i, BTree Y, BTree Z); // 将X的第i个关键字和节点Z合并到节点Y(Y,Z均只有t-1个关键字)
void BTreeDownUpKey(BTree X, int i, BTree Y, int y, BTree Z, int z);  // 将X的第i个关键字下降到Y作为Y的第y个关键字，并将Z的第z个关键字上升到X替换X的第i个关键字

#endif