#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "avl.h"

/*
 *  pavlElement newAvlElement()
 *  desc:   新建数据节点
 *  input: 
 *  output: 
 *  return: success pavlElement 数据节点指针
 *          fail    NULL    
*/
pavlElement newAvlElement()
{
    pavlElement e = (pavlElement)calloc(1, sizeof(avlElement));
    if(NULL == e)
    {
        perror("newAvlElement error.\n");
        return NULL;
    }
    return e;
}

/*
 *  pavlTree newAvlNode()
 *  desc:   创建二叉树节点
 *  input: 
 *  output: 
 *  return: success pavlTree avl节点指针
 *          fail    NULL
*/
pavlTree newAvlNode()
{
    pavlTree pNode = (pavlTree)calloc(1, sizeof(avlNode));
    if(NULL == pNode)
    {
        perror("newAvlNode error.\n");
        return NULL;
    }
    pNode->data = newAvlElement();
    if(NULL == pNode->data)
    {
        return NULL;
    }
    return pNode;
}


void doAdjust(pavlTree *T, pavlTree adj, char *adjType, int flag)
{
    pavlTree p = adj->parent;
    // int index = 0;
    // outputSortedAvl(*T, &index, 0, 1);
    
    int rootRL = adj->data->otherInfo.rlFlag;
    if(memcmp(adjType, "LL", 2) == 0)
    {
        // 将最小非平衡子树根节点firstAdj的左子树节点left作为当前调整数的根(更新父节点相关信息)
        // 将firstAdj 作为left 的右子树
        // 将left 的右子树作为 firstAdj 的左子树
        pavlTree left = adj->lchild;  
        pavlTree ll = left->lchild;  
        pavlTree lr = left->rchild;     // 判断存在性 
        left->parent = p;   // 更新调整好后根的父节点为p
        left->rchild = adj;
        left->data->otherInfo.rlFlag = rootRL;
        left->data->otherInfo.balanceFactor = 0;

        adj->parent = left;
        adj->lchild = lr;
        adj->data->otherInfo.rlFlag = 2;

        adj->data->otherInfo.balanceFactor = 0;

        if(ll && flag)        // 由旋转而来
        {
            ll->data->otherInfo.balanceFactor = 0;
            if(memcmp(adjType, "LLR", 3) == 0)  // LRR
                ll->data->otherInfo.balanceFactor = 1;      // add
        }

        if(adj->rchild && !lr)    // adj 有右节点且left 无右节点
        {
            adj->data->otherInfo.balanceFactor = -1;
        }

        if(lr)      // 对应原来的 lrr
        {
            lr->data->otherInfo.rlFlag = 1;
            lr->parent = adj;
        }
        if(p)   // firstAdj has parent
        {
            if(rootRL == 1)
                p->lchild = left;
            else if(rootRL == 2)
                p->rchild = left;
        }
        else    // firstAdj is the root
            *T = left;
    }
    else if(memcmp(adjType, "LR", 2) == 0)
    {
        // 先将LR型转成LL型
        // 将最小非平衡子树根节点firstAdj的左子树节点left作为新调整数的根(更新父节点相关信息)
        // 将left右子树lr 作为left的父节点 left 作为lr的右节点
        // 更新lr左子树lrl的信息(如果有就是LRL型) lrl.p = left  left.r = lrl
        pavlTree left = adj->lchild;  
        pavlTree lr = left->rchild;
        pavlTree lrl = lr->lchild;      // 判断存在性 

        adj->lchild = lr;
        lr->parent = adj;
        lr->lchild = left;
        left->parent = lr;
        left->rchild = lrl;
        lr->data->otherInfo.rlFlag = 1;

        if(lrl)     // LRL 
        {
            lrl->parent = left;
            left->rchild = lrl;
            lr->data->otherInfo.rlFlag = 2;
        }
        // 继续做一次LL型旋转调整
        adjType[1] = 'L';
        return doAdjust(T, adj, adjType, 1);
    
    }
    else if(memcmp(adjType, "RR", 2) == 0)
    {
        // 将最小非平衡子树根节点firstAdj的右子树节点right作为当前调整数的根(更新父节点相关信息)
        // 将firstAdj 作为right 的左子树
        // 将right 的左子树作为 firstAdj 的右子树
        pavlTree right = adj->rchild;  
        pavlTree rr = right->rchild;  
        pavlTree rl = right->lchild;     // 判断存在性 
        right->parent = p;   // 更新调整好后根的父节点为p
        right->lchild = adj;
        right->data->otherInfo.rlFlag = rootRL;
        right->data->otherInfo.balanceFactor = 0;

        adj->parent = right;
        adj->rchild = rl;
        adj->data->otherInfo.rlFlag = 1;

        adj->data->otherInfo.balanceFactor = 0;

        if(rr && flag) 
        {
            rr->data->otherInfo.balanceFactor = 0;
            if(memcmp(adjType, "RRL", 3) == 0)  // RRL
                rr->data->otherInfo.balanceFactor = -1;
        }


        if(adj->lchild && !rl)    // adj 有左节点且right 无左节点
        {
            adj->data->otherInfo.balanceFactor = 1;
        }

        if(rl)
        {
            rl->data->otherInfo.rlFlag = 2;
            rl->parent = adj;
        }
        if(p)   // firstAdj has parent
        {
            if(rootRL == 1)
                p->lchild = right;
            else if(rootRL == 2)
                p->rchild = right;
        }
        else    // firstAdj is the root
            *T = right;
    }
    else if(memcmp(adjType, "RL", 2) == 0)
    {
        // 先将RL型转成RR型
        // 将最小非平衡子树根节点firstAdj的右子树节点right作为新调整数的根(更新父节点相关信息)
        // 将right左子树rl 作为right的父节点 right 作为rl的右节点
        // 更新rl右子树rlr的信息(如果有就是RLR型) rlr.p = right  right.l = rlr
        pavlTree right = adj->rchild;  
        pavlTree rl = right->lchild;
        pavlTree rlr = rl->rchild;      // 判断存在性 

        rl->parent = adj;
        adj->rchild = rl;
        rl->rchild = right;
        right->parent = rl;
        right->lchild = rlr;
        rl->data->otherInfo.rlFlag = 2;

        if(rlr)     // RLR
        {
            rlr->parent = right;
            right->lchild = rlr;
            rl->data->otherInfo.rlFlag = 1;
        }
        
        // 继续做一次LL型旋转调整
        adjType[1] = 'R';
        return doAdjust(T, adj, adjType, 1);
    }
    // printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
    // index = 0;
    // outputSortedAvl(*T, &index, 0, 1);
}

