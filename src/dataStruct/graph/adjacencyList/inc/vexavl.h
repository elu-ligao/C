#ifndef VEX_AVL_H
#define VEX_AVL_H

typedef char*  KeyType;    // 关键字类型

typedef struct InfoType
{
    // int curLevel;       // 当前层数
    // int totalLevel;     // 总层数
    // int index;          // 中序遍历后排列索引位置
    int rlFlag;         // 属于父节点左右子树标志位 1-是父节点的左子树 2-是父节点的右子树
    int height;         // 高度 
    int balanceFactor;  // 平衡因子 值为该节点左子树与右子树的深度只差 绝对值不应大于1 
}InfoType;   // 其他数据项类型

/* 每个节点的数据域 */
typedef struct avlElement
{
    /* data */
    KeyType     key;        
    int         index;
    InfoType    otherInfo;  // 其他数据项
}avlElement, *pavlElement;   

/* 二叉树节点 */
typedef struct avlNode
{
    /* data */
    pavlElement data;          // 数据域
    struct avlNode *lchild, *rchild, *parent;
}avlNode, *pavlTree;

// 新建节点
pavlElement newAvlElement(int keylen);

// 创建二叉树节点
pavlTree newAvlNode(int keylen);

// 创建二叉树
pavlTree initAVLTree();

// 查找关键字域为key的节点
pavlTree searchAvl(pavlTree T, pavlElement key);


// 插入
pavlTree insertAvl(pavlTree S, pavlElement e, int rlFlag, pavlTree p);

// 删除
// int deleteAvl(pavlTree *T, pavlElement e);


// 释放
void freeAvl(pavlTree *T);


void outputSortedAvl(pavlTree T, int rlFlat, int prtFlag);

#endif