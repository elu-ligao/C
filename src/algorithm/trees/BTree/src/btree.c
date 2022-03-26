/*
 * @Author       : elu
 * @Date         : 2022-02-13 22:38:01
 * @LastEditTime : 2022-02-18 11:16:35
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../inc/btree.h"

static int minDegree = 0;

/**
 * @descripttion:   在B树中搜索关键字k
 * @return {*}      k所在的节点及所在节点中的关键字索引(从1开始)    y.keyi == k 的索引
 * @param {BTree} X B树树根X
 * @param {KEYTYPE} k   关键字k
 */
Position BTreeSearch(BTree X, KEYTYPE k)
{
    int i = 1;
    // 找出最小下标i，使得 k <= X.keys[i]
    while(i <= X->n && k > X->keys[i])
        i += 1;
    
    // printf("BTreeSearch: X is [%s], i = %d\n", X->keys, i);
    Position P = (Position)calloc(1, sizeof(struct BTreeNode));
    if(i <= X->n && k == X->keys[i]) // 在节点X找到关键字k
    {
        P->node = X;
        P->i = i;
        return P;
    }
    else if(X->leaf)    // 没有找到且已经遍历到了叶节点
    {
        return NULL;
    }
    else    // 递归搜索X的相应子树
    {
        return BTreeSearch(X->childs[i], k);
    }
}

/**
 * @descripttion:   创建一棵空的B树
 * @return {*}
 * @param {t}       B树最小度数 (阶数m >= 2*t, t>=2)
 */
BTree BTreeCreate(int t)
{
    if(t<2)
    {
        perror("最小度数小于2.\n");
        return NULL;
    }
    BTree X = BTreeAllocateNode();
    minDegree = t;
    X->leaf = 1;
    return X;
}

/**
 * @descripttion:   申请一个B树节点
 * @return {*}
 * @param {t}       B树最小度数 (阶数m >= 2*t, t>=2)
 */
BTree BTreeAllocateNode()
{
    BTree X = (BTree)calloc(1, sizeof(struct BTNode));
    if(!X)
    {
        perror("Alloc BTNode error.\n");
        return NULL;
    }
    X->keys = (KEYTYPE*)calloc(2*minDegree, sizeof(KEYTYPE));
    if(!X->keys)
    {
        perror("Alloc BTNode.keys error.\n");
        return NULL;
    }
    
    X->keys[0] = ' ';
    X->childs = (BTree*)calloc(2*minDegree+1, sizeof(BTree));
    if(!X->childs)
    {
        perror("Alloc BTNode.childs error.\n");
        return NULL;
    }
    return X;
}

/**
 * @descripttion:   分裂B树中的节点，X 是非满的内部节点， i满足 X.ci 为满关键字子节点
 * @return {*}
 * @param {BTree} X
 * @param {int} i
 */
void BTreeSplitChild(BTree X, int i)
{
    int t = minDegree;
    BTree Z = BTreeAllocateNode();
    BTree Y = X->childs[i];     // Y 是 X 的第i个孩子，是满节点，有2*t-1个关键字个2*t个孩子
    
    Z->leaf = Y->leaf;  // 将 Y 分裂成 Y 和 Z， 均有 t-1 个关键字和 t 个孩子
    Z->n = t - 1;   
    Y->n = t - 1;

    int j = 0;
    // 将 Y 的后 t-1 个孩子分给 Z
    for(j=1; j<=t-1; ++j)
        Z->keys[j] = Y->keys[t+j];

    // Y 是内部节点，非叶子节点, 叶子节点childs为NULL
    if(!Y->leaf)    
    {
        for(j=1; j<=t; ++j)
            Z->childs[j] = Y->childs[t+j];
    }

    // 将 X 子树从第 i+1 个依次后移一位
    for(j=X->n+1; j>=i+1; --j)
        X->childs[j+1] = X->childs[j];
    
    // Z 成为 X 的第 i+1 个孩子
    X->childs[i+1] = Z;

    // 将 X 从第 i 个关键字一次后移一位
    if(X->n > 0)
        for(j=X->n+1; j>=i; --j)
            X->keys[j+1] = X->keys[j];

    // Y 的中间关键字成为 X 的第 i 个关键字
    X->keys[i] = Y->keys[t];

    // 调整 X 的关键字个数
    X->n += 1;

    for(j=0; j<t; ++j)
        Y->keys[t+j] = 0;
    for(j=1; j<=t; ++j)
        Y->childs[t+j] = NULL;

    return;
}