char *reverseChar(char *str)
{
    int i = 0;
    int len = strlen(str);
    for(i=0; i<len/2; ++i)
    {
        str[i] = str[i]^str[len-1-i];
        str[len-1-i] = str[i]^str[len-1-i];
        str[i] = str[i]^str[len-1-i];
    }
    return str;
}

/*
 *  void adjustAvlNode(pavlTree *T, pavlTree ist)
 *  desc:   调整节点使保持平衡树的特性
 *  input:  pavlTree *T     平衡树根节点指针
 *          pavlTree ist   插入节点指针 
 *  output: 
 *  return: 
*/
void adjustAvlNode(pavlTree *T, pavlTree ist)
{
    if(*T == ist)  // 第一次插入，即插入的节点就是根节点
    {
        // inst->data->otherInfo.balanceFactor = 0;     // 默认就是0，初始化已经为0了
        return;
    }

    pavlTree p = ist;
    pavlTree firstAdj = NULL;
    char    adjType[20] = {0};      // 引发 *** stack smashing detected ***: terminated
                                    // stack smashing detected __stack_chk_fail () at stack_chk_fail.c:24
    int     times = 0;
    int     go = 1;

    while(go && p->parent)
    {
        printf("~~~ p %d rl = %d, parent %d bf = %d\n", p->data->key, p->data->otherInfo.rlFlag, p->parent->data->key, p->parent->data->otherInfo.balanceFactor);
        if(p->data->otherInfo.rlFlag == 1)         // p位于左子树， 
        {
            adjType[times++] = 'L';
            p->parent->data->otherInfo.balanceFactor += 1;  // 父节点的bf值只能增大1
            go = p->parent->data->otherInfo.balanceFactor;  // 父节点平衡因子bf 变更后为0， 不改变祖父节点bf值
        }
        else if(p->data->otherInfo.rlFlag == 2)    // p位于右子树
        {
            adjType[times++] = 'R';
            p->parent->data->otherInfo.balanceFactor -= 1;  // 父节点的bf值只能减小1
            go = p->parent->data->otherInfo.balanceFactor;  // 父节点平衡因子bf 变更后为0， 不改变祖父节点bf值
        }
        printf("~~~ parent %d bf = %d, \n", p->parent->data->key, p->parent->data->otherInfo.balanceFactor);
        p = p->parent;
       
        if(p->data->otherInfo.balanceFactor == 2 || p->data->otherInfo.balanceFactor == -2)
        {
            firstAdj = p;
            break;
        }
    }

    if(firstAdj)    // 开始旋转调整 
    {
        printf("************ firstAdj = %p, now p = %d, balanceFactor = %d\n", firstAdj, p->data->key, p->data->otherInfo.balanceFactor);
        reverseChar(adjType);
        doAdjust(T, firstAdj, adjType, 0);
    }
    // printf("$$$$$$$$$$$$$$  adjust is over...\n");
    return;
}


