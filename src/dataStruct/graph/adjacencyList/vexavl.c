#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vexavl.h"



/**
 * @descripttion:   // 新建节点
 * @return {*}
 */
pavlElement newAvlElement(int keylen)
{
    pavlElement e = (pavlElement)calloc(1, sizeof(avlElement));
    if(NULL == e)
    {
        perror("newAvlElement error.\n");
        return NULL;
    }
    e->key = (KeyType)calloc(keylen+1, sizeof(char));
    return e;
}

/**
 * @descripttion:   // 创建二叉树节点
 * @return {*}
 */
pavlTree newAvlNode(int keylen)
{
    pavlTree pNode = (pavlTree)calloc(1, sizeof(avlNode));
    if(NULL == pNode)
    {
        perror("newAvlNode error.\n");
        return NULL;
    }
    pNode->data = newAvlElement(keylen);
    if(NULL == pNode->data)
    {
        return NULL;
    }
    return pNode;
}

/**
 * @descripttion:   // 初始化二叉树
 * @return {*}
 */
pavlTree initAVLTree()
{
    return NULL;
}


static int Height(pavlTree T)
{
    if(NULL == T)
        return -1;
    return T->data->otherInfo.height;
}

static int Max(int a, int b)
{
    return a > b ? a : b;
}

// LL: adjust 变为 left 的右节点 
static pavlTree adjustLL(pavlTree T)
{
    pavlTree left = T->lchild;
    pavlTree p = T->parent;

    if(p)
    {
        if(T->data->otherInfo.rlFlag == 1)
            p->lchild = left;
        else
            p->rchild = left;
    }

    T->parent = left;
    T->lchild = left->rchild;
    if(left->rchild) 
    {
        left->rchild->parent = T;
        left->rchild->data->otherInfo.rlFlag = 1;
    }

    left->data->otherInfo.rlFlag = T->data->otherInfo.rlFlag;
    T->data->otherInfo.rlFlag = 2;

    left->parent = p;
    left->rchild = T;
    // 更新高度及平衡因子
    T->data->otherInfo.height = Max(Height(T->lchild), Height(T->rchild)) + 1;
    T->data->otherInfo.balanceFactor = Height(T->lchild) - Height(T->rchild);
    
    left->data->otherInfo.height = Max(Height(left->lchild), Height(left->rchild)) + 1;
    left->data->otherInfo.balanceFactor = Height(left->lchild) - Height(left->rchild);

    return left;
}


// RR: adjust 变为 right 左节点
static pavlTree adjustRR(pavlTree T)
{
    pavlTree right = T->rchild;
    pavlTree p = T->parent;
    if(p)
    {
        if(T->data->otherInfo.rlFlag == 1)
            p->lchild = right;
        else
            p->rchild = right;
    }

    T->parent = right;
    T->rchild = right->lchild;
    if(right->lchild)
    {
        right->lchild->data->otherInfo.rlFlag = 2;
        right->lchild->parent = T;
    }

    right->data->otherInfo.rlFlag = T->data->otherInfo.rlFlag;
    T->data->otherInfo.rlFlag = 1;

    right->parent = p;
    right->lchild = T;

    T->data->otherInfo.height = Max(Height(T->lchild), Height(T->rchild)) + 1;
    T->data->otherInfo.balanceFactor = Height(T->lchild) - Height(T->rchild);
    
    right->data->otherInfo.height = Max(Height(right->lchild), Height(right->rchild)) + 1;
    right->data->otherInfo.balanceFactor = Height(right->lchild) - Height(right->rchild);

    return right;
}


int compareKey(KeyType k1, KeyType k2)
{
    return strcmp(k1, k2);
}

/**
 * @descripttion:   // 查找关键字域为key的节点
 * @return {*}
 * @param {pavlTree} T
 * @param {pavlElement} key
 */
pavlTree searchAvl(pavlTree T, pavlElement find)
{
    if(T)
    {
        if(compareKey(T->data->key, find->key) == 0)       // 根节点关键域与待查找关键域相等，find it, return
        {
            return T;
        }
        else if(compareKey(find->key, T->data->key) < 0)   // 待查找关键域小于根节点，往左子树继续查找
        {
            return searchAvl(T->lchild, find);
        }
        else    // 否则往右子树查找 
        {
            return searchAvl(T->rchild, find);
        }
    }
    return NULL;
}


void copyElement(pavlElement dst, pavlElement src)
{
    strcpy(dst->key, src->key);
    dst->index = src->index;
    memcpy(&dst->otherInfo, &src->otherInfo, sizeof(InfoType));
}