/**
 * @descripttion:   沿树单程下行方式向B树插入关键字k
 * @return {*}
 * @param {BTree} *T
 * @param {KEYTYPE} k
 */
void BTreeInsert(BTree *T, KEYTYPE k)
{
    int t = minDegree;
    BTree R = *T;
    // printf("T = %p\n", *T);
    
    // 根节点已满
    if(R->n == 2*t - 1)
    {
        BTree S = BTreeAllocateNode();  // 创建空节点S作为根
        S->childs[1] = R;   // 使R成为S的第一棵子树

        BTreeSplitChild(S, 1);      // 分裂这棵满关键字的子树R， 为新根S的第一棵子树
    // BTreeTraverse(S);
        BTreeInsertNonFull(S, k);   // 再将k插入
        *T = S;
    }
    else
    {
        BTreeInsertNonFull(R, k);
        *T = R;
    }
    // printf("T = %p, n = %d, \n", *T, (*T)->n);
    
    // BTreeTraverse(*T);

    return;
}

/**
 * @descripttion:   将关键字插入非满的节点x
 * @return {*}
 * @param {BTree} x
 * @param {KEYTYPE} k
 */
void BTreeInsertNonFull(BTree X, KEYTYPE k)
{
    int i = X->n;

    // X 是叶节点
    if(X->leaf)
    {
        // 将大于k的关键字依次后移一位
        while(i>=1 && k < X->keys[i])
        {
            X->keys[i+1] = X->keys[i];
            --i;
        }
        X->keys[i+1] = k;   // 插入k
        X->n += 1;  // 调整节点关键字个数
        // printf("insert [%c] success. n = %d\n", k, X->n);
    }
    else
    {
        while(i>=1 && k < X->keys[i])
            --i;
        i += 1;     // 定位到X的子节点 i

        if(X->childs[i]->n == 2 * minDegree -1)     // 节点已满
        {
            BTreeSplitChild(X, i);      // 分裂该满节点
            if(k > X->keys[i])  ++i;    // 确定k下降到哪个子节点
        }

        BTreeInsertNonFull(X->childs[i], k);    // 递归插入
    }
    return;
}

void TraverseKeys(BTree X, int *h)
{
    // printf("X = %p, X.h = %d, X.n = %d, X.keys = [%s]\n", X, X->h, X->n, X->keys);
    if(X->h > *h)
    {
        *h = X->h;
        printf("\n\n");
    }
    else if(*h != 0)
    {
        printf("  |  ");
    }
    
    int i = 0;
    for(i=1; i<=X->n; ++i)
    {
        if(i > 1)   printf(" ");
        printf("%c", X->keys[i]);
    }
}

/**
 * @descripttion: 
 * @return {*}
 * @param {BTree} T
 */
void BTreeTraverse(BTree T)
{
    if(!T)  return;

    int N = 50;     // 层序遍历队列长度
    BTree *Q = (BTree*)calloc(N, sizeof(BTree));
    int rear = 0, front = 0;
    Q[rear++] = T;
    int h = 0;
    
    int i = 1;
    while(rear!=front)
    {
        // printf("front = %d\n", front);
        TraverseKeys(Q[front], &h);   // 打印当前节点
        if(!Q[front]->leaf)
        {
            // 内部节点将子树指针追加到队列
            for(i=1; i<=Q[front]->n+1 && Q[front]->childs[i]; ++i)
            {
                Q[rear] = Q[front]->childs[i];
                // printf("Q[rear] = %p\n", Q[rear]);
                Q[rear]->h = h + 1;
                // printf("Q[rear]->h = %d, Q[rear] = %p, rear = %d\n", Q[rear]->h, Q[rear], rear);
                ++rear;
                rear %= N;
            }
        }
        ++front;
        front %= N;
    }
    printf("\n");
}

void CopyKey(BTree X, int i, KEYTYPE k)
{
    X->keys[i] = k;
}

/**
 * @descripttion:   B树中删除关键字k
 * @return {*}
 * @param {BTree} T
 * @param {KEYTYPE} k
 */
