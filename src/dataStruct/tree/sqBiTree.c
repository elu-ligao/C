/*
 * @Author       : elu
 * @Date         : 2021-12-06 20:09:54
 * @LastEditTime : 2021-12-07 16:08:43
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqBiTree.h"

/**
 * @descripttion:   构造n个元素的满二叉树顺序存储空间数组, 申请n+1 个空间，从索引为1开始存放树结点
 * @return {*}
 * @param {int} n
 */
SqBiTree initBiTree(int n)
{
    SqBiTree bt = (SqBiTree)calloc(n+1, sizeof(pElemType));
    if(!bt)
    {
        perror("initBiTree error.\n");
        return NULL;
    }
    int i = 0;
    for(i=0; i<=n; ++i)
    {
        bt[i] = (pElemType)calloc(1, sizeof(ElemType));
        if(!bt[i])
        {
            perror("calloc for ElemType error.\n");
            return NULL;
        }
    }
    bt[0]->iVal = n;
    return bt;
}

/**
 * @descripttion:   以层序遍历创建树来填充二叉树的存储
 * @return {*}
 * @param {pElemType} *datas
 * @param {int} n
 */
SqBiTree createBiTreeLevelOrder(pElemType *datas, int n)
{
    SqBiTree bt = initBiTree(n);
    if(!bt) return NULL;
    int i = 0;
    for(i=1; i<=n; ++i)
    {
        bt[i]->sVal = datas[i-1]->sVal;
    }
    bt[0]->iVal = n;
    return bt;
}

bool checkBiTree(SqBiTree bt, int cur)
{
    if(!bt) return false;
    int n = bt[0]->iVal;
    if(cur > n || cur < 1) return false;  
    return true;  
}


/**
 * @descripttion:   以先序遍历创建树来填充二叉树的存储
 *      第一个是根节点，连续的元素是父子关系，第一次遇到#后设置下一次是右子树的标志
 *      元素是右子树的元素，判断是否还有子树，没有的话，下一个位置是父亲的右兄弟
 * @return {*}
 * @param {pElemType} *datas
 * @param {int} n
 */
SqBiTree createBiTreePreOrder(pElemType *datas, int n)
{
    // int num = n/2;       // 带#号总元素个数的要除2
    int num = n;
    SqBiTree bt = initBiTree(num);
    if(!bt) return NULL;
    
    int k = 1, i = 0;
    assign(bt, k, datas[i]);
    // int right = 0;
    k = leftChild(bt, k);  // #号判断法这行要注释掉
    while(i < n)
    {
        ++i;
        if(datas[i]->sVal != '#')
        {
            assign(bt, k, datas[i]);
            if(k*2 <= num)    // 左孩子存在
            {
                k = leftChild(bt, k);
            }   
            else if(k%2 == 1)   // 当前k是右孩子
            {
                k = parentNode(bt, k);
                k = parentNode(bt, k);
                k = rightChild(bt, k);
            }
            else if(k%2 == 0)   // 当前k是左孩子
            {
                k = rightChild(bt, parentNode(bt, k));
                if(k < 0)   break;
            }
        }
        #if 0
        if(datas[i]->sVal != '#')     // 数据
        {
            if(right)
            {
                right = 0;
                k++;
                // bt[k]->sVal = datas[i]->sVal;
                assign(bt, k, datas[i]);
                
                if(k*2 > n/2)    // 没有子树了
                {
                    k = parentNode(bt, k);
                    k = parentNode(bt, k);
                    k = leftChild(bt, k);
                }                
            }
            else
            {
                k <<= 1;
                // bt[k]->sVal = datas[i]->sVal;
                assign(bt, k, datas[i]);
            }            
        }
        else   // # 
        {
            right = 1;
            ++i;    // 多加一次，跳过右子树的#
        }
        #endif
    }
    return bt;
}

/**
 * @descripttion:   以后序遍历创建树来填充二叉树的存储
 * @return {*}
 * @param {pElemType} *datas
 * @param {int} n
 */
