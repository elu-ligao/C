#ifndef _SQ_BITREE_H_
#define _SQ_BITREE_H_

#include <stdbool.h>

// 二叉树的顺序存储结构，适合满二叉树

typedef union ElemType
{
    int  iVal;
    char sVal;
} ElemType, *pElemType;

typedef pElemType* SqBiTree;


SqBiTree initBiTree(int n);      // 构造n个元素的满二叉树顺序存储空间数组
SqBiTree createBiTreeLevelOrder(pElemType *datas, int n);   // 以层序遍历创建树来填充二叉树的存储
SqBiTree createBiTreePreOrder(pElemType *datas, int n);     // 以先序遍历创建树来填充二叉树的存储
SqBiTree createBiTreePostOrder(pElemType *datas, int n);    // 以后序遍历创建树来填充二叉树的存储

SqBiTree createBiTreeInPreOrder(pElemType *in, pElemType *pre, int n);   // 以中序和前序遍历的顺序创建树来填充二叉树的存储

void destroyBiTree(SqBiTree bt);    // 销毁二叉树
void clearBiTree(SqBiTree bt);      // 清空二叉树

bool biTreeIsEmpty(SqBiTree bt);    // 二叉树是否为空树
int rootOfBiTree(SqBiTree bt);      // 返回二叉树的根节点
pElemType value(SqBiTree bt, int cur);  // 返回二叉树cur结点的数据
void assign(SqBiTree bt, int cur, pElemType d);   // 为二叉树的cur个结点赋值d
int parentNode(SqBiTree bt, int cur);       // 返回二叉树的cur结点的父节点
int leftChild(SqBiTree bt, int cur);    // 返回二叉树的cur结点的左孩子
int rightChild(SqBiTree bt, int cur);   // 返回二叉树的cur结点的右孩子

char *levelOrderTraverse(SqBiTree T);       // 层序遍历
char *preOrderTraverse(SqBiTree T);         // 先序遍历
char *postOrderTraverse(SqBiTree T);        // 后序遍历
char *inOrderTraverse(SqBiTree T);          // 中序遍历

#endif