// 先对右子树做LL, 在对本身做一次RR
static pavlTree adjustRL(pavlTree T)
{
    T->rchild = adjustLL(T->rchild);
    return adjustRR(T);
}


// 先对左子树做RR, 在对本身做一次LL
static pavlTree adjustLR(pavlTree T)
{
    T->lchild = adjustRR(T->lchild);
    return adjustLL(T);
}


/**
 * @descripttion:   // 插入
 * @return {*}
 * @param {pavlTree} *T
 * @param {pavlElement} e
 * @param {pavlTree} p
 */
pavlTree insertAvl(pavlTree S, pavlElement e, int rlFlag, pavlTree p)
{
    if(NULL == S)
    {
        S = newAvlNode(strlen(e->key));
        copyElement(S->data, e);
        S->data->otherInfo.rlFlag = rlFlag;
        S->parent = p;
    }
    else if(S->data->key == e->key)
    {
        // fprintf(stderr, "elem key [%d] existed.\n", e->key);
        return S;
    }
    else if(compareKey(S->data->key, e->key) > 0)
    {
        
        S->lchild = insertAvl(S->lchild, e, 1, S);     // 往左边插入， 只可能左边高度增加 
        if(Height(S->lchild) - Height(S->rchild) == 2)
        {
            
            if(compareKey( e->key, S->lchild->data->key) < 0)  // LL
                S = adjustLL(S);
            else    // LR
                S = adjustLR(S);
            
        }
    }
    else if(compareKey( S->data->key, e->key) < 0)
    {
        
        S->rchild = insertAvl(S->rchild, e, 2, S);
        if(Height(S->rchild) - Height(S->lchild) == 2)
        {
            if(compareKey( e->key, S->rchild->data->key) > 0)  // RR
                S = adjustRR(S);
            else    // RL
                S = adjustRL(S);
                
        }
    }

    S->data->otherInfo.height = Max(Height(S->lchild), Height(S->rchild)) + 1;
    S->data->otherInfo.balanceFactor = Height(S->lchild) - Height(S->rchild);

    return S;
}

/**
 * @descripttion:   // 删除
 * @return {*}
 * @param {pavlTree} *T
 * @param {pavlElement} e
 */
int deleteAvl(pavlTree *T, pavlElement e)
{
    return 0;
}


/*
 *  void freeAvl(pavlTree *T)
 *  desc:   释放二叉树内存
 *  input:  pavlTree T   avl根节点指针
 *  output: 
 *  return: 
*/
void freeAvl(pavlTree *T)
{
    pavlTree S = *T;
    pavlTree left, right;
    if(S)
    {
        // printf("free %d\n", S->data->key);
        left = S->lchild;
        right = S->rchild;
        
        free(S->data);
        S->data = NULL;
        S->parent = NULL;
        S->lchild = NULL;
        S->rchild = NULL;
        free(S);
        S = NULL;

        freeAvl(&left);
        freeAvl(&right);
    }
}

/*
 *  void outputSortedAvl(pavlTree T)
 *  desc:   中序遍历
 *  input:  pavlTree T   avl根节点指针
 *  output: 中序遍历的结果
 *  return: 
*/
void outputSortedAvl(pavlTree T, int rlFlat, int prtFlag)
{
    if(T)
    {
        if(T->lchild)
        {
            outputSortedAvl(T->lchild, 1, prtFlag);
        }
        // T->data->otherInfo.index = *index;
        T->data->otherInfo.rlFlag = rlFlat;
        // *index += 1;
        if(prtFlag == 1)
            // printf("key = %d, index = %d, rlFlag = %d, curLevel = %d, totalLevel = %d, Height = %d, nodeNum = %d, balance factor = %d, parent = %d\n", 
            // T->data->key, T->data->otherInfo.index, T->data->otherInfo.rlFlag, T->data->otherInfo.curLevel, T->data->otherInfo.totalLevel, 
            // T->data->otherInfo.height, T->data->otherInfo.nodeNum, T->data->otherInfo.balanceFactor, T->parent?T->parent->data->key:-1);
            printf("key = %s, index = %d, rlFlag = %d, Height = %d, balance factor = %d, parent = [%d]%s\n", 
            T->data->key, T->data->index, T->data->otherInfo.rlFlag, 
            T->data->otherInfo.height, T->data->otherInfo.balanceFactor, T->parent?T->parent->data->index:-1, T->parent?T->parent->data->key:"");
        
        else if(prtFlag == 2)
            printf("%d[%s] ", T->data->index, T->data->key);
        if(T->rchild)
        {
            outputSortedAvl(T->rchild, 2, prtFlag);
        }
    }
}