/*
 *  pavlTree createAVLTree(pavlElement elemlists)
 *  desc:   创建二叉树
 *  input:  pavlElement elemlists   数据域元素列表
 *  output: pavlTree root    avl根节点指针
 *  return: success pavlTree avl根节点指针
 *          fail    NULL
*/
pavlTree createAVLTree(pavlElement elemlists, int n)
{
    pavlTree root = NULL;
    int i;
    int index = 0;
    
    for(i=0; i<n; ++i)
    {
        root = insertAvl(root, &elemlists[i], 0, NULL);
    }
    updateLevelInfo(root, 1, 0);
    
    return root;
}

/*
 *  pavlTree searchAvl(pavlTree T, KeyType key)
 *  desc:   查找关键字域为key的节点
 *  input:  pavlTree T   avl根节点指针
 *          KeyType key 关键数据域
 *  output: 
 *  return: success pavlTree avl节点指针
 *          fail       NULL
*/
pavlTree searchAvl(pavlTree T, pavlElement find)
{
    if(T)
    {
        if(T->data->key == find->key)       // 根节点关键域与待查找关键域相等，find it, return
        {
            return T;
        }
        else if(find->key < T->data->key)   // 待查找关键域小于根节点，往左子树继续查找
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

/*
 *  pavlTree minAvl(pavlTree T)
 *  desc:   返回最小键值的对象
 *  input: 
 *  output: 
 *  return: success pavlTree avl节点指针
 *          fail    NULL
*/
pavlTree minAvl(pavlTree T)
{
    pavlTree p = NULL;
    pavlTree t = T;
    while(t)
    {
        p = t;
        t = t->lchild;
    }
    return p;
}

/*
 *  pavlTree maxAvl(pavlTree T)
 *  desc:   返回最大键值的对象
 *  input: 
 *  output: 
 *  return: success pavlTree avl节点指针
 *          fail    NULL
*/
pavlTree maxAvl(pavlTree T)
{
    pavlTree p = NULL;
    pavlTree t = T;
    while(t)
    {
        p = t;
        t = t->rchild;
    }
    return p;
}

/*
 *  pavlTree predecessorAvl(pavlTree T, pavlElement key)
 *  desc:   返回键值仅次于(更小)给定键的对象
 *  input:  pavlTree T   avl根节点指针
 *          pavlElement key 指定的关键值    
 *  output: 
 *  return: success pavlTree avl键值仅次于(更小)给定键的对象节点指针
 *          fail    NULL
*/
pavlTree predecessorAvl(pavlTree T, pavlElement key)
{
    pavlTree x = searchAvl(T, key);      // 先找到关键域key的节点x
    if(x->lchild)                       // x有左子树，则左子树最大的节点就是仅次于x的节点
    {
        return maxAvl(x->lchild);       // 返回左子树最大节点
    }
    // 否则沿着x向上遍历，第一次左拐的节点就是所求仅次于x的节点
    pavlTree p = x->parent;
    while (p)
    {
        if(p->rchild == x)      // x是p的右节点，即p是x左拐的父节点
            return p;   
        x = p;                  // 继续往上查找
        p = x->parent;
    }
    
    return NULL;
}

/*
 *  pavlTree successorAvl(pavlTree T, pavlElement key)
 *  desc:   返回键值仅大于(更大)给定键的对象
 *  input:  pavlTree T   avl根节点指针
 *          pavlElement key 指定的关键值    
 *  output: 
 *  return: success pavlTree avl键值仅大于(更大)给定键的对象节点指针
 *          fail    NULL
*/
pavlTree successorAvl(pavlTree T, pavlElement key)
{
    pavlTree x = searchAvl(T, key);      // 先找到关键域key的节点x
    if(x->rchild)                       // x有右子树，则右子树最小的节点就是仅大于x的节点
    {
        return minAvl(x->rchild);       // 返回右子树最小节点
    }
    // 否则沿着x向上遍历，第一次右拐的节点就是所求仅大于x的节点
    pavlTree p = x->parent;
    while (p)
    {
        if(p->lchild == x)      // x是p的左节点，即p是x右拐的父节点
            return p;   
        x = p;                  // 继续往上查找
        p = x->parent;
    }
    
    return NULL;
}

/*
 *  void outputSortedAvl(pavlTree T)
 *  desc:   中序遍历
 *  input:  pavlTree T   avl根节点指针
 *  output: 中序遍历的结果
 *  return: 
*/
void outputSortedAvl(pavlTree T, int *index, int rlFlat, int prtFlag)
{
    if(T)
    {
        if(T->lchild)
        {
            outputSortedAvl(T->lchild, index, 1, prtFlag);
        }
        T->data->otherInfo.index = *index;
        T->data->otherInfo.rlFlag = rlFlat;
        *index += 1;
        if(prtFlag == 1)
            printf("key = %d, index = %d, rlFlag = %d, curLevel = %d, totalLevel = %d, Height = %d, nodeNum = %d, balance factor = %d, parent = %d\n", 
            T->data->key, T->data->otherInfo.index, T->data->otherInfo.rlFlag, T->data->otherInfo.curLevel, T->data->otherInfo.totalLevel, 
            T->data->otherInfo.height, T->data->otherInfo.nodeNum, T->data->otherInfo.balanceFactor, T->parent?T->parent->data->key:-1);
        else if(prtFlag == 2)
            printf("%-3d", T->data->key);
        if(T->rchild)
        {
            outputSortedAvl(T->rchild, index, 2, prtFlag);
        }
    }
}


/*
 *  int countTreeLevel(pavlTree T, int total)
 *  desc:   插入数据域
 *  input:  pavlTree T      avl根节点指针
 *          int total       当前节点包含的总层数 
 *  output: 
 *  return:     总层数 
*/
int countTreeLevel(pavlTree T, int total)
{
    int l = 0, r = 0;
    if(T)
    {
        l = countTreeLevel(T->lchild, total+1);
        r = countTreeLevel(T->rchild, total+1);
        return l > r ? l : r;
    }
    return total;
}

/*
 *  int updateLevelInfo(pavlTree T, int curLevel, int totalLevel)
 *  desc:   插入数据域
 *  input:  pavlTree T      avl根节点指针
 *          int curLevel    当前第几层
 *          int totalLevel  总共多少层
 *  output: 
 *  return: 以当前节点为子树所包含的节点数 
*/
int updateLevelInfo(pavlTree T, int curLevel, int totalLevel)
{
    if(totalLevel <= 0)
    {
        totalLevel = 0;
        totalLevel = countTreeLevel(T, totalLevel);
        T->data->otherInfo.totalLevel = totalLevel;
        T->data->otherInfo.curLevel = 0;
    }

    int l = 0, r = 0;
    if(T)
    {
        if(T->lchild)
        {
            T->lchild->data->otherInfo.curLevel = curLevel;
            T->lchild->data->otherInfo.totalLevel = totalLevel;
            l = updateLevelInfo(T->lchild, curLevel+1, totalLevel);
        }
        if(T->rchild)
        {
            T->rchild->data->otherInfo.curLevel = curLevel;
            T->rchild->data->otherInfo.totalLevel = totalLevel;
            r = updateLevelInfo(T->rchild, curLevel+1, totalLevel);
        }
        T->data->otherInfo.nodeNum = l + r + 1;
        return l + r + 1;
    }
    return 0;
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


/*
 *  void insertAvl(pavlTree &T, pavlElement e)
 *  desc:   插入数据域
 *  input:  pavlTree T      avl根节点指针
 *          pavlElement e   数据域指针
 *          pavlTree p      T的父节点
 *  output: 
 *  return: success with 0 else failure
*/
pavlTree insertAvl(pavlTree S, pavlElement e, int rlFlag, pavlTree p)
{
    if(NULL == S)
    {
        S = newAvlNode();
        S->parent = p;
        S->data->key = e->key;
        S->data->otherInfo = e->otherInfo;
        S->data->otherInfo.rlFlag = rlFlag;
    }
    else if(S->data->key == e->key)
    {
        // fprintf(stderr, "elem key [%d] existed.\n", e->key);
        return S;
    }
    else if(S->data->key > e->key)
    {
        
        S->lchild = insertAvl(S->lchild, e, 1, S);     // 往左边插入， 只可能左边高度增加 
        if(Height(S->lchild) - Height(S->rchild) == 2)
        {
            
            if(e->key < S->lchild->data->key)  // LL
                S = adjustLL(S);
            else    // LR
                S = adjustLR(S);
            
        }
    }
    else if(S->data->key < e->key)
    {
        
        S->rchild = insertAvl(S->rchild, e, 2, S);
        if(Height(S->rchild) - Height(S->lchild) == 2)
        {
            if(e->key > S->rchild->data->key)  // RR
                S = adjustRR(S);
            else    // RL
                S = adjustRL(S);
                
        }
    }

    S->data->otherInfo.height = Max(Height(S->lchild), Height(S->rchild)) + 1;
    S->data->otherInfo.balanceFactor = Height(S->lchild) - Height(S->rchild);

    return S;
}

// 将src 的关键字更新为 dst的关键字
void updateAvlElemInfo(pavlTree src, pavlTree dst)
{
    src->data->key = dst->data->key;
}

/*
 *  void deleteAvl(pavlTree *T, pavlElement e)
 *  desc:   删除数据域
 *  input:  pavlTree T   avl根节点指针
 *          pavlElement e   数据域指针
 *  output: 
 *  return: success with 0 else failure
*/
int deleteAvl(pavlTree *T, pavlElement e)
{
    // find e
    // printf("to be find element key = %d\n", e->key);
    pavlTree TT = *T;
    pavlTree S = searchAvl(*T, e);
    // printf("S is %d[%p]\n", S ? S->data->key : -1, S);
    if(NULL == S)
        return -1;
    // printf("find the deleted element %d[%p], father is %d[%p]\n", S->data->key, S, S->parent ? S->parent->data->key : -1, S->parent);
#if 0
    if(NULL == S->lchild && NULL == S->rchild)  // S is leaf node
    {
        // delete S
        if(S != *T) // S is not the root
        {
            if(S->data->otherInfo.rlFlag == 1)  // S is left child
            {
                S->parent->lchild = NULL;
            }
            else    // S is right child
            {
                S->parent->rchild = NULL;
            }
        }
        // *S = deleteAvlLeafFunc(*S, e, 0, NULL);
    }   // end S is leaf node
    else if(S->lchild && S->rchild)     //  S have two children node
    {
        pavlTree leftMax = maxAvl(S->lchild);
        if(leftMax->data->otherInfo.rlFlag == 1)    // S's left child's max node is left node
        {
            // leftMax is S->left
            pavlTree Sl = S->lchild;     // S has only left child
            S->lchild->parent = S->parent;
            S->lchild->rchild = leftMax->rchild;
            leftMax->rchild = S->rchild;
            S->rchild->parent = leftMax;
            if(leftMax->rchild)
            {
                leftMax->rchild->parent = S->lchild;
            }
            if(S != *T) // S is not the root
            {
                if(S->data->otherInfo.rlFlag == 1)  // S is left child
                {
                    S->parent->lchild = Sl;
                }
                else    // S is right child
                {
                    S->parent->rchild = Sl;;
                }
            }
            else
                TT = Sl;
        }   // end S's left child's max node is left node
        else    // S's left child's max node is right node
        {
            // change S and leftMax, then delete new S, connect new S's child if new S has
            
            // leftMax has only left child
            if(leftMax->lchild)
            {
                leftMax->lchild->parent = leftMax->parent;
            }
            leftMax->parent->rchild = leftMax->lchild;

            leftMax->lchild = S->lchild;
            S->lchild->parent = leftMax;
            leftMax->rchild = S->rchild;
            S->rchild->parent = leftMax;
            leftMax->parent = S->parent;

            if(S != *T) // S is not the root
            {
                if(S->data->otherInfo.rlFlag == 1)  // S is left child
                {
                    S->parent->lchild = leftMax;
                }
                else    // S is right child
                {
                    S->parent->rchild = leftMax;
                }
            }
            else
                TT = leftMax;
        }
    }   // end S have two children node
    else    // S have only one child
    {
        pavlTree Schild = S->lchild ? S->lchild : S->rchild;
        Schild->parent = S->parent;     // S's child's parent direct to S'parent
        if(S != *T) // S is not the root
        {
            if(S->data->otherInfo.rlFlag == 1)  // S is left child
            {
                S->parent->lchild = Schild;
            }
            else    // S is right child
            {
                S->parent->rchild = Schild;;
            }
        }
        else
            TT = Schild;
    }   
#endif
    // 找到替换S位置的结点 predecessor
    printf("S is %d\n", S->data->key);
    pavlTree predecessor = maxAvl(S->lchild);
    // 删除 predecessor
    if(!predecessor)    predecessor = S;
    // printf("predecessor is %d\n", predecessor->data->key);
    *T = deleteAvlLeaf(*T, predecessor->data, 0, NULL);
    // 将 predecessor的关键信息域替换待删结点e的关键信息域
    updateAvlElemInfo(S, predecessor);
    // free predecessor
    free(predecessor->data);
    predecessor->data = NULL;
    predecessor->lchild = NULL;
    predecessor->rchild = NULL;
    predecessor->parent = NULL;
    free(predecessor);
    predecessor = NULL;

    *T = TT;
    updateLevelInfo(*T, 1, 0);
    return 0;
}




/**
 * @descripttion:   删除平衡二叉树结点：
 *                  1. 找到待删除结点S
 *                  2. 定位S的predecessor，即S左子树最大的结点predecessor(最右下角，即肯定没有右子树的结点)
 *                  3. 删除predecessor
 *                      3.1 predcessor 无左子树，说明predecessor 是叶子结点，直接删除
 *                      3.2 predcessor 有左子树，将其左子树结点替代predecessor 结点位置
 *                  4. 更新height和bf信息
 *                  5. 根据bf失衡信息做调整：
 *                      5.1 如果待删除的结点(这里是predecessor结点)是其父节点的左结点，删除之后只有可能左子树高度减小
 *                          即产生失衡时只有可能是右子树高度等于2，这时判断右子树的bf：
 *                              如果右子树bf==-1，说明是右子树的右子树过高，做RR调整
 *                              如果右子树bf==0，说明右子树的左右子树高度相等，此时可以做RR调整或RL调整
 *                              如果右子树bf==1，说明右子树的左子树高度过高，应做RL调整
 *                      5.2 类似操作
 *                  6. 将predecessor 的信息域替换掉待删结点e的信息域
 * @return {*}
 * @param {pavlTree} S
 * @param {pavlElement} e
 * @param {int} rlFlag
 * @param {pavlTree} p
 */
pavlTree deleteAvlLeaf(pavlTree S, pavlElement e, int rlFlag, pavlTree p)
{
    if(NULL == S)
    {
        return NULL;
    }
    // printf("S.data.key = %d, e.key = %d, rlFlag = %d, S.parent.data = %d\n",
    //     S?S->data->key:-1, e->key, rlFlag, p?p->data->key:-1);
    if(S->data->key == e->key)  // 删除结点
    {
        if(!S->lchild)  // 没有左子树
        {
            return NULL;
        }
        // 有左子树，将左子树根结点取代待删除的S结点位置
        S->lchild->parent = p;
        if(p && rlFlag == 1)
        {
            p->lchild = S->lchild;  
            S = p->lchild;
        }
        else if(p && rlFlag == 2)
        {
            p->rchild = S->lchild;
            S = p->rchild;
        }
        
    }
    else if(S->data->key > e->key)
    {
        
        S->lchild = deleteAvlLeaf(S->lchild, e, 1, S);     // delete on the left child， 只可能左边高度减少 
        if(Height(S->rchild) - Height(S->lchild) == 2)
        {
            
            if(S->rchild->data->otherInfo.balanceFactor == -1)  // RR
                S = adjustRR(S);
            else if(S->rchild->data->otherInfo.balanceFactor == 0)  // RR or RL
                S = adjustRR(S);
            else if(S->rchild->data->otherInfo.balanceFactor == 1)   // RL
                S = adjustRL(S);
        }
    }
    else if(S->data->key < e->key)
    {
        
        S->rchild = deleteAvlLeaf(S->rchild, e, 2, S);
        if(Height(S->lchild) - Height(S->rchild) == 2)
        {
            if(S->lchild->data->otherInfo.balanceFactor == 1)  // LL
                S = adjustLL(S);
            else if(S->lchild->data->otherInfo.balanceFactor == 0)  // LL or LR
                S = adjustLL(S);
            else if(S->lchild->data->otherInfo.balanceFactor == -1)   // LR
                S = adjustLR(S);
        }
    }

    S->data->otherInfo.height = Max(Height(S->lchild), Height(S->rchild)) + 1;
    S->data->otherInfo.balanceFactor = Height(S->lchild) - Height(S->rchild);

    return S;
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
 *  void drawAvlTree(pavlTree T, int prtFlag)
 *  desc:   删除数据域
 *  input:  pavlTree T   avl根节点指针
 *          int prtFlag   打印标志  (1-详细信息, 2-中序遍历)
 *              0   - 只打印树形节点
 *              1   - 树形节点 + 节点详细信息
 *              2   - 树形节点 + 中序遍历结果
 *              10  - 输出节点详细信息 + 树形节点
 *              12  - 输出节点详细信息 + 树形节点 + 中序遍历结果
 *              
 *  output: 
 *  return: success with 0 else failure
*/
void drawAvlTree(pavlTree T, int prtFlag)
{
    int index = 0;
    outputSortedAvl(T, &index, 0, prtFlag/10);
    int i, j, k;
    int start = 0, last = 0, totalLevel = T->data->otherInfo.totalLevel;
    int maxData = 1 << totalLevel;  // 最后一层最多节点个数
    int curMaxData = 0;             // 当前层最多节点个数
    pavlTree *curbfs = (pavlTree *)calloc(maxData, sizeof(pavlTree));    // 存储当前层的节点
    pavlTree *bfs = (pavlTree *)calloc(maxData, sizeof(pavlTree));    // 存储下一层的节点
    char level[maxData];         // 保存当前层有无数据标志
    char nextlevel[maxData];     // 保存下一层有无数据标志
    char sData[maxData*3+1], sLink[maxData*3+1];

    // 初始化根节点为下一层信息
    memset(nextlevel, 0, sizeof(nextlevel));
    nextlevel[0] = '1';
    memset(bfs, 0, sizeof(bfs));
    bfs[0] = T;
    for (i=1; i<=totalLevel; ++i)
    {
        last = 0;
        memset(level, 0, sizeof(level));
        memcpy(level, nextlevel, sizeof(level));
        memset(curbfs, 0, sizeof(pavlTree)*maxData);
        memcpy(curbfs, bfs, sizeof(pavlTree)*maxData);

        memset(nextlevel, '0', sizeof(nextlevel));
        nextlevel[sizeof(nextlevel)-1] = 0;
        memset(bfs, 0, sizeof(bfs));

        curMaxData = 1 << (i-1);
        // 输出数据并更新下一层信息
        index = 0;
        memset(sData, 0, sizeof(sData));
        for(j=0; j<curMaxData; ++j)
        {
            if(level[j] == '1')
            {
                start = curbfs[j]->data->otherInfo.index;
                for(k=0; k<start-last; ++k)
                {
                    strcat(sData, "   ");
                }
                last = start;

                // if(curbfs[j]->data->otherInfo.rlFlag == 2)
                // {
                //     sprintf(sData, "%s%-3d", sData, curbfs[j]->data->key);
                // }
                // else
                // {
                //     sprintf(sData, "%s%3d", sData, curbfs[j]->data->key);
                // }
                sprintf(sData, "%s%-3d", sData, curbfs[j]->data->key);
                last += 1;

                if(curbfs[j]->lchild)
                {
                    index = 2*j;    // 2*(j+1)-1-1
                    bfs[index] = curbfs[j]->lchild;
                    nextlevel[index] = '1';
                }
                if(curbfs[j]->rchild)
                {
                    index = 2*j+1;  // 2*(j+1)-1-1+1
                    bfs[index] = curbfs[j]->rchild;
                    nextlevel[index] = '1';
                }
            }
        }
        strcat(sData, "\n");
        printf("%s", sData);
        
        // 输出连接符  
        last = 0;
        memset(sLink, 0, sizeof(sLink));
        // if(i > 1)
        {
            for(j=0; j<(curMaxData<<1); ++j)
            {
                if(nextlevel[j] == '1')
                {
                    start = bfs[j]->data->otherInfo.index;
                    if(bfs[j]->data->otherInfo.rlFlag == 1)         // left
                    {
                        for(k=0; k<start-last; ++k)
                        {
                            strcat(sLink, "   ");
                        }

                        strcat(sLink, "|--");

                        for(k=start+1; k<bfs[j]->parent->data->otherInfo.index; ++k)
                        {
                            strcat(sLink, "---");
                        }
                        if(bfs[j]->parent->rchild)
                            strcat(sLink, "/\\-");
                        else
                            strcat(sLink, "/  ");
                        last = bfs[j]->parent->data->otherInfo.index + 1;
                    }   // end if left
                    else if(bfs[j]->data->otherInfo.rlFlag == 2)    // right
                    {
                        if(bfs[j]->parent->lchild)  // have left brother
                        {
                            for(k=0; k<start-last; ++k)
                            {
                                strcat(sLink, "---");
                            }
                            strcat(sLink, "-| ");
                            last = bfs[j]->data->otherInfo.index + 1;
                        }
                        else    // only right self
                        {
                            // get parent node
                            for(k=last; k<bfs[j]->parent->data->otherInfo.index; ++k)
                            {
                                strcat(sLink, "   ");
                            }
                            strcat(sLink, "  \\");

                            for(k=bfs[j]->parent->data->otherInfo.index; k<start-1; ++k)
                            {
                                strcat(sLink, "---");
                            }
                            strcat(sLink, "-| ");

                            last = bfs[j]->data->otherInfo.index+1;
                        }   
                    }   // end else if right
                }   // end if
            }   // end for
            strcat(sLink, "\n");
            printf("%s", sLink);
        }   // end if i > 1
    }

    index = 0;
    outputSortedAvl(T, &index, 0, prtFlag%10);
    printf("\n");
}


// int height(pavlTree T)
// {
//     if(NULL == T)   return 0;
//     int left = height(T->lchild);
//     int right = height(T->rchild);
//     return (left > right) ? left+1 : right+1;
// }

// int isBalanceTree2(pavlTree T)
// {
//     if(NULL == T)
//         return 1;

//     int left = height(T->lchild);
//     int right = height(T->rchild);

//     int diff = left - right;
//     if(diff > 1 || diff < -1)
//     {
//         printf("T %d left height = %d, right height = %d\n", T->data->key, left, right);
//         return 0;
//     }

//     return isBalanceTree(T->lchild) && isBalanceTree(T->rchild);
// }

int isBalanceTree(pavlTree T)
{
    if(NULL == T)
        return 1;
    int bf = T->data->otherInfo.balanceFactor;
    if( bf < -1 || bf > 1)
        return 0;
    return isBalanceTree(T->lchild) && isBalanceTree(T->rchild);
}


// 根据索引返回元素值   (也可依赖nodeNum数量判断)
pavlTree selectAvl(pavlTree T, int index)
{
    if(!T)  return NULL;
    if(index == T->data->otherInfo.index)
        return T;
    else if(index > T->data->otherInfo.index)
        return selectAvl(T->rchild, index);
    else
        return selectAvl(T->lchild, index);
}

// 根据元素值返回索引
int rankAvl(pavlTree T, pavlElement e)
{
    pavlTree p = searchAvl(T, e);
    if(!p)
        return -1;
    return p->data->otherInfo.index;
}


// 左右镜像
void lrMirror(pavlTree T)
{
    if(NULL == T)   return;
    pavlTree t = T->lchild;
    T->lchild = T->rchild;
    T->rchild = t;
    lrMirror(T->lchild);
    lrMirror(T->rchild);
}