BTree BTreeDelete(BTree X, KEYTYPE k)
{
    // printf("Now X: \n");
    // BTreeTraverse(X);
    int i = 1;
    while(i<X->n && k > X->keys[i])
        ++i;

    if(i<=X->n && k == X->keys[i])  // 关键字在当前节点中
    {
        if(X->leaf)     // 当前节点是叶节点
        {
            X = BTreeDeleteLeafNode(X, i);
        }
        else    // 当前节点是内部节点
        {
            X = BTreeDeleteNonLeafNode(X, i);
        }
    }
    else    // 关键字不在当前节点中
    {
        X = BTreeDeleteInChild(X, i, k);
    }

            
    return X;
}

BTree BTreeDeleteInChild(BTree X, int i, KEYTYPE k)
{
    int t = minDegree;
    int child = i;
    
    // printf("BTreeDeleteInChild: i = %d, X->childs[i]->n = %d, t-1 = %d\n", i, X->childs[i]->n, t-1);
    
    // X.ci 只有t-1个关键字
    // 相邻兄弟至少包含t个关键字，则将X的某个相应关键字降至X.ci中，
    // 将X.ci的相邻的至少t个关键字的兄弟的一个关键字升至X，将该兄弟相应的孩子指针上移到X.ci中
    // 这样就使得X.ci增加了一个额外的关键字
    if(X->childs[i]->n == t-1)
    {
        BTree Y, Z;
        // 左邻兄弟包含至少t个关键字
        if(i-1 >= 1 && X->childs[i-1]->n >= t)
        {
            Y = X->childs[i];
            Z = X->childs[i-1];

            BTreeDownUpKey(X, i-1, Y, 1, Z, Z->n);
        }
        // 右邻兄弟包含至少t个关键字
        else if(i <= X->n && X->childs[i+1]->n >= t)
        {
            Y = X->childs[i];
            Z = X->childs[i+1];

            BTreeDownUpKey(X, i, Y, Y->n+1, Z, 1);
        }
        // 左右兄弟均只含有t-1个关键字，则将X.ci与一个兄弟合并
        // 即将X的一个关键字移至新合并的节点，使之成为该节点的中间关键字
        else
        {
            if(i-1 >= 1)   // X.ci与左兄弟及X.keys[i]关键字合并
            {
                X = BTreeUnionChild(X, i-1, X->childs[i-1], X->childs[i]);
            }
            else if(i <= X->n)  // X.ci与右兄弟及X.keys[i-1]关键字合并
            {
                X = BTreeUnionChild(X, i, X->childs[i], X->childs[i+1]);
            }

            child = 1;
            // 找出最小下标i，使得 k <= X.keys[i]
            while(child <= X->n && k > X->keys[child])
                child += 1;
        }
    }
    // printf("BTreeDeleteInChild: Now X is [%s], child = %d [%s], n = %d\n", X->keys, child, X->childs[child]->keys, X->childs[child]->n);
    // 最后通过对X的某个合适的节点递归删除
    if(X->childs[child]->n > t-1)
        X->childs[child] = BTreeDelete(X->childs[child], k);
    else 
        X = BTreeDelete(X, k);
    return X;
}

BTree BTreeDeleteNonLeafNode(BTree X, int i)
{            
    int t = minDegree;
    BTree Y = X->childs[i];
    BTree Z = X->childs[i+1];

    // X中前于k的子节点Y包含不少于t个关键字个数
    // 确定k在Y中的前驱prek, 递归删除prek, 并在X中用prek替代k
    if(Y->n >= t)   
    {
        KEYTYPE prek = Y->keys[Y->n];
        Y = BTreeDelete(Y, prek);
        CopyKey(X, i, prek);
    }
    // X中后于k的子节点Z包含不少于t个关键字
    // 确定k在Z中的后去postk， 递归删除postk, 并在X中用postk替代k
    else if(Z->n >= t)
    {
        KEYTYPE postk = Z->keys[1];
        Z = BTreeDelete(Z, postk);
        CopyKey(X, i, postk);
    }
    // Y和Z都只包含t-1个关键字，将k和Z合并到Y，这样X就失去了k和指向Z的指针
    // 且Y现在成为了满节点。 然后释放Z， 并递归的从Y中删除k
    else
    {
        X = BTreeUnionChild(X, i, Y, Z);
    }
    return X;
}

/**
 * @descripttion:   删除当前节点X的第i个关键字
 * @return {*}
 * @param {BTree} X
 * @param {int} i
 */