SqBiTree createBiTreePostOrder(pElemType *datas, int n)
{
    // int num = n/2;
    int num = n;
    SqBiTree bt = initBiTree(num);
    if(!bt) return NULL;

    int k = 1, i = n-1;
    assign(bt, k, datas[i]);
    k = rightChild(bt, k);
    while (i >= 1)
    {
        --i;
        if(datas[i]->sVal != '#')
        {
            assign(bt, k, datas[i]);
            if(k*2+1 <= num)    // 右孩子存在
            {
                k = rightChild(bt, k);
            }   
            else if(k%2 == 0)   // 当前k是左孩子
            {
                k = parentNode(bt, k);
                k = parentNode(bt, k);
                k = leftChild(bt, k);
            }
            else if(k%2 == 1)   // 当前k是右孩子
            {
                if(k*2 <= num)
                    k = leftChild(bt, k);
                else
                    k = leftChild(bt, parentNode(bt, k));
            }
        }
    }
    return bt;
}


/** 
 * @descripttion:   以中序遍历创建树来填充二叉树的存储
 * @return {*}
 * @param {pElemType} *datas
 * @param {int} n
 */
SqBiTree createBiTreeInPreOrder(pElemType *in, pElemType *pre, int n)
{
    return NULL;
}

/**
 * @descripttion:   销毁二叉树
 * @return {*}
 * @param {SqBiTree} bt
 */
void destroyBiTree(SqBiTree bt)
{
    if(!bt) return;
    int n = bt[0]->iVal;
    int i = 0;
    for(i=0; i<=n; ++i)
    {
        free(bt[i]);
        bt[i] = NULL;
    }
    free(bt);
    bt = NULL;
}

/**
 * @descripttion:   清空二叉树
 * @return {*}
 * @param {SqBiTree} bt
 */
void clearBiTree(SqBiTree bt)
{
    if(!bt) return;
    int n = bt[0]->iVal;
    int i = 0;
    for(i=0; i<=n; ++i)
    {
        memset(bt[i], 0, sizeof(ElemType));
    }   
}

/**
 * @descripttion:   二叉树是否为空树
 * @return {*}
 * @param {SqBiTree} bt
 */
bool biTreeIsEmpty(SqBiTree bt)
{
    if(!bt) return false;
    return bt[0]->iVal == 0;
} 

/**
 * @descripttion:   返回二叉树的根节点
 * @return {*}
 * @param {SqBiTree} bt
 */
int rootOfBiTree(SqBiTree bt)
{
    if(!bt) return -1;
    return 1;
}

/**
 * @descripttion:   返回二叉树cur结点的数据
 * @return {*}
 * @param {SqBiTree} bt
 * @param {int} cur
 */
pElemType value(SqBiTree bt, int cur)
{
    if(!checkBiTree(bt, cur))
        return NULL;
    return bt[cur];
}

/**
 * @descripttion:   为二叉树的cur个结点赋值d
 * @return {*}
 * @param {SqBiTree} bt
 * @param {int} cur
 * @param {pElemType} d
 */
void assign(SqBiTree bt, int cur, pElemType d)
{
    if(!checkBiTree(bt, cur))
        return;
    memcpy(bt[cur], d, sizeof(ElemType));
}

/**
 * @descripttion:   返回二叉树的cur结点的父节点
 * @return {*}      0 表示无父节点，即所求为根的父节点
 * @param {SqBiTree} bt
 * @param {int} cur
 */
int parentNode(SqBiTree bt, int cur)
{
    if(!checkBiTree(bt, cur))
        return -1;
    return cur/2;
}

/**
 * @descripttion:   返回二叉树的cur结点的左孩子
 *                  2*cur > n:  无左孩子, 否则左孩子为2*cur
 * @return {*}
 * @param {SqBiTree} bt
 * @param {int} cur
 */
int leftChild(SqBiTree bt, int cur)
{
    if(!checkBiTree(bt, cur))
        return -1;
    int index = cur << 1;
    if(index > bt[0]->iVal)
    {
        return -1;
    }
    return index;
}