BTree BTreeDeleteLeafNode(BTree X, int i)
{
    int j = i;
    // 将X的第i个以后的关键字依次前移一位
    for(j=i; j<X->n; ++j)
        X->keys[j] = X->keys[j+1];
        
    // 将X的第i个以后的子节点指针依次前移一位
    // for(j=i; j<=X->n; ++j)
    //     X->childs[j] = X->childs[j+1];
    
    // X->childs[X->n+1] = NULL;
    X->keys[X->n] = 0;
    X->n -= 1;

    // 如果X只有一个关键字被删除，则X成为了空节点，释放他
    if(0 == X->n)
    {
        BTree Tmp = X;
        X = X->childs[1];
        free(Tmp);
        Tmp = NULL;
    }
    return X;
}

/**
 * @descripttion:   将X的第i个关键字和节点Z合并到节点Y(Y,Z均只有t-1个关键字)
 * @return {*}
 * @param {BTree} X
 * @param {int} i
 * @param {BTree} Y
 * @param {BTree} Z
 */
BTree BTreeUnionChild(BTree X, int i, BTree Y, BTree Z)
{
    int t = minDegree;
    KEYTYPE k = X->keys[i];
    // printf("BTreeUnionChild: X is [%s], i = %d, Y is [%s], Z is [%s]\n", X->keys, i, Y->keys, Z->keys);
    // k成为Y的中间关键字
    CopyKey(Y, t, k);

    int j = 1;
    int n = X->n;
    
    // Z节点的关键字追加到Y的后半部，t-1个
    for(j=1; j<t; ++j)
        Y->keys[t+j] = Z->keys[j];

    // Z节点的子树追加到Y的后半部，t个
    for(j=1; j<=t; ++j)
        Y->childs[t+j] = Z->childs[j];

    // 更新Y的关键字个数
    Y->n = 2*t-1;

    // X的第i个以后的关键字依次前移一位
    for(j=i; j<n; ++j)
        X->keys[j] = X->keys[j+1];

    // X的第i个以后的子树指针依次前移一位
    for(j=1+1; j<=n; ++j)
        X->childs[j] = X->childs[j+1];

    X->keys[n] = 0;
    X->childs[n+1] = NULL;
    X->n -= 1;

    free(Z);

    // printf("BTreeUnionChild: X is [%s], Y is [%s]\n", X->keys, Y->keys);
    // 如果X只有一个关键字，将其合并到Y后X成为了空节点，释放他
    if(0 == X->n)
    {
        BTree Tmp = X;
        X = Y;
        free(Tmp);
        Tmp = NULL;
    }
    return X;
}

/**
 * @descripttion:   将X的第i个关键字下降到Y作为Y的第y个关键字，并将Z的第z个关键字上升到X替换X的第i个关键字
 * @return {*}
 * @param {BTree} X
 * @param {int} i
 * @param {BTree} Y
 * @param {int} y
 * @param {BTree} Z
 * @param {int} z
 */
void BTreeDownUpKey(BTree X, int i, BTree Y, int y, BTree Z, int z)
{
    KEYTYPE kDown, kUp;
    kDown = X->keys[i];
    kUp = Z->keys[z];

    int j = 0;
    
    // printf("BTreeDownUpKey: X is [%s], i = %d, Y is [%s], y = %d, Z is [%s], z = %d\n", X->keys, i, Y->keys, y, Z->keys, z);
    // 将Y节点的第y个关键字以后的关键字依次后移一位
    for(j=Y->n; j>=y; --j)
        Y->keys[j+1] = Y->keys[j];
    
    // 将Y节点的第y个子树以后的子树依次后移一位
    for(j=Y->n+1; j>=y; --j)
        Y->childs[j+1] = Y->childs[j];
    
    // 将Z的相应子树移到Y中相应的位置
    if(1 == y)
        Y->childs[y] = Z->childs[z+1];
    else if(1 == z)
    {
        Y->childs[y+1] = Z->childs[1];
        // 将Z节点第z个以后的子树指针依次前移一位
        for(j=z; j<Z->n; ++j)
            Z->childs[j] = Z->childs[j+1];
            
        // 将Z节点第z个以后的关键字依次前移一位
        for(j=z; j<=Z->n; ++j)
            Z->keys[j] = Z->keys[j+1];
    }
    
    // 将X的关键字下降到Y使其成为第y个关键字
    Y->keys[y] = kDown;
    Y->n += 1;

    Z->childs[Z->n+1] = NULL;
    Z->keys[Z->n] = 0;
    Z->n -= 1;

    CopyKey(X, i, kUp);
}