/**
 * @descripttion:   返回二叉树的cur结点的右孩子
 *                  2*cur + 1 > n:  无右孩子, 否则右孩子为2*cur+1
 * @return {*}
 * @param {SqBiTree} bt
 * @param {int} cur
 */
int rightChild(SqBiTree bt, int cur)
{
    if(!checkBiTree(bt, cur))
        return -1;
    int index = (cur << 1) + 1;
    if(index > bt[0]->iVal)
    {
        return -2;
    }
    return index;
}

/**
 * @descripttion:   层序遍历
 * @return {*}
 * @param {SqBiTree} bt
 */
char *levelOrderTraverse(SqBiTree bt)
{
    if(!bt) return NULL;
    int n = bt[0]->iVal;
    int i = 0, k = 0;
    char *s = (char *)calloc(2*n+1, sizeof(char));
    if(!s)  return NULL;
    for(i=1; i<=n; ++i)
    {
        s[k++] = bt[i]->sVal;
        s[k++] = ' ';
    }
    printf("levelOrderTraverse: %s\n", s);
    return s;
}

void preOrder(SqBiTree bt, char *s, int cur)
{
    int start = strlen(s);
    if(cur < 0) 
    {
        // s[start++] = '#';
        // s[start++] = ' ';
        return;
    }
    s[start++] = bt[cur]->sVal;
    s[start++] = ' ';
    preOrder(bt, s, leftChild(bt, cur));
    preOrder(bt, s, rightChild(bt, cur));
}

/**
 * @descripttion:   先序遍历
 *      实际总结点数n, 满二叉树结点数N, 缺节点数(N-n), 
 *      用#填充完满二叉树的个数N+1(最后一层结点数为N/2+1), 此时带#的满二叉树层数比原二叉树多一层，总结点数为2*N+1
 *      设实际应该用#填充现有叶子结点的个数为x，则有x=2*N+1-n-2*(N-n)=n+1, 即#加上原有结点共有2*n+1个输出
 * @return {*}
 * @param {SqBiTree} bt
 */
char *preOrderTraverse(SqBiTree bt)
{
    if(!bt) return NULL;
    int n = bt[0]->iVal;
    char *s = (char *)calloc(2*(2*n+1)+1, sizeof(char));
    if(!s)  return NULL;

    preOrder(bt, s, 1);
    printf("preOrderTraverse: %s\n", s);
    return s;
}

void postOrder(SqBiTree bt, char *s, int cur)
{
    int start = strlen(s);
    if(cur < 0)
    {
        // s[start++] = '#';
        // s[start++] = ' ';
        return;        
    }
    postOrder(bt, s, leftChild(bt, cur));
    postOrder(bt, s, rightChild(bt, cur));
    start = strlen(s);
    s[start++] = bt[cur]->sVal;
    s[start++] = ' ';
}

/**
 * @descripttion:   后序遍历
 * @return {*}
 * @param {SqBiTree} bt
 */
char *postOrderTraverse(SqBiTree bt)
{
    if(!bt) return NULL;
    int n = bt[0]->iVal;
    char *s = (char *)calloc(2*(2*n+1)+1, sizeof(char));
    if(!s)  return NULL;

    postOrder(bt, s, 1);
    printf("postOrderTraverse: %s\n", s);
    return s;
}

void inOrder(SqBiTree bt, char *s, int cur)
{
    int start = strlen(s);
    if(cur < 0)
    {
        // s[start++] = '#';
        // s[start++] = ' ';
        return;        
    }
    inOrder(bt, s, leftChild(bt, cur));
    start = strlen(s);
    s[start++] = bt[cur]->sVal;
    s[start++] = ' ';
    inOrder(bt, s, rightChild(bt, cur));
}

/**
 * @descripttion:   中序遍历
 * @return {*}
 * @param {SqBiTree} bt
 */
char *inOrderTraverse(SqBiTree bt)
{
    if(!bt) return NULL;
    int n = bt[0]->iVal;
    char *s = (char *)calloc(2*(2*n+1)+1, sizeof(char));
    if(!s)  return NULL;

    inOrder(bt, s, 1);
    printf("inOrderTraverse: %s\n", s);
    return s